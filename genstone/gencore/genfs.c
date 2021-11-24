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

#define GEN_INTERNAL_FS_FP_HANDLE_ERR(handle, error) \
	do { \
		if(!error) { \
			if(ferror(handle->file_handle)) \
				error = errno; \
			else \
				error = 0; \
			clearerr(handle->file_handle); \
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
	GetFullPathNameA(path, GEN_PATH_MAX, output_path, NULL);
	GEN_ERROR_OUT_IF_WINERR(GetFullPathNameA, GetLastError());
#else
	realpath(path, output_path);
	GEN_ERROR_OUT_IF_ERRNO(realpath, errno);
#endif

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_path_filename(char* restrict output_filename, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_filename);

	if(!output_filename) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_filename` was NULL");
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	strcpy_s(output_filename, GEN_PATH_MAX, strrchr(path, '/') + 1);
	GEN_ERROR_OUT_IF_ERRNO(strcpy_s, errno);

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_path_pathname(char* restrict output_path, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_pathname);

	if(!output_path) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_path` was NULL");
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	size_t mark = (size_t) ((strrchr(path, '/') + 1) - path);
	strncpy_s(output_path, GEN_PATH_MAX, path, mark);
	GEN_ERROR_OUT_IF_ERRNO(strncpy_s, errno);
	output_path[mark - 1] = '\0';

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_path_extension(char* restrict output_extension, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_extension);

	if(!output_extension) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_extension` was NULL");
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	const char* final_pathseg_terminator = strrchr(path, '/');
	if(!final_pathseg_terminator) final_pathseg_terminator = path;
	const char* const ext_start = strchr(final_pathseg_terminator, '.');
	size_t mark = ext_start ? (size_t) (ext_start - path) : 0;
	strcpy_s(output_extension, GEN_PATH_MAX, path + mark);
	GEN_ERROR_OUT_IF_ERRNO(strcpy_s, errno);
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
	fopen_s(&stream, path, "w+");
	GEN_ERROR_OUT_IF_ERRNO(fopen_s, errno);
	fclose(stream);
	GEN_ERROR_OUT_IF_ERRNO(fclose, errno);

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_path_create_dir(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_create_dir);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

#if PLATFORM == WIN
	CreateDirectoryA(path, NULL);
	GEN_ERROR_OUT_IF_WINERR(CreateDirectoryA, GetLastError());
#else
	mkdir(path, 0777);
	GEN_ERROR_OUT_IF_ERRNO(mkdir, errno);
#endif

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_path_delete(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_delete);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	struct stat s;
	stat(path, &s);
	GEN_ERROR_OUT_IF_ERRNO(stat, errno);
	if(S_ISDIR(s.st_mode)) {
#if PLATFORM == WIN
		RemoveDirectoryA(path);
		GEN_ERROR_OUT_IF_WINERR(RemoveDirectoryA, GetLastError());
#else
		rmdir(path);
		GEN_ERROR_OUT_IF_ERRNO(rmdir, errno);
#endif
	}
	else {
#if PLATFORM == WIN
		DeleteFileA(path);
		GEN_ERROR_OUT_IF_WINERR(DeleteFileA, GetLastError());
#else
		unlink(path);
		GEN_ERROR_OUT_IF_ERRNO(unlink, errno);
#endif
	}

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_handle_open(gen_filesystem_handle_t* restrict output_handle, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_handle_open);

	if(!output_handle) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_handle` was NULL");
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	if(!output_handle->path) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_handle->path` was NULL");
	strcpy_s(output_handle->path, GEN_PATH_MAX, path);
	GEN_ERROR_OUT_IF_ERRNO(strcpy_s, errno);

	int fd = open(path, O_DIRECTORY | O_RDONLY);
	if(fd == -1 && errno == ENOTDIR) {
		errno = EOK;

		fd = open(path, O_RDWR);

		output_handle->dir = false;
		output_handle->file_handle = fdopen(fd, "r+");
		GEN_ERROR_OUT_IF_ERRNO(fdopen, errno);

		GEN_ERROR_OUT(GEN_OK, "");
	}

	GEN_ERROR_OUT_IF_ERRNO(open, errno);
	output_handle->dir = true;
	output_handle->directory_handle = fdopendir(fd);
	GEN_ERROR_OUT_IF_ERRNO(opendir, errno);

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_handle_close(gen_filesystem_handle_t* const restrict handle) {
	GEN_FRAME_BEGIN(gen_handle_close);

	if(!handle) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`handle` was NULL");

	if(handle->dir) {
		closedir(handle->directory_handle);
		GEN_ERROR_OUT_IF_ERRNO(closedir, errno);
	}
	else {
		fclose(handle->file_handle);
		GEN_ERROR_OUT_IF_ERRNO(fclose, errno);
	}

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_handle_size(size_t* const restrict out_size, const gen_filesystem_handle_t* const restrict handle) {
	GEN_FRAME_BEGIN(gen_handle_size);

	if(!handle) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`handle` was NULL");
	if(handle->dir) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a directory");

	fseek(handle->file_handle, 0, SEEK_END);
	GEN_ERROR_OUT_IF_ERRNO(fseek, errno);
	size_t mark = (size_t) ftell(handle->file_handle);
	if(mark == SIZE_MAX) GEN_ERROR_OUT_ERRNO(ftell, errno);

	rewind(handle->file_handle);

	*out_size = mark;
	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_file_read(uint8_t* restrict output_buffer, const gen_filesystem_handle_t* const restrict handle, const size_t start, const size_t end) {
	GEN_FRAME_BEGIN(gen_file_read);

	if(!handle) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`handle` was NULL");
	if(handle->dir) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a directory");
	if(!output_buffer) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`output_buffer` was NULL");

	int error = fseek(handle->file_handle, (long) start, SEEK_SET);
	GEN_ERROR_OUT_IF_ERRNO(fseek, errno);

	error = (int) fread(output_buffer, sizeof(uint8_t), end - start, handle->file_handle);
	GEN_INTERNAL_FS_FP_HANDLE_ERR(handle, error);

	rewind(handle->file_handle);

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gen_file_write(const gen_filesystem_handle_t* const restrict handle, const size_t n_bytes, const uint8_t* const restrict buffer) {
	GEN_FRAME_BEGIN(gen_file_write);

	if(!handle) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`handle` was NULL");
	if(handle->dir) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a directory");
	if(!buffer) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`buffer` was NULL");

	int error = (int) fwrite(buffer, sizeof(uint8_t), n_bytes, handle->file_handle);
	GEN_INTERNAL_FS_FP_HANDLE_ERR(handle, error);

	rewind(handle->file_handle);

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
