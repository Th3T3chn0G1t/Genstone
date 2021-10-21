// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genfs.h"

#include "include/gendbg.h"
#include "include/gentooling.h"

#if GEN_DEBUG_PATH_VALIDATION == ENABLED
/**
 * Validates the path parameter to filesystem functions
 * @param path the path to validate
 * @see GEN_DEBUG_PATH_VALIDATION
 */
#define GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path) \
	do { \
		if(!path) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`" #path "` was NULL"); \
		gen_error_t path_error = gen_path_validate(path); \
		if(path_error) GEN_ERROR_OUT(path_error, "`" #path "` was invalid"); \
	} while(0)
#else
#define GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path) (void) path
#endif

#define GEN_INTERNAL_FS_FP_HANDLE_ERR(handle, fhandle_index, error) \
	do { \
		if(!error) { \
			if(ferror(handle->file_handles[fhandle_index])) \
				error = errno; \
			else \
				error = 0; \
			clearerr(handle->file_handles[fhandle_index]); \
			if(error) GEN_ERROR_OUT(gen_convert_errno(errno), "Filesystem error"); \
		} \
	} while(0)

gen_error_t gen_path_canonical(char* restrict output_path, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_canonical);

	if(!output_path) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_path` was NULL");
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

#if PLATFORM == WIN
	// Getting an error out of this function is very strange
	// We just have to presume that GEN_PATH_MAX will always be enough storage
	unsigned long error = GetFullPathNameA(path, GEN_PATH_MAX, output_path, NULL);
	if(!error) GEN_ERROR_OUT_WINERR(GetFullPathNameA, GetLastError());
#else
	if(!realpath(path, output_path)) GEN_ERROR_OUT_ERRNO(realpath, errno);
#endif

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_path_filename(char* restrict output_filename, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_filename);

	if(!output_filename) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_filename` was NULL");
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	errno_t error = strcpy_s(output_filename, GEN_PATH_MAX, strrchr(path, '/') + 1);
	if(error) GEN_ERROR_OUT_ERRNO(strcpy_s, errno);

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_path_pathname(char* restrict output_path, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_pathname);

	if(!output_path) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_path` was NULL");
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	size_t mark = (size_t) ((strrchr(path, '/') + 1) - path);
	errno_t error = strncpy_s(output_path, GEN_PATH_MAX, path, mark);
	if(error) GEN_ERROR_OUT_ERRNO(strncpy_s, errno);
	output_path[mark - 1] = '\0';

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_path_extension(char* restrict output_extension, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_extension);

	if(!output_extension) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_extension` was NULL");
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	size_t mark = (size_t) (strchr(strrchr(path, '/'), '.') - path);
	errno_t error = strcpy_s(output_extension, GEN_PATH_MAX, path + mark);
	if(error) GEN_ERROR_OUT_ERRNO(strcpy_s, errno);
	output_extension[mark - 1] = '\0';

	GEN_ERROR_OUT(GEN_OK, "");
}

bool gen_path_exists(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_exists);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

#if PLATFORM == WIN
	return PathFileExistsA(path);
#else
	return !access(path, F_OK);
#endif
}

gen_error_t gen_path_validate(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_validate);

	if(!path) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`path` was NULL");
	if(!path[0]) GEN_ERROR_OUT(GEN_TOO_SHORT, "`path` was too short (`strlen(path)` < 1)");

		// macOS apparently has no restrictions on file names?
		// (might have to do black magic voodoo for filenames containing `/`)
		// Linux/BSD apparently only prevent using /
		// In practise its probably more complicated, but this will do for now
#if PLATFORM == WIN
	const size_t len = strlen(path);

	if(len > GEN_PATH_MAX) {
		GEN_ERROR_OUT(GEN_TOO_LONG, "`path` was too long (`strlen(path)` > GEN_PATH_MAX)");
	}
	GEN_FOREACH_PTR(i, path_char, len, path) {
		const static char invalid_chars[] = ":*?\"<>|";
		GEN_FOREACH_PTR(j, invalid, sizeof(invalid_chars), invalid_chars) {
			if(*path_char == *invalid) {
				GEN_ERROR_OUT(GEN_BAD_CONTENT, "`path` contained an invalid character");
			}
		}
	}
#else
	if(strlen(path) > GEN_PATH_MAX) GEN_ERROR_OUT(GEN_TOO_LONG, "`path` was too long (`strlen(path)` > GEN_PATH_MAX)");
#endif

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_path_create_file(const char* const restrict path) {
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	FILE* stream;
	errno_t error = fopen_s(&stream, path, "w+");
	if(error) GEN_ERROR_OUT_ERRNO(fopen_s, errno);
	error = fclose(stream);
	if(error) GEN_ERROR_OUT_ERRNO(fclose, errno);

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_path_create_dir(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_create_dir);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

#if PLATFORM == WIN
	int error = CreateDirectoryA(path, NULL);
	if(error) GEN_ERROR_OUT_WINERR(CreateDirectoryA, GetLastError());
#else
	errno_t error = mkdir(path, 0777);
	if(error) GEN_ERROR_OUT_ERRNO(mkdir, errno);
#endif

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_path_delete(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_delete);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	errno_t error = remove(path);
	if(error) GEN_ERROR_OUT_ERRNO(remove, errno);

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_handle_open(gen_filesystem_handle_t* restrict output_handle, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_handle_open);

	if(!output_handle) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_handle` was NULL");
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	errno_t error = 0;
	error = strcpy_s(output_handle->path, GEN_PATH_MAX, path);
	if(error) GEN_ERROR_OUT_ERRNO(strcpy_s, errno);

	struct stat s;
	error = stat(path, &s);
	if(error) GEN_ERROR_OUT_ERRNO(stat, errno);
	if(S_ISDIR(s.st_mode)) {
		output_handle->dir = true;
		output_handle->directory_handle = opendir(path);
		if(!output_handle->directory_handle) GEN_ERROR_OUT_ERRNO(opendir, errno);
	}
	else {
		output_handle->dir = false;
		error = fopen_s(&output_handle->file_handles[1], path, "w+");
		if(error || !output_handle->file_handles[1]) GEN_ERROR_OUT_ERRNO(fopen_s, errno);
		error = fopen_s(&output_handle->file_handles[0], path, "r");
		if(error || !output_handle->file_handles[0]) GEN_ERROR_OUT_ERRNO(fopen_s, errno);
	}

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_handle_close(gen_filesystem_handle_t* const restrict handle) {
	GEN_FRAME_BEGIN(gen_handle_close);

	if(!handle) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`handle` was NULL");

	free(handle->path);

	if(handle->dir) {
		errno_t error = closedir(handle->directory_handle);
		if(error) GEN_ERROR_OUT_ERRNO(closedir, errno);
	}
	else {
		errno_t error = fclose(handle->file_handles[0]);
		if(error) GEN_ERROR_OUT_ERRNO(fclose, errno);
		error = fclose(handle->file_handles[1]);
		if(error) GEN_ERROR_OUT_ERRNO(fclose, errno);
	}

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_handle_size(size_t* const restrict out_size, const gen_filesystem_handle_t* const restrict handle) {
	GEN_FRAME_BEGIN(gen_handle_size);

	if(!handle) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`handle` was NULL");
	if(handle->dir) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a directory");

	int error = fseek(handle->file_handles[0], 0, SEEK_END);
	if(error) GEN_ERROR_OUT_ERRNO(fseek, errno);
	size_t mark = (size_t) ftell(handle->file_handles[0]);
	if(mark == SIZE_MAX) GEN_ERROR_OUT_ERRNO(ftell, errno);

	rewind(handle->file_handles[0]);

	*out_size = mark;
	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_file_read(uint8_t* restrict output_buffer, const gen_filesystem_handle_t* const restrict handle, const size_t start, const size_t end) {
	GEN_FRAME_BEGIN(gen_file_read);

	if(!handle) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`handle` was NULL");
	if(handle->dir) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a directory");
	if(!output_buffer) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_buffer` was NULL");

	int error = fseek(handle->file_handles[0], (long) start, SEEK_SET);
	if(error) GEN_ERROR_OUT_ERRNO(fseek, errno);

	error = (int) fread(output_buffer, sizeof(uint8_t), end - start, handle->file_handles[0]);
	GEN_INTERNAL_FS_FP_HANDLE_ERR(handle, 0, error);

	rewind(handle->file_handles[0]);

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_file_write(const gen_filesystem_handle_t* const restrict handle, const size_t n_bytes, const uint8_t* const restrict buffer) {
	GEN_FRAME_BEGIN(gen_file_write);

	if(!handle) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`handle` was NULL");
	if(handle->dir) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a directory");
	if(!buffer) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`buffer` was NULL");

	int error = (int) fwrite(buffer, sizeof(uint8_t), n_bytes, handle->file_handles[1]);
	GEN_INTERNAL_FS_FP_HANDLE_ERR(handle, 1, error);

	rewind(handle->file_handles[1]);

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_directory_list(const gen_filesystem_handle_t* const restrict handle, const gen_directory_list_handler_t handler, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_directory_list);

	if(!handle) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`handle` was NULL");
	if(!handle->dir) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a directory");
	if(!handler) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`handler` was NULL");

	struct dirent* entry;
	errno = 0;
	while((entry = readdir(handle->directory_handle))) {
		if(!entry && errno) GEN_ERROR_OUT_ERRNO(readdir, errno);
		if(entry->d_name[0] == '.' && entry->d_name[1] == '\0') continue;
		if(entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == '\0') continue;

		handler(entry->d_name, passthrough);

		errno = 0;
	}
	rewinddir(handle->directory_handle);

	GEN_ERROR_OUT(GEN_OK, "");
}
