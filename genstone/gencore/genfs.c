// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

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
		if(!path) return GEN_INVALID_PARAMETER; \
		gen_error_t path_error = gen_path_validate(path); \
		if(path_error) return path_error; \
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
			if(error) return gen_convert_errno(error); \
		} \
	} while(0)

gen_error_t gen_path_canonical(char* restrict output_path, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_canonical);

	if(!output_path) return GEN_INVALID_PARAMETER;
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

#if PLATFORM == WIN
	// Getting an error out of this function is very strange
	// We just have to presume that GEN_PATH_MAX will always be enough storage
	unsigned long error = GetFullPathNameA(path, GEN_PATH_MAX, output_path, NULL);
	if(!error) return gen_convert_winerr(GetLastError());
#else
	char* error = realpath(path, output_path);
	if(!error) return gen_convert_errno(errno);
#endif

	return GEN_OK;
}

gen_error_t gen_path_filename(char* restrict output_filename, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_filename);

	if(!output_filename) return GEN_INVALID_PARAMETER;
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	errno_t error = strcpy_s(output_filename, GEN_PATH_MAX, strrchr(path, '/') + 1);
	if(error) return gen_convert_errno(error);

	return GEN_OK;
}

gen_error_t gen_path_pathname(char* restrict output_path, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_pathname);

	if(!output_path) return GEN_INVALID_PARAMETER;
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	size_t mark = (size_t) ((strrchr(path, '/') + 1) - path);
	errno_t error = strncpy_s(output_path, GEN_PATH_MAX, path, mark);
	if(error) return gen_convert_errno(error);
	output_path[mark - 1] = '\0';

	return GEN_OK;
}

gen_error_t gen_path_extension(char* restrict output_extension, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_extension);

	if(!output_extension) return GEN_INVALID_PARAMETER;
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	size_t mark = (size_t) (strchr(strrchr(path, '/'), '.') - path);
	errno_t error = strcpy_s(output_extension, GEN_PATH_MAX, path + mark);
	if(error) return gen_convert_errno(error);
	output_extension[mark - 1] = '\0';

	return GEN_OK;
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

	if(!path) return GEN_INVALID_PARAMETER;
	if(!path[0]) return GEN_TOO_SHORT;

		// macOS apparently has no restrictions on file names?
		// and Linux/BSD apparently only prevent using /
		// In practise its probably more complicated, but this will do for now
#if PLATFORM == WIN
	const size_t len = strlen(path);

	if(len > GEN_PATH_MAX) {
		return GEN_TOO_LONG;
	}
	GEN_FOREACH_PTR(i, path_char, len, path) {
		const static char invalid_chars[] = "/\\:*?\"<>|";
		GEN_FOREACH_PTR(j, invalid, sizeof(invalid_chars), invalid_chars) {
			if(*path_char == *invalid) {
				return GEN_WRONG_OBJECT_TYPE;
			}
		}
	}
#else
	if(strlen(path) > GEN_PATH_MAX) return GEN_TOO_LONG;
#endif

	return GEN_OK;
}

gen_error_t gen_path_create_file(const char* const restrict path) {
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	FILE* const stream;
	errno_t error = fopen_s(&stream, path, 'w+');
	if(error) return gen_convert_errno(errno);
	error = fclose(stream);
	if(error) return gen_convert_errno(errno);

	return GEN_OK;
}

gen_error_t gen_path_create_dir(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_create_dir);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

#if PLATFORM == WIN
	int error = CreateDirectoryA(path, NULL);
	if(error) return gen_convert_winerr(GetLastError());
#else
	errno_t error = mkdir(path, 0777);
	if(error) return gen_convert_errno(errno);
#endif

	return GEN_OK;
}

gen_error_t gen_path_delete(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_delete);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	errno_t error = remove(path);
	if(error) return gen_convert_errno(errno);

	return GEN_OK;
}

gen_error_t gen_handle_open(gen_filesystem_handle_t* restrict output_handle, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_handle_open);

	if(!output_handle) return GEN_INVALID_PARAMETER;
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	errno_t error = 0;
	error = strcpy_s(output_handle->path, GEN_PATH_MAX, path);
	if(error) return gen_convert_errno(errno);

	struct stat s;
	error = stat(path, &s);
	if(error && errno != ENOENT) return gen_convert_errno(errno);
	if(S_ISDIR(s.st_mode)) {
		output_handle->dir = true;
		output_handle->directory_handle = opendir(path);
		if(!output_handle->directory_handle) return gen_convert_errno(errno);
	}
	else {
		output_handle->dir = false;
		error = fopen_s(&output_handle->file_handles[1], path, "w+");
		if(error || !output_handle->file_handles[1]) return gen_convert_errno(errno);
		error = fopen_s(&output_handle->file_handles[0], path, "r");
		if(error || !output_handle->file_handles[0]) return gen_convert_errno(errno);
	}

	return GEN_OK;
}

gen_error_t gen_handle_close(gen_filesystem_handle_t* const restrict handle) {
	GEN_FRAME_BEGIN(gen_handle_close);

	if(!handle) return GEN_INVALID_PARAMETER;

	free(handle->path);

	if(handle->dir) {
		errno_t error = closedir(handle->directory_handle);
		if(error) return gen_convert_errno(errno);
	}
	else {
		errno_t error = fclose(handle->file_handles[0]);
		if(error) return gen_convert_errno(errno);
		error = fclose(handle->file_handles[1]);
		if(error) return gen_convert_errno(errno);
	}

	return GEN_OK;
}

gen_error_t gen_handle_size(size_t* const restrict out_size, const gen_filesystem_handle_t* const restrict handle) {
	GEN_FRAME_BEGIN(gen_handle_size);

	if(!handle) return GEN_INVALID_PARAMETER;
	if(handle->dir) return GEN_WRONG_OBJECT_TYPE;

	int error = fseek(handle->file_handles[0], 0, SEEK_END);
	if(error) return gen_convert_errno(errno);
	size_t mark = (size_t) ftell(handle->file_handles[0]);
	if(mark == SIZE_MAX) return gen_convert_errno(errno);

	rewind(handle->file_handles[0]);

	*out_size = mark;
	return GEN_OK;
}

gen_error_t gen_file_read(uint8_t* restrict output_buffer, const gen_filesystem_handle_t* const restrict handle, const size_t start, const size_t end) {
	GEN_FRAME_BEGIN(gen_file_read);

	if(!handle) return GEN_INVALID_PARAMETER;
	if(handle->dir) return GEN_WRONG_OBJECT_TYPE;
	if(!output_buffer) return GEN_INVALID_PARAMETER;

	int error = fseek(handle->file_handles[0], (long) start, SEEK_SET);
	if(error) return gen_convert_errno(errno);

	error = (int) fread(output_buffer, sizeof(uint8_t), end - start, handle->file_handles[0]);
	GEN_INTERNAL_FS_FP_HANDLE_ERR(handle, 0, error);

	rewind(handle->file_handles[0]);

	return GEN_OK;
}

gen_error_t gen_file_write(const gen_filesystem_handle_t* const restrict handle, const size_t n_bytes, const uint8_t* const restrict buffer) {
	GEN_FRAME_BEGIN(gen_file_write);

	if(!handle) return GEN_INVALID_PARAMETER;
	if(handle->dir) return GEN_WRONG_OBJECT_TYPE;
	if(!buffer) return GEN_INVALID_PARAMETER;

	int error = (int) fwrite(buffer, sizeof(uint8_t), n_bytes, handle->file_handles[1]);
	GEN_INTERNAL_FS_FP_HANDLE_ERR(handle, 1, error);

	rewind(handle->file_handles[1]);

	return GEN_OK;
}

gen_error_t gen_directory_list(const gen_filesystem_handle_t* const restrict handle, const gen_directory_list_handler_t handler, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_directory_list);

	if(!handle) return GEN_INVALID_PARAMETER;
	if(!handle->dir) return GEN_WRONG_OBJECT_TYPE;
	if(!handler) return GEN_INVALID_PARAMETER;

	struct dirent* entry;
	errno = 0;
	while((entry = readdir(handle->directory_handle))) {
		if(!entry && errno) return gen_convert_errno(errno);
		if(entry->d_name[0] == '.' && entry->d_name[1] == '\0') continue;
		if(entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == '\0') continue;

		handler(entry->d_name, passthrough);

		errno = 0;
	}
	rewinddir(handle->directory_handle);

	return GEN_OK;
}
