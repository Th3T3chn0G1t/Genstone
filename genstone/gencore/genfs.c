// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genfs.h"

#include "include/gendbg.h"
#include "include/gentooling.h"

#include <sys/inotify.h>

#if GEN_DEBUG_PATH_VALIDATION == ENABLED
/**
 * Validates the path parameter to filesystem functions
 * @param[in] path the path to validate
 * @see GEN_DEBUG_PATH_VALIDATION
 */
#define GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path) \
	do { \
		GEN_INTERNAL_BASIC_PARAM_CHECK(path); \
		gen_error_t path_error = gen_path_validate(path); \
		if(path_error) GEN_ERROR_OUT(path_error, "`" #path "` was invalid"); \
	} while(0)
#else
#define GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path) (void) path
#endif

gen_error_t gen_path_canonical(char* restrict output_path, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_canonical);

	GEN_INTERNAL_BASIC_PARAM_CHECK(output_path);
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	realpath(path, output_path);
	GEN_ERROR_OUT_IF_ERRNO(realpath, errno);

	GEN_ALL_OK;
}

gen_error_t gen_path_filename(char* restrict output_filename, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_filename);

	GEN_INTERNAL_BASIC_PARAM_CHECK(output_filename);
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	strcpy_s(output_filename, GEN_PATH_MAX, strrchr(path, '/') + 1);
	GEN_ERROR_OUT_IF_ERRNO(strcpy_s, errno);

	GEN_ALL_OK;
}

gen_error_t gen_path_pathname(char* restrict output_path, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_pathname);

	GEN_INTERNAL_BASIC_PARAM_CHECK(output_path);
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	size_t mark = (size_t) ((strrchr(path, '/') + 1) - path);
	strncpy_s(output_path, GEN_PATH_MAX, path, mark);
	GEN_ERROR_OUT_IF_ERRNO(strncpy_s, errno);
	output_path[mark - 1] = '\0';

	GEN_ALL_OK;
}

gen_error_t gen_path_extension(char* restrict output_extension, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_extension);

	GEN_INTERNAL_BASIC_PARAM_CHECK(output_extension);
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	char* final_pathseg_terminator = NULL;
	strrchr_s(path, GEN_PATH_MAX, '/', &final_pathseg_terminator);
	if(!final_pathseg_terminator) {
		// We need to duplicate like this because `strrchr_s` doesn't take a `const char*`

		char* ext_start = NULL;
		strchr_s(path, GEN_PATH_MAX, '.', &ext_start);
		size_t mark = ext_start ? (size_t) (ext_start - path) : 0;
		strcpy_s(output_extension, GEN_PATH_MAX, path + mark);
		GEN_ERROR_OUT_IF_ERRNO(strcpy_s, errno);
		if(mark == 0) GEN_ERROR_OUT(GEN_UNKNOWN, "`mark` became invalid for some reason");
		output_extension[mark - 1] = '\0';

		GEN_ALL_OK;
	}
	char* ext_start = NULL;
	strchr_s(final_pathseg_terminator, GEN_PATH_MAX, '.', &ext_start);
	size_t mark = ext_start ? (size_t) (ext_start - path) : 0;
	strcpy_s(output_extension, GEN_PATH_MAX, path + mark);
	GEN_ERROR_OUT_IF_ERRNO(strcpy_s, errno);
	if(mark == 0) GEN_ERROR_OUT(GEN_UNKNOWN, "`mark` became invalid for some reason");
	output_extension[mark - 1] = '\0';

	GEN_ALL_OK;
}

bool gen_path_exists(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_exists);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	return !access(path, F_OK);
}

gen_error_t gen_path_validate(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_validate);

	GEN_INTERNAL_BASIC_PARAM_CHECK(path);
	if(!path[0]) GEN_ERROR_OUT(GEN_TOO_SHORT, "`path` was too short (`len(path)` was 0)");

	// This is a kinda nonsensical test but it feels like the best way to do this
	if(strnlen_s(path, GEN_PATH_MAX) > GEN_PATH_MAX) GEN_ERROR_OUT(GEN_TOO_LONG, "`path` was too long (`len(path)` > GEN_PATH_MAX)");

	GEN_ALL_OK;
}

gen_error_t gen_path_create_file(const char* const restrict path) {
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	int descriptor = open(path, O_RDWR | O_CREAT | O_CLOEXEC, 0777);
	GEN_ERROR_OUT_IF_ERRNO(open, errno);
	close(descriptor);
	GEN_ERROR_OUT_IF_ERRNO(close, errno);

	GEN_ALL_OK;
}

gen_error_t gen_path_create_dir(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_create_dir);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	mkdir(path, 0777);
	GEN_ERROR_OUT_IF_ERRNO(mkdir, errno);

	GEN_ALL_OK;
}

gen_error_t gen_path_delete(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_delete);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);
	if(rmdir(path) == -1 && errno == ENOTDIR) {
		errno = EOK;

		unlink(path);
		GEN_ERROR_OUT_IF_ERRNO(unlink, errno);

		GEN_ALL_OK;
	}
	GEN_ERROR_OUT_IF_ERRNO(rmdir, errno);

	GEN_ALL_OK;
}

gen_error_t gen_handle_open(gen_filesystem_handle_t* restrict output_handle, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_handle_open);

	GEN_INTERNAL_BASIC_PARAM_CHECK(output_handle);
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	int fd = open(path, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
	if(fd == -1 && errno == ENOTDIR) {
		errno = EOK;

		fd = open(path, O_RDWR | O_CLOEXEC);
		GEN_ERROR_OUT_IF_ERRNO(open, errno);

		output_handle->is_directory = false;
		output_handle->file_handle = fd;

		GEN_ALL_OK;
	}

	GEN_ERROR_OUT_IF_ERRNO(open, errno);
	output_handle->is_directory = true;
	output_handle->directory_handle = fdopendir(fd);
	GEN_ERROR_OUT_IF_ERRNO(fdopendir, errno);

	GEN_ALL_OK;
}

gen_error_t gen_handle_close(gen_filesystem_handle_t* const restrict handle) {
	GEN_FRAME_BEGIN(gen_handle_close);

	GEN_INTERNAL_BASIC_PARAM_CHECK(handle);

	if(handle->is_directory) {
		closedir(handle->directory_handle);
		GEN_ERROR_OUT_IF_ERRNO(closedir, errno);
	}
	else {
		close(handle->file_handle);
		GEN_ERROR_OUT_IF_ERRNO(close, errno);
	}

	GEN_ALL_OK;
}

gen_error_t gen_handle_size(size_t* const restrict out_size, const gen_filesystem_handle_t* const restrict handle) {
	GEN_FRAME_BEGIN(gen_handle_size);

	GEN_INTERNAL_BASIC_PARAM_CHECK(handle);
	if(handle->is_directory) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a directory");

	size_t mark = (size_t) lseek(handle->file_handle, 0, SEEK_END);
	GEN_ERROR_OUT_IF_ERRNO(lseek, errno);

	lseek(handle->file_handle, 0, SEEK_SET);
	GEN_ERROR_OUT_IF_ERRNO(lseek, errno);

	*out_size = mark;

	GEN_ALL_OK;
}

gen_error_t gen_file_read(uint8_t* restrict output_buffer, const gen_filesystem_handle_t* const restrict handle, const size_t start, const size_t end) {
	GEN_FRAME_BEGIN(gen_file_read);

	GEN_INTERNAL_BASIC_PARAM_CHECK(handle);
	if(handle->is_directory) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a directory");
	if(start >= end) GEN_ERROR_OUT(GEN_TOO_SHORT, "`start` >= `end`");
	GEN_INTERNAL_BASIC_PARAM_CHECK(output_buffer);

	lseek(handle->file_handle, (long) start, SEEK_SET);
	GEN_ERROR_OUT_IF_ERRNO(lseek, errno);

	read(handle->file_handle, output_buffer, end - start);
	GEN_ERROR_OUT_IF_ERRNO(read, errno);

	lseek(handle->file_handle, 0, SEEK_SET);
	GEN_ERROR_OUT_IF_ERRNO(lseek, errno);

	GEN_ALL_OK;
}

gen_error_t gen_file_write(const gen_filesystem_handle_t* const restrict handle, const size_t n_bytes, const uint8_t* const restrict buffer) {
	GEN_FRAME_BEGIN(gen_file_write);

	GEN_INTERNAL_BASIC_PARAM_CHECK(handle);
	if(handle->is_directory) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a directory");
	GEN_INTERNAL_BASIC_PARAM_CHECK(buffer);

	write(handle->file_handle, buffer, n_bytes);
	GEN_ERROR_OUT_IF_ERRNO(write, errno);

	lseek(handle->file_handle, 0, SEEK_SET);
	GEN_ERROR_OUT_IF_ERRNO(lseek, errno);

	GEN_ALL_OK;
}

gen_error_t gen_directory_list(const gen_filesystem_handle_t* const restrict handle, const gen_directory_list_handler_t handler, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_directory_list);

	GEN_INTERNAL_BASIC_PARAM_CHECK(handle);
	if(!handle->is_directory) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a directory");
	GEN_INTERNAL_BASIC_PARAM_CHECK(handler);

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

	GEN_ALL_OK;
}

// gen_error_t gen_filewatch_create(const gen_filewatch_handle_t* handle) {
// 	#if PLATFORM == LNX
// 	#elif PLATFORM == DWN
// 	#endif
// }

// gen_error_t gen_filewatch_add_path(const gen_filewatch_handle_t* handle, const char* const restrict path) {
// 	#if PLATFORM == LNX
// 	#elif PLATFORM == DWN
// 	#endif
// }

// gen_error_t gen_filewatch_poll(const gen_filewatch_handle_t* handle, bool* const restrict out_modified) {
// 	#if PLATFORM == LNX
// 	#elif PLATFORM == DWN
// 	#endif
// }

// gen_error_t gen_filewatch_destroy(const gen_filewatch_handle_t* handle) {
// 	#if PLATFORM == LNX
// 	#elif PLATFORM == DWN
// 	#endif
// }
