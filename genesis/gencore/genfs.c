#include "include/genfs.h"

static gen_filesystem_error_t gen_fs_convert_errno(int error) {
    switch(error) {
        case EACCES: return GEN_FS_PERMISSION;
        case EINVAL: return GEN_FS_INVALID_PARAMETER;
        case EIO: return GEN_FS_IO;
        case ELOOP: return GEN_FS_TOO_LONG;
        case ENAMETOOLONG: return GEN_FS_TOO_LONG;
        case ENOENT: return GEN_FS_NO_SUCH_OBJECT;
        case ENOMEM: return GEN_FS_OUT_OF_MEMORY;
        case ENOTDIR: return GEN_FS_WRONG_OBJECT_TYPE;
#if PLATFORM != WIN // Bit random to be missing but okay
        case EDQUOT: return GEN_FS_OUT_OF_SPACE;
#endif
        case EEXIST: return GEN_FS_ALREADY_EXISTS;
        case EMLINK: return GEN_FS_TOO_LONG;
        case ENOSPC: return GEN_FS_OUT_OF_SPACE;
        case EPERM: return GEN_FS_PERMISSION;
        case EROFS: return GEN_FS_PERMISSION;
        case EMFILE: return GEN_FS_OUT_OF_HANDLES;
        case ENFILE: return GEN_FS_OUT_OF_HANDLES;
        default: return GEN_FS_UNKNOWN;
        return GEN_FS_OK;
    }
    return GEN_FS_OK;
}

static gen_filesystem_error_t gen_fs_convert_win_error(int error) {
#if PLATFORM == WIN
    switch(error) {
        case ERROR_ALREADY_EXISTS: return GEN_FS_ALREADY_EXISTS;
        case ERROR_PATH_NOT_FOUND: return GEN_FS_NO_SUCH_OBJECT;
        default: return GEN_FS_UNKNOWN;
    }
#else
    return GEN_FS_UNKNOWN;
#endif
}

gen_filesystem_error_t gen_path_canonical(char* output_path, const char* path) {
#if PLATFORM == WIN
    // Getting an error out of this function is very strange
    // We just have to presume that GEN_PATH_MAX will always be enough storage
    int error = GetFullPathNameA(path, GEN_PATH_MAX, output_path, NULL);
    if(!error) return GEN_FS_UNKNOWN; // Converting GetLastError is nontrivial here (MSDN doesn't specify any errors)
#else
    char* error = realpath(path, output_path);
    if(error == NULL) return gen_fs_convert_errno(errno);
#endif
    return GEN_FS_OK;
}

gen_filesystem_error_t gen_path_relative(char* output_path, const char* path, const char* to) {
    size_t mark;
    while(path[mark] == to[mark]) mark++;
    strcpy(output_path, path + mark);

    return GEN_FS_OK;
}

gen_filesystem_error_t gen_path_filename(char* output_filename, const char* path) {
    strcpy(output_filename, strrchr(path, '/') + 1);

    return GEN_FS_OK;
}

gen_filesystem_error_t gen_path_pathname(char* output_path, const char* path) {
    size_t mark = strrchr(path, '/') - path;
    strncpy(output_path, path, mark);
    output_path[mark - 1] = '\0';

    return GEN_FS_OK;
}

gen_filesystem_error_t gen_path_extension(char* output_extension, const char* path) {
    size_t mark = strchr(strrchr(path, '/'), '.') - path;
    strncpy(output_extension, path, mark);
    output_extension[mark - 1] = '\0';

    return GEN_FS_OK;
}

bool gen_path_exists(const char* path) {
#if PLATFORM == WIN
    return PathFileExistsA(path);
#else
    return !access(path, F_OK);
#endif
}

gen_path_error_t gen_path_validate(const char* path) {
    if(!path[0]) return GEN_PATH_INVALID_LENGTH;

    // macOS apparently has no restrictions on file names?
    // and Linux/BSD apparently only prevent using /
    // In practise its probably more complicated, but this will do for now
#if PLATFORM == WIN
    for(size_t i = 0; path[i]; i++) {
        // Checking here instead of looping over the path twice should be faster for the case where we need to check the path's contents
        if(i > GEN_PATH_MAX) return GEN_PATH_INVALID_LENGTH;
        const static char invalid_chars[] = "/\\:*?\"<>|";
        for(size_t j = 0; j < sizeof(invalid_chars); j++)
            if(path[i] == invalid_chars[j]) return GEN_PATH_INVALID_CHAR;
    }
#else
    if(strlen(path) > GEN_PATH_MAX) return GEN_PATH_INVALID_LENGTH;
#endif

    return GEN_PATH_OK;
}

gen_filesystem_error_t gen_path_create_dir(const char* path) {
#if PLATFORM == WIN
    int error = CreateDirectoryA(path, NULL);
    if(!error) return gen_fs_convert_win_error(GetLastError());
#else
    int error = mkdir(path, 0777);
    if(error) return gen_fs_convert_errno(errno);
#endif

    return GEN_FS_OK;
}

gen_filesystem_error_t gen_path_delete(const char* path) {
    int error = remove(path);
    if(error) return gen_fs_convert_errno(errno);

    return GEN_FS_OK;
}

gen_filesystem_error_t gen_handle_open(gen_filesystem_handle_t* output_handle, const char* path) {
    output_handle->path = strdup(path);

    struct stat s;
    int error = stat(path, &s);
    if(error) return gen_fs_convert_errno(errno);
    if(S_ISDIR(s.st_mode)) {
        output_handle->dir = true;
        output_handle->directory_handle = opendir(path);
        if(!output_handle->directory_handle) return gen_fs_convert_errno(errno);
    }
    else {
        output_handle->dir = false;
        output_handle->file_handles[0] = fopen(path, "r");
        if(!output_handle->file_handles[0]) return gen_fs_convert_errno(errno);
        output_handle->file_handles[1] = fopen(path, "w");
        if(!output_handle->file_handles[1]) return gen_fs_convert_errno(errno);
    }

    return GEN_FS_OK;
}

gen_filesystem_error_t gen_handle_close(gen_filesystem_handle_t* handle) {
    free(handle->path);

    if(handle->dir) {
        closedir(handle->directory_handle);
    }
    else {
        fclose(handle->file_handles[0]);
        fclose(handle->file_handles[1]);
    }

    return GEN_FS_OK;
}

size_t gen_handle_size(const gen_filesystem_handle_t* handle) {
    fseek(handle->file_handles[0], 0, SEEK_END);
    size_t mark = (size_t) ftell(handle->file_handles[0]);

    rewind(handle->file_handles[0]);

    return mark;
}

gen_filesystem_error_t gen_file_read(unsigned char* output_buffer, const gen_filesystem_handle_t* handle, const size_t start, const size_t end) {
    fseek(handle->file_handles[0], (long) start, SEEK_SET);

    int error = fread(output_buffer, sizeof(char), end - start, handle->file_handles[0]);
    if(!error) {
        if(ferror(handle->file_handles[0])) error = errno;
        else error = 0;
        clearerr(handle->file_handles[0]);
        if(error) return gen_fs_convert_errno(error);
    }

    rewind(handle->file_handles[0]);

    return GEN_FS_OK;
}

gen_filesystem_error_t gen_file_write(const gen_filesystem_handle_t* handle, const size_t n_bytes, const unsigned char* buffer) {

    return GEN_FS_OK;
}

gen_filesystem_error_t gen_directory_list(const gen_filesystem_handle_t* handle, const gen_directory_list_handler_t handler, void* passthrough) {

    return GEN_FS_OK;
}
