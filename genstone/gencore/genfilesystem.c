// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genfs.h"
#include "include/genstring.h"

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <unistd.h>

#if GEN_PLATFORM == GEN_LINUX
#include <sys/inotify.h>
#endif
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

gen_error_t gen_path_canonical(char* restrict output_path, const char* const restrict path) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_path_canonical", (void*) gen_path_canonical, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(output_path);
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	realpath(path, output_path);
	gen_error_attach_backtrace_IF_ERRNO(realpath, errno);

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

gen_error_t gen_path_exists(const char* const restrict path, bool* const restrict out_exists) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_path_exists", (void*) gen_path_exists, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(out_exists);
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	*out_exists = !access(path, F_OK);
	if(errno != ENOENT) gen_error_attach_backtrace_IF_ERRNO(access, errno);

	errno = EOK;

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

gen_error_t gen_path_validate(const char* const restrict path) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_path_validate", (void*) gen_path_validate, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(path);
	if(!path[0]) gen_error_attach_backtrace(GEN_TOO_SHORT, GEN_LINENO, "`path` was too short (`length(path)` was 0)");

	// This is a kinda nonsensical test but it feels like the best way to do this
	// Testing if length of path is less than GEN_PATH_MAX
	size_t path_length = 0;
	gen_error_t error = gen_string_length(path, GEN_PATH_MAX + 1, GEN_PATH_MAX, &path_length);
	gen_error_attach_backtrace_IF(error, "`gen_string_length` failed");

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

gen_error_t gen_path_create_file(const char* const restrict path) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_path_create_file", (void*) gen_path_create_file, GEN_FILENAME);
	if(error.type) return error;

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	int descriptor = open(path, O_RDWR | O_CREAT | O_CLOEXEC, 0777);
	gen_error_attach_backtrace_IF_ERRNO(open, errno);
	close(descriptor);
	gen_error_attach_backtrace_IF_ERRNO(close, errno);

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

gen_error_t gen_path_create_dir(const char* const restrict path) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_path_create_dir", (void*) gen_path_create_dir, GEN_FILENAME);
	if(error.type) return error;

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	mkdir(path, 0777);
	gen_error_attach_backtrace_IF_ERRNO(mkdir, errno);

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

gen_error_t gen_path_delete(const char* const restrict path) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_path_delete", (void*) gen_path_delete, GEN_FILENAME);
	if(error.type) return error;

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);
	if(rmdir(path) == -1 && errno == ENOTDIR) {
		errno = EOK;

		unlink(path);
		gen_error_attach_backtrace_IF_ERRNO(unlink, errno);

		return (gen_error_t){GEN_OK, GEN_LINENO, ""};
	}
	gen_error_attach_backtrace_IF_ERRNO(rmdir, errno);

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

gen_error_t gen_filesystem_handle_open(gen_filesystem_handle_t* restrict output_handle, const char* const restrict path) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_filesystem_handle_open", (void*) gen_filesystem_handle_open, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(output_handle);
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	int fd = open(path, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
	if(errno == ENOTDIR) {
		errno = EOK;

		fd = open(path, O_RDWR | O_CLOEXEC);
		gen_error_attach_backtrace_IF_ERRNO(open, errno);

		output_handle->is_directory = false;
		output_handle->file_handle = fd;

		return (gen_error_t){GEN_OK, GEN_LINENO, ""};
	}

	gen_error_attach_backtrace_IF_ERRNO(open, errno);
	output_handle->is_directory = true;
	output_handle->file_handle = fd;
	output_handle->directory_handle = fdopendir(fd);
	gen_error_attach_backtrace_IF_ERRNO(fdopendir, errno);

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

gen_error_t gen_filesystem_handle_close(gen_filesystem_handle_t* const restrict handle) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_filesystem_handle_close", (void*) gen_filesystem_handle_close, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(handle);

	if(handle->is_directory) {
		closedir(handle->directory_handle);
		gen_error_attach_backtrace_IF_ERRNO(closedir, errno);
	}
	else {
		close(handle->file_handle);
		gen_error_attach_backtrace_IF_ERRNO(close, errno);
	}

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

gen_error_t gen_filesystem_handle_size(size_t* const restrict out_size, const gen_filesystem_handle_t* const restrict handle) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_filesystem_handle_size", (void*) gen_filesystem_handle_size, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(handle);
	if(handle->is_directory) gen_error_attach_backtrace(GEN_WRONG_OBJECT_TYPE, GEN_LINENO, "`handle` was a directory");

	size_t mark = (size_t) lseek(handle->file_handle, 0, SEEK_END);
	gen_error_attach_backtrace_IF_ERRNO(lseek, errno);

	lseek(handle->file_handle, 0, SEEK_SET);
	gen_error_attach_backtrace_IF_ERRNO(lseek, errno);

	*out_size = mark;

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

gen_error_t gen_filesystem_handle_read(unsigned char* restrict output_buffer, const gen_filesystem_handle_t* const restrict handle, const size_t start, const size_t end) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_filesystem_handle_read", (void*) gen_filesystem_handle_read, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(handle);
	if(handle->is_directory) gen_error_attach_backtrace(GEN_WRONG_OBJECT_TYPE, GEN_LINENO, "`handle` was a directory");
	if(start > end) gen_error_attach_backtrace(GEN_TOO_SHORT, GEN_LINENO, "`start` > `end`");
	GEN_NULL_CHECK(output_buffer);

	if(start == end) {
		lseek(handle->file_handle, 0, SEEK_SET);
		gen_error_attach_backtrace_IF_ERRNO(lseek, errno);

		return (gen_error_t){GEN_OK, GEN_LINENO, ""};
	}

	lseek(handle->file_handle, (long) start, SEEK_SET);
	gen_error_attach_backtrace_IF_ERRNO(lseek, errno);

	read(handle->file_handle, output_buffer, end - start);
	gen_error_attach_backtrace_IF_ERRNO(read, errno);

	lseek(handle->file_handle, 0, SEEK_SET);
	gen_error_attach_backtrace_IF_ERRNO(lseek, errno);

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

gen_error_t gen_filesystem_handle_write(const gen_filesystem_handle_t* const restrict handle, const size_t bytes_length, const unsigned char* const restrict buffer) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_filesystem_handle_write", (void*) gen_filesystem_handle_write, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(handle);
	if(handle->is_directory) gen_error_attach_backtrace(GEN_WRONG_OBJECT_TYPE, GEN_LINENO, "`handle` was a directory");
	GEN_NULL_CHECK(buffer);

	write(handle->file_handle, buffer, bytes_length);
	gen_error_attach_backtrace_IF_ERRNO(write, errno);

	lseek(handle->file_handle, 0, SEEK_SET);
	gen_error_attach_backtrace_IF_ERRNO(lseek, errno);

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

gen_error_t gen_directory_list(const gen_filesystem_handle_t* const restrict handle, const gen_directory_list_handler_t handler, void* const restrict passthrough) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_directory_list", (void*) gen_directory_list, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(handle);
	if(!handle->is_directory) gen_error_attach_backtrace(GEN_WRONG_OBJECT_TYPE, GEN_LINENO, "`handle` was a file");
	GEN_NULL_CHECK(handler);

	rewinddir(handle->directory_handle);

	struct dirent* entry;
	while((entry = readdir(handle->directory_handle))) {
		gen_error_attach_backtrace_IF_ERRNO(readdir, errno);

		if(entry->d_name[0] == '.' && entry->d_name[1] == '\0') continue;
		if(entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == '\0') continue;

		gen_error_t error = handler(entry->d_name, passthrough);
		gen_error_attach_backtrace_IF(error, "Call to directory list handler failed");
	}
	gen_error_attach_backtrace_IF_ERRNO(readdir, errno);

	rewinddir(handle->directory_handle);

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

#if GEN_FILESYSTEM_FILEWATCH_USE_SYSTEM_LIBRARY == DISABLED
GEN_INTERNAL_ERRORABLE gen_internal_filewatch_dwn_dircount(__unused const char* const restrict path, void* const restrict passthrough) {
	GEN_NULL_CHECK(passthrough);
	++(*(size_t*) passthrough);
	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}
#endif

gen_error_t gen_filewatch_create(gen_filesystem_handle_t* const restrict out_handle, const gen_filesystem_handle_t* const restrict handle) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_filewatch_create", (void*) gen_filewatch_create, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(out_handle);
	GEN_NULL_CHECK(handle);

#if PLATFORM == LNX && GEN_FILESYSTEM_FILEWATCH_USE_SYSTEM_LIBRARY == ENABLED
	out_handle->is_directory = false;

	char pipe_name[GEN_PATH_MAX + 1] = {0};
	snprintf(pipe_name, GEN_PATH_MAX, "/proc/self/fd/%i", handle->file_handle);
	gen_error_attach_backtrace_IF_ERRNO(snprintf, errno);

	char path[GEN_PATH_MAX + 1] = {0};
	readlink(pipe_name, path, GEN_PATH_MAX);
	gen_error_attach_backtrace_IF_ERRNO(readlink, errno);

	out_handle->file_handle = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
	gen_error_attach_backtrace_IF_ERRNO(inotify_init1, errno);
	inotify_add_watch(out_handle->file_handle, path, IN_ATTRIB | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO);
	gen_error_attach_backtrace_IF_ERRNO(inotify_add_watch, errno);
#else
	out_handle->file_handle = dup(handle->file_handle);
	gen_error_attach_backtrace_IF_ERRNO(dup, errno);
	out_handle->is_directory = handle->is_directory;
	if(out_handle->is_directory) {
		out_handle->directory_handle = fdopendir(out_handle->file_handle);
		gen_error_attach_backtrace_IF_ERRNO(fdopendir, errno);

		gen_error_t error = gen_directory_list(handle, gen_internal_filewatch_dwn_dircount, &out_handle->internal_directory_length);
		gen_error_attach_backtrace_IF(error, "`gen_directory_list` failed");
	}

	fstat(out_handle->file_handle, &out_handle->internal_descriptor_details);
	gen_error_attach_backtrace_IF_ERRNO(fstat, errno);
#endif

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}

gen_error_t gen_filewatch_poll(gen_filesystem_handle_t* const restrict handle, gen_filewatch_event_t* const restrict out_event) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_filewatch_poll", (void*) gen_filewatch_poll, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(handle);
	GEN_NULL_CHECK(out_event);

	*out_event = GEN_FILEWATCH_NONE;

#if GEN_FILESYSTEM_FILEWATCH_USE_SYSTEM_LIBRARY == ENABLED
	struct pollfd fd = {handle->file_handle, POLLIN, 0};

	fd.revents = 0;
	poll(&fd, 1, 0);
	gen_error_attach_backtrace_IF_ERRNO(poll, errno);

	if(fd.revents & POLLIN) {
		unsigned int events_size;
		ioctl(handle->file_handle, FIONREAD, &events_size);
		gen_error_attach_backtrace_IF_ERRNO(ioctl, errno);

		unsigned char* raw_events = NULL;
		gen_error_t error = gzalloc_aligned((void**) &raw_events, events_size, sizeof(unsigned char), alignof(struct inotify_event));
		gen_error_attach_backtrace_IF(error, "`gzalloc_aligned` failed");

		read(handle->file_handle, raw_events, events_size);
		if(errno != EOK) {
			error = gfree(raw_events);
			gen_error_attach_backtrace_IF(error, "`gfree` failed");

			gen_error_attach_backtrace_ERRNO(read, errno);
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
		gen_error_attach_backtrace_IF(error, "`gfree` failed");
	}

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
#else
	struct stat file_info;
	fstat(handle->file_handle, &file_info);
	gen_error_attach_backtrace_IF_ERRNO(fstat, errno);

#if PLATFORM == DWN
	if(!(file_info.st_mtimespec.tv_sec == handle->internal_descriptor_details.st_mtimespec.tv_sec && file_info.st_mtimespec.tv_nsec == handle->internal_descriptor_details.st_mtimespec.tv_nsec) || !(file_info.st_ctimespec.tv_sec == handle->internal_descriptor_details.st_ctimespec.tv_sec && file_info.st_ctimespec.tv_nsec == handle->internal_descriptor_details.st_ctimespec.tv_nsec)) {
#else
	if(file_info.st_mtime != handle->internal_descriptor_details.st_mtime || file_info.st_ctime != handle->internal_descriptor_details.st_ctime) {
#endif
		if(handle->is_directory) {
			size_t items_length = 0;
			gen_error_t error = gen_directory_list(handle, gen_internal_filewatch_dwn_dircount, &items_length);
			gen_error_attach_backtrace_IF(error, "`gen_directory_list` failed");

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
	gen_error_attach_backtrace_IF_ERRNO(memcpy_s, errno);

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
#endif
}

gen_error_t gen_filewatch_destroy(gen_filesystem_handle_t* const restrict handle) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_filewatch_destroy", (void*) gen_filewatch_destroy, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(handle);

#if PLATFORM == LNX && GEN_FILESYSTEM_FILEWATCH_USE_SYSTEM_LIBRARY == ENABLED
	close(handle->file_handle);
	gen_error_attach_backtrace_IF_ERRNO(close, errno);
#else
	gen_error_t error = gen_filesystem_handle_close(handle);
	gen_error_attach_backtrace_IF(error, "`gen_filesystem_handle_close` failed");
#endif

	return (gen_error_t){GEN_OK, GEN_LINENO, ""};
}
