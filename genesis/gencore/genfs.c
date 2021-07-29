#include "include/genfs.h"

#define _GEN_FS_PATH_PARAMETER_VALIDATION(path) \
    do { \
        if(!path) return GEN_INVALID_PARAMETER; \
        gen_error_t path_error = gen_path_validate(path); \
        if(path_error) return path_error; \
    } while(0)

gen_error_t gen_path_canonical(char* output_path, const char* path) {
    if(!output_path) return GEN_INVALID_PARAMETER;
    _GEN_FS_PATH_PARAMETER_VALIDATION(path);

#if PLATFORM == WIN
    // Getting an error out of this function is very strange
    // We just have to presume that GEN_PATH_MAX will always be enough storage
    int error = GetFullPathNameA(path, GEN_PATH_MAX, output_path, NULL);
    if(!error) return gen_convert_winerr(GetLastError());
#else
    char* error = realpath(path, output_path);
    if(!error) return gen_convert_errno(errno);
#endif

    return GEN_OK;
}

gen_error_t gen_path_relative(char* output_path, const char* path, const char* to) {
    if(!output_path) return GEN_INVALID_PARAMETER;
    _GEN_FS_PATH_PARAMETER_VALIDATION(path);
    _GEN_FS_PATH_PARAMETER_VALIDATION(to);

    size_t mark;
    while(path[mark] == to[mark]) mark++;
    strcpy(output_path, path + mark);

    return GEN_OK;
}

gen_error_t gen_path_filename(char* output_filename, const char* path) {
    if(!output_filename) return GEN_INVALID_PARAMETER;
    _GEN_FS_PATH_PARAMETER_VALIDATION(path);

    strcpy(output_filename, strrchr(path, '/') + 1);

    return GEN_OK;
}

gen_error_t gen_path_pathname(char* output_path, const char* path) {
    if(!output_path) return GEN_INVALID_PARAMETER;
    _GEN_FS_PATH_PARAMETER_VALIDATION(path);

    size_t mark = strrchr(path, '/') - path;
    strncpy(output_path, path, mark);
    output_path[mark - 1] = '\0';

    return GEN_OK;
}

gen_error_t gen_path_extension(char* output_extension, const char* path) {
    if(!output_extension) return GEN_INVALID_PARAMETER;
    _GEN_FS_PATH_PARAMETER_VALIDATION(path);

    size_t mark = strchr(strrchr(path, '/'), '.') - path;
    strncpy(output_extension, path, mark);
    output_extension[mark - 1] = '\0';

    return GEN_OK;
}

bool gen_path_exists(const char* path) {
    _GEN_FS_PATH_PARAMETER_VALIDATION(path);

#if PLATFORM == WIN
    return PathFileExistsA(path);
#else
    return !access(path, F_OK);
#endif
}

gen_error_t gen_path_validate(const char* path) {
    if(!path) return GEN_INVALID_PARAMETER;
    if(!path[0]) return GEN_TOO_SHORT;

    // macOS apparently has no restrictions on file names?
    // and Linux/BSD apparently only prevent using /
    // In practise its probably more complicated, but this will do for now
#if PLATFORM == WIN
    for(size_t i = 0; path[i]; i++) {
        // Checking here instead of looping over the path twice should be faster for the case where we need to check the path's contents
        if(i > GEN_PATH_MAX) return GEN_TOO_LONG;
        const static char invalid_chars[] = "/\\:*?\"<>|";
        for(size_t j = 0; j < sizeof(invalid_chars); j++)
            if(path[i] == invalid_chars[j]) return GEN_WRONG_OBJECT_TYPE;
    }
#else
    if(strlen(path) > GEN_PATH_MAX) return GEN_TOO_LONG;
#endif

    return GEN_OK;
}

gen_error_t gen_path_create_dir(const char* path) {
    _GEN_FS_PATH_PARAMETER_VALIDATION(path);

#if PLATFORM == WIN
    int error = CreateDirectoryA(path, NULL);
    if(!error) return gen_convert_winerr(GetLastError());
#else
    int error = mkdir(path, 0777);
    if(error) return gen_convert_errno(errno);
#endif

    return GEN_OK;
}

gen_error_t gen_path_delete(const char* path) {
    _GEN_FS_PATH_PARAMETER_VALIDATION(path);

    int error = remove(path);
    if(error) return gen_convert_errno(errno);

    return GEN_OK;
}

gen_error_t gen_handle_open(gen_filesystem_handle_t* output_handle, const char* path) {
    if(!output_handle) return GEN_INVALID_PARAMETER;
    _GEN_FS_PATH_PARAMETER_VALIDATION(path);

    output_handle->path = strdup(path);

    struct stat s;
    int error = stat(path, &s);
    if(error) return gen_convert_errno(errno);
    if(S_ISDIR(s.st_mode)) {
        output_handle->dir = true;
        output_handle->directory_handle = opendir(path);
        if(!output_handle->directory_handle) return gen_convert_errno(errno);
    }
    else {
        output_handle->dir = false;
        output_handle->file_handles[0] = fopen(path, "r");
        if(!output_handle->file_handles[0]) return gen_convert_errno(errno);
        output_handle->file_handles[1] = fopen(path, "w");
        if(!output_handle->file_handles[1]) return gen_convert_errno(errno);
    }

    return GEN_OK;
}

gen_error_t gen_handle_close(gen_filesystem_handle_t* handle) {
    if(!handle) return GEN_INVALID_PARAMETER;

    free(handle->path);

    if(handle->dir) {
        int error = closedir(handle->directory_handle);
        if(error) return gen_convert_errno(errno);
    }
    else {
        int error = fclose(handle->file_handles[0]);
        if(error) return gen_convert_errno(errno);
        error = fclose(handle->file_handles[1]);
        if(error) return gen_convert_errno(errno);
    }

    return GEN_OK;
}

size_t gen_handle_size(const gen_filesystem_handle_t* handle) {
    if(!handle) return GEN_INVALID_PARAMETER;

    int error = fseek(handle->file_handles[0], 0, SEEK_END);
    if(error) return gen_convert_errno(errno);
    size_t mark = (size_t) ftell(handle->file_handles[0]);
    if(mark == SIZE_MAX) return gen_convert_errno(errno);

    rewind(handle->file_handles[0]);

    return mark;
}

gen_error_t gen_file_read(unsigned char* output_buffer, const gen_filesystem_handle_t* handle, const size_t start, const size_t end) {
    int error = fseek(handle->file_handles[0], (long) start, SEEK_SET);
    if(error) return gen_convert_errno(errno);

    error = fread(output_buffer, sizeof(char), end - start, handle->file_handles[0]);
    if(!error) {
        if(ferror(handle->file_handles[0])) error = errno;
        else error = 0;
        clearerr(handle->file_handles[0]);
        if(error) return gen_convert_errno(error);
    }

    rewind(handle->file_handles[0]);

    return GEN_OK;
}

gen_error_t gen_file_write(const gen_filesystem_handle_t* handle, const size_t n_bytes, const unsigned char* buffer) {

    return GEN_OK;
}

gen_error_t gen_directory_list(const gen_filesystem_handle_t* handle, const gen_directory_list_handler_t handler, void* passthrough) {

    return GEN_OK;
}
