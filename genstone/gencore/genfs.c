// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genfs.h"

#include "include/gendbg.h"
#include "include/genstring.h"
#include "include/gentooling.h"

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

gen_error_t gen_path_exists(const char* const restrict path, bool* const restrict out_exists) {
	GEN_FRAME_BEGIN(gen_path_exists);

	GEN_INTERNAL_BASIC_PARAM_CHECK(out_exists);
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	*out_exists = !access(path, F_OK);
	if(errno != ENOENT) GEN_ERROR_OUT_IF_ERRNO(access, errno);

	errno = EOK;

	GEN_ALL_OK;
}

gen_error_t gen_path_validate(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_validate);

	GEN_INTERNAL_BASIC_PARAM_CHECK(path);
	if(!path[0]) GEN_ERROR_OUT(GEN_TOO_SHORT, "`path` was too short (`length(path)` was 0)");

	// This is a kinda nonsensical test but it feels like the best way to do this
	// Testing if length of path is less than GEN_PATH_MAX
	size_t path_length = 0;
	gen_error_t error = gen_string_length(path, GEN_PATH_MAX + 1, GEN_PATH_MAX, &path_length);
	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

	GEN_ALL_OK;
}

gen_error_t gen_path_create_file(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_create_file);

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
	if(errno == ENOTDIR) {
		errno = EOK;

		fd = open(path, O_RDWR | O_CLOEXEC);
		GEN_ERROR_OUT_IF_ERRNO(open, errno);

		output_handle->is_directory = false;
		output_handle->file_handle = fd;

		GEN_ALL_OK;
	}

	GEN_ERROR_OUT_IF_ERRNO(open, errno);
	output_handle->is_directory = true;
	output_handle->file_handle = fd;
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

gen_error_t gen_handle_read(unsigned char* restrict output_buffer, const gen_filesystem_handle_t* const restrict handle, const size_t start, const size_t end) {
	GEN_FRAME_BEGIN(gen_handle_read);

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

gen_error_t gen_handle_write(const gen_filesystem_handle_t* const restrict handle, const size_t bytes_length, const unsigned char* const restrict buffer) {
	GEN_FRAME_BEGIN(gen_handle_write);

	GEN_INTERNAL_BASIC_PARAM_CHECK(handle);
	if(handle->is_directory) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a directory");
	GEN_INTERNAL_BASIC_PARAM_CHECK(buffer);

	write(handle->file_handle, buffer, bytes_length);
	GEN_ERROR_OUT_IF_ERRNO(write, errno);

	lseek(handle->file_handle, 0, SEEK_SET);
	GEN_ERROR_OUT_IF_ERRNO(lseek, errno);

	GEN_ALL_OK;
}

gen_error_t gen_directory_list(const gen_filesystem_handle_t* const restrict handle, const gen_directory_list_handler_t handler, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_directory_list);

	GEN_INTERNAL_BASIC_PARAM_CHECK(handle);
	if(!handle->is_directory) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`handle` was a file");
	GEN_INTERNAL_BASIC_PARAM_CHECK(handler);

	rewinddir(handle->directory_handle);

	struct dirent* entry;
	while((entry = readdir(handle->directory_handle))) {
		GEN_ERROR_OUT_IF_ERRNO(readdir, errno);

		if(entry->d_name[0] == '.' && entry->d_name[1] == '\0') continue;
		if(entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == '\0') continue;

		handler(entry->d_name, passthrough);
	}
	GEN_ERROR_OUT_IF_ERRNO(readdir, errno);

	rewinddir(handle->directory_handle);

	GEN_ALL_OK;
}

#if PLATFORM == DWN
static void gen_internal_filewatch_dwn_dircount(__unused const char* const restrict path, void* const restrict passthrough) {
	++(*(size_t*) passthrough);
}
#endif

#ifndef GEN_FS_FILEWATCH_USE_SYSLIB
/**
 * Whether to use the system library where implemented to get filewatch functionality.
 * @note Disabling may make results more consistent across platforms as the alternative uses standard utilities.
 */
#define GEN_FS_FILEWATCH_USE_SYSLIB ENABLED
#endif

gen_error_t gen_filewatch_create(gen_filesystem_handle_t* const restrict out_handle, const gen_filesystem_handle_t* const restrict handle) {
	GEN_FRAME_BEGIN(gen_filewatch_create);

	GEN_INTERNAL_BASIC_PARAM_CHECK(out_handle);
	GEN_INTERNAL_BASIC_PARAM_CHECK(handle);

#if PLATFORM == LNX && GEN_FS_FILEWATCH_USE_SYSLIB == ENABLED
	out_handle->is_directory = false;

	char pipe_name[GEN_PATH_MAX + 1] = {0};
	snprintf(pipe_name, GEN_PATH_MAX, "/proc/self/fd/%i", handle->file_handle);
	GEN_ERROR_OUT_IF_ERRNO(snprintf, errno);

	char path[GEN_PATH_MAX + 1] = {0};
	readlink(pipe_name, path, GEN_PATH_MAX);
	GEN_ERROR_OUT_IF_ERRNO(readlink, errno);

	out_handle->file_handle = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
	GEN_ERROR_OUT_IF_ERRNO(inotify_init1, errno);
	inotify_add_watch(out_handle->file_handle, path, IN_ATTRIB | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO);
	GEN_ERROR_OUT_IF_ERRNO(inotify_add_watch, errno);
#else
	out_handle->file_handle = dup(handle->file_handle);
	GEN_ERROR_OUT_IF_ERRNO(dup, errno);
	out_handle->directory_handle = fdopendir(out_handle->file_handle);
	GEN_ERROR_OUT_IF_ERRNO(fdopendir, errno);
	out_handle->is_directory = handle->is_directory;

	gen_error_t error = gen_directory_list(handle, gen_internal_filewatch_dwn_dircount, &out_handle->internal_directory_length);
	GEN_ERROR_OUT_IF(error, "`gen_directory_list` failed");

	fstat(out_handle->file_handle, &out_handle->internal_descriptor_details);
	GEN_ERROR_OUT_IF_ERRNO(fstat, errno);
#endif

	GEN_ALL_OK;
}

gen_error_t gen_filewatch_poll(gen_filesystem_handle_t* const restrict handle, gen_filewatch_event_t* const restrict out_event) {
	GEN_FRAME_BEGIN(gen_filewatch_poll);

	GEN_INTERNAL_BASIC_PARAM_CHECK(handle);
	GEN_INTERNAL_BASIC_PARAM_CHECK(out_event);

	*out_event = GEN_FILEWATCH_NONE;

#if PLATFORM == LNX && GEN_FS_FILEWATCH_USE_SYSLIB == ENABLED
	struct pollfd fd = {handle->file_handle, POLLIN, 0};

	fd.revents = 0;
	poll(&fd, 1, 0);
	GEN_ERROR_OUT_IF_ERRNO(poll, errno);

	if(fd.revents & POLLIN) {
		unsigned int events_size;
		ioctl(handle->file_handle, FIONREAD, &events_size);
		GEN_ERROR_OUT_IF_ERRNO(ioctl, errno);

		unsigned char* raw_events = NULL;
		gen_error_t error = gzalloc_aligned((void**) &raw_events, events_size, sizeof(unsigned char), alignof(struct inotify_event));
		GEN_ERROR_OUT_IF(error, "`gzalloc_aligned` failed");

		read(handle->file_handle, raw_events, events_size);
		if(errno != EOK) {
			error = gfree(raw_events);
			GEN_ERROR_OUT_IF(error, "`gfree` failed");

			GEN_ERROR_OUT_ERRNO(read, errno);
		}

		unsigned int offset = 0;
		while(offset < events_size) {
			struct inotify_event* const event = (struct inotify_event*) (raw_events + offset);

			if(event->mask & IN_ATTRIB || event->mask & IN_MODIFY) *out_event |= GEN_FILEWATCH_MODIFIED;
			if(event->mask & IN_CREATE) *out_event |= GEN_FILEWATCH_CREATED;
			if(event->mask & IN_DELETE || event->mask & IN_DELETE_SELF) *out_event |= GEN_FILEWATCH_DELETED;
			if(event->mask & IN_MOVE_SELF || event->mask & IN_MOVED_FROM || event->mask & IN_MOVED_TO) *out_event |= GEN_FILEWATCH_MOVED;

			offset += sizeof(struct inotify_event) + event->len;
		}
		error = gfree(raw_events);
		GEN_ERROR_OUT_IF(error, "`gfree` failed");
	}

	GEN_ALL_OK;
#else
	struct stat file_info;
	fstat(handle->file_handle, &file_info);
	GEN_ERROR_OUT_IF_ERRNO(fstat, errno);

	if(!(file_info.st_mtimespec.tv_sec == handle->internal_descriptor_details.st_mtimespec.tv_sec && file_info.st_mtimespec.tv_nsec == handle->internal_descriptor_details.st_mtimespec.tv_nsec) || !(file_info.st_ctimespec.tv_sec == handle->internal_descriptor_details.st_ctimespec.tv_sec && file_info.st_ctimespec.tv_nsec == handle->internal_descriptor_details.st_ctimespec.tv_nsec)) {
		if(handle->is_directory) {
			size_t items_length = 0;
			gen_error_t error = gen_directory_list(handle, gen_internal_filewatch_dwn_dircount, &items_length);
			GEN_ERROR_OUT_IF(error, "`gen_directory_list` failed");

			if(handle->internal_directory_length > items_length)
				*out_event |= GEN_FILEWATCH_DELETED;
			else if(handle->internal_directory_length < items_length)
				*out_event |= GEN_FILEWATCH_CREATED;
			else
				*out_event |= GEN_FILEWATCH_MODIFIED;

			handle->internal_directory_length = items_length;
		}
		else {
			*out_event |= GEN_FILEWATCH_MODIFIED;
		}
	}

	handle->internal_descriptor_details = file_info;
	GEN_ERROR_OUT_IF_ERRNO(memcpy_s, errno);

	GEN_ALL_OK;
#endif
}

gen_error_t gen_filewatch_destroy(gen_filesystem_handle_t* const restrict handle) {
	GEN_FRAME_BEGIN(gen_filewatch_destroy);

	GEN_INTERNAL_BASIC_PARAM_CHECK(handle);

#if PLATFORM == LNX && GEN_FS_FILEWATCH_USE_SYSLIB == ENABLED
	close(handle->file_handle);
	GEN_ERROR_OUT_IF_ERRNO(close, errno);
#else
	gen_error_t error = gen_handle_close(handle);
	GEN_ERROR_OUT_IF(error, "`gen_handle_close` failed");
#endif

	GEN_ALL_OK;
}
