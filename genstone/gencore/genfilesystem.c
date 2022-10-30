// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genfilesystem.h"

#include "include/genmemory.h"
#include "include/genstring.h"

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <errno.h>

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/uio.h>
#endif

#if GEN_PLATFORM == GEN_LINUX
#include <sys/inotify.h>
#include <sys/resource.h>
#if !defined(GEN_LINUX_ANDROID) || GEN_LINUX_ANDROID >= 30
#include <sys/mman.h>
#endif
#endif
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

GEN_MAYBE_UNUSED static void gen_filesystem_internal_path_canonicalize_cleanup_path(char** path) {
    if(!*path) return;

    gen_error_t* error = gen_memory_free((void**) path);
    if(error) {
        gen_error_print("genfilesystem", error, GEN_ERROR_SEVERITY_FATAL);
        gen_error_abort();
    }
}

GEN_MAYBE_UNUSED static void gen_filesystem_internal_path_canonicalize_cleanup_fd(gen_filesystem_file_handle_t** file_handle) {
	if(!*file_handle) return;

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	if(**file_handle == -1) return;

	int result = close(**file_handle);
	if(result == -1) {
		gen_error_t* error = gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not canonicalize path: %t", gen_error_description_from_errno());
		gen_error_print("genfilesystem", error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
#endif
}

gen_error_t* gen_filesystem_path_canonicalize(const char* const restrict path, const size_t path_length, char* restrict out_canonical, size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_canonicalize, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(!out_canonical && !out_length) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "Both `out_canonical` and `out_length` were `NULL`");
	if(path_length != GEN_STRING_NO_BOUNDS && path[path_length] != '\0') return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	error = gen_filesystem_path_validate(path, path_length);
	if(error) return error;

    gen_filesystem_file_handle_t fd = open(path, O_RDWR);
    if(fd == -1 && errno == EISDIR) {
        fd = open(path, O_RDONLY | O_DIRECTORY);
        if(fd == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not canonicalize path `%tz`: %t", path, path_length, gen_error_description_from_errno());
    }
    else if(fd == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not canonicalize path `%tz`: %t", path, path_length, gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_path_canonicalize_cleanup_fd)
	GEN_UNUSED gen_filesystem_file_handle_t* file_handle_scope_variable = &fd;

    long value = pathconf("/", _PC_PATH_MAX);
    if(value == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not canonicalize path `%tz`: %t", path, path_length, gen_error_description_from_errno());

    char* canonicalized = NULL;
    error = gen_memory_allocate_zeroed((void**) &canonicalized, (size_t) (value + 1), sizeof(char));
    if(error) return error;
    
    GEN_CLEANUP_FUNCTION(gen_filesystem_internal_path_canonicalize_cleanup_path) GEN_UNUSED char* path_cleanup_scope_var = canonicalized;

#if GEN_PLATFORM == GEN_OSX
    int result = fcntl(fd, F_GETPATH, canonicalized);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not canonicalize path `%tz`: %t", path, path_length, gen_error_description_from_errno());
#elif GEN_PLATFORM == GEN_LINUX
    struct rlimit limit = {0};
    int result = getrlimit(RLIMIT_NOFILE, &limit);

    char* fdpath = NULL;
    size_t fdpath_size = sizeof("/proc/self/fd/") + limit.rlim_cur;
    error = gen_memory_allocate_zeroed((void**) &fdpath, fdpath_size, sizeof(char));
    if(error) return error;
    GEN_CLEANUP_FUNCTION(gen_filesystem_internal_path_canonicalize_cleanup_path) GEN_UNUSED char* fdpath_scope_var = NULL;

    error = gen_string_format(fdpath_size, fdpath, NULL, "/proc/self/fd/%si", sizeof("/proc/self/fd/%si") - 1, fd);
    if(error) return error;

    ssize_t readlink_result = readlink(fdpath, canonicalized, (size_t) (value + 1));
    if(readlink_result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not canonicalize path `%tz`: %t", path, path_length, gen_error_description_from_errno());
#endif

	size_t canonicalized_length = 0;
	error = gen_string_length(canonicalized, (size_t) (value + 1), (size_t) value, &canonicalized_length);
	if(error) return error;

	if(out_length) *out_length = canonicalized_length;

	if(out_canonical) {
		error = gen_string_copy(out_canonical, GEN_STRING_NO_BOUNDS, canonicalized, canonicalized_length + 1, canonicalized_length);
		if(error) return error;
	}
#endif

	return NULL;
}

gen_error_t* gen_filesystem_path_exists(const char* const restrict path, const size_t path_length, bool* const restrict out_exists) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_exists, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(!out_exists) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_exists` was `NULL`");
	if(path_length != GEN_STRING_NO_BOUNDS && path[path_length] != '\0') return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

	error = gen_filesystem_path_validate(path, path_length);
	if(error) return error;

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	const int result = access(path, F_OK);
	if(result == -1 && errno != ENOENT) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not check whether path `%tz` exists: %t", path, path_length, gen_error_description_from_errno());

	*out_exists = !result;
#endif

	return NULL;
}

gen_error_t* gen_filesystem_path_validate(const char* const restrict path, const size_t path_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_validate, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(path_length != GEN_STRING_NO_BOUNDS && path[path_length] != '\0') return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

    if(path_length == GEN_STRING_NO_BOUNDS) return NULL;

	if(path_length == 0) return gen_error_attach_backtrace_formatted(GEN_ERROR_TOO_SHORT, GEN_LINE_NUMBER, "Path `%tz` was too short", path, path_length);
#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX
    long value = pathconf("/", _PC_NAME_MAX);
    if(value == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not validate path `%tz`: %t", gen_error_description_from_errno());

	if(path_length > (size_t) value) return gen_error_attach_backtrace_formatted(GEN_ERROR_TOO_LONG, GEN_LINE_NUMBER, "Path `%tz` exceeded the maximum number of characters in a path %uz", path, path_length, value);
#elif GEN_PLATFORM == GEN_WINDOWS
	if(path_length > MAX_PATH) return gen_error_attach_backtrace_formatted(GEN_ERROR_TOO_LONG, GEN_LINE_NUMBER, "Path `%tz` exceeded the maximum number of characters in a path %uz", path, path_length, MAX_PATH);
	// TODO: Windows has path content limitations
#endif


	return NULL;
}

gen_error_t* gen_filesystem_path_create_file(const char* const restrict path, const size_t path_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_create_file, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(path_length != GEN_STRING_NO_BOUNDS && path[path_length] != '\0') return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

	error = gen_filesystem_path_validate(path, path_length);
	if(error) return error;

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	int result = open(path, O_RDWR | O_CREAT, 0777);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file at path `%tz`: %t", path, path_length, gen_error_description_from_errno());
	result = close(result);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file at path `%tz`: %t", path, path_length, gen_error_description_from_errno());
#endif

	return NULL;
}

gen_error_t* gen_filesystem_path_create_directory(const char* const restrict path, const size_t path_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_create_directory, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(path_length != GEN_STRING_NO_BOUNDS && path[path_length] != '\0') return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

	error = gen_filesystem_path_validate(path, path_length);
	if(error) return error;

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	int result = mkdir(path, 0777);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create directory at path `%tz`: %t", path, path_length, gen_error_description_from_errno());
#endif

	return NULL;
}

gen_error_t* gen_filesystem_path_delete(const char* const restrict path, const size_t path_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_delete, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(path_length != GEN_STRING_NO_BOUNDS && path[path_length] != '\0') return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

	error = gen_filesystem_path_validate(path, path_length);
	if(error) return error;

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	int result = rmdir(path);
	if(result == -1 && errno != ENOTDIR) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not remove path `%tz`: %t", path, path_length, gen_error_description_from_errno());

	if(result == -1 && errno == ENOTDIR) {
		result = unlink(path);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not remove path `%tz`: %t", path, path_length, gen_error_description_from_errno());
	}
#endif

	return NULL;
}

GEN_MAYBE_UNUSED static void gen_filesystem_internal_handle_open_cleanup_lock(gen_threads_mutex_t** mutex) {
	if(!*mutex) return;

	gen_error_t* error = gen_threads_mutex_destroy(*mutex);
	if(error) {
		gen_error_print("genfilesystem", error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
}

GEN_MAYBE_UNUSED static void gen_filesystem_internal_handle_open_cleanup_file_handle(gen_filesystem_file_handle_t** file_handle) {
	if(!*file_handle) return;

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	if(**file_handle == -1) return;

	int result = close(**file_handle);
	if(result == -1) {
		gen_error_t* error = gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path: %t", gen_error_description_from_errno());
		gen_error_print("genfilesystem", error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
#endif
}

GEN_MAYBE_UNUSED static void gen_filesystem_internal_handle_open_cleanup_directory_handle(gen_filesystem_directory_handle_t** directory_handle) {
	if(!*directory_handle) return;

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	int result = closedir(**directory_handle);
	if(result == -1) {
		gen_error_t* error = gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path: %t", gen_error_description_from_errno());
		gen_error_print("genfilesystem", error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
#endif
}

gen_error_t* gen_filesystem_handle_open_anonymous(gen_filesystem_handle_t* restrict out_handle) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_open_anonymous, GEN_FILE_NAME);
	if(error) return error;

	if(!out_handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_handle` was `NULL`");

	error = gen_threads_mutex_create(&out_handle->lock);
	if(error) return error;

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_lock)
	gen_threads_mutex_t* lock_scope_variable = &out_handle->lock;

#if GEN_PLATFORM == GEN_LINUX && (!defined(GEN_LINUX_ANDROID) || GEN_LINUX_ANDROID >= 30)
    gen_filesystem_file_handle_t fd = memfd_create("__genstone_anon_file", 0);
	if(fd == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle for an anonymous file: %t", gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_file_handle)
	gen_filesystem_file_handle_t* file_handle_scope_variable = &fd;

    out_handle->type = GEN_FILESYSTEM_HANDLE_ANONYMOUS;
    out_handle->file_handle = fd;

    // Prevent error-cleanup
    file_handle_scope_variable = NULL;
#elif GEN_PLATFORM == GEN_OSX || GEN_PLATFORM == GEN_LINUX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	gen_filesystem_file_handle_t fd = open("__genstone_anon_file", O_CREAT | O_RDWR, 0777);
	if(fd == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle for an anonymous file: %t", gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_file_handle)
	gen_filesystem_file_handle_t* file_handle_scope_variable = &fd;

    int result = unlink("__genstone_anon_file");
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle for an anonymous file: %t", gen_error_description_from_errno());

    out_handle->type = GEN_FILESYSTEM_HANDLE_ANONYMOUS;
    out_handle->file_handle = fd;

    // Prevent error-cleanup
    file_handle_scope_variable = NULL;
#endif
    lock_scope_variable = NULL;

	return NULL;
}

gen_error_t* gen_filesystem_handle_open(const char* const restrict path, const size_t path_length, gen_filesystem_handle_t* restrict out_handle) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_open, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(!out_handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_handle` was `NULL`");
	if(path_length != GEN_STRING_NO_BOUNDS && path[path_length] != '\0') return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

	error = gen_filesystem_path_validate(path, path_length);
	if(error) return error;

	error = gen_threads_mutex_create(&out_handle->lock);
	if(error) return error;

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_lock)
	gen_threads_mutex_t* lock_scope_variable = &out_handle->lock;

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	gen_filesystem_file_handle_t fd = open(path, O_DIRECTORY | O_RDONLY);
	if(fd == -1 && errno != ENOTDIR) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path `%tz`: %t", path, path_length, gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_file_handle)
	gen_filesystem_file_handle_t* file_handle_scope_variable = &fd;

	if(fd == -1 && errno == ENOTDIR) {
		fd = open(path, O_RDWR);
		if(fd == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path `%tz`: %t", path, path_length, gen_error_description_from_errno());

		out_handle->type = GEN_FILESYSTEM_HANDLE_FILE;
		out_handle->file_handle = fd;

		// Prevent error-cleanup
		lock_scope_variable = NULL;
		file_handle_scope_variable = NULL;

		return NULL;
	}

	out_handle->type = GEN_FILESYSTEM_HANDLE_DIRECTORY;
	out_handle->file_handle = fd;
	out_handle->directory_handle = fdopendir(fd);
	if(!out_handle->directory_handle) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path `%tz`: %t", path, path_length, gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_directory_handle)
	gen_filesystem_directory_handle_t* directory_handle_scope_variable = &out_handle->directory_handle;

	// Prevent error-cleanup
	file_handle_scope_variable = NULL;
	directory_handle_scope_variable = NULL;
#endif
	lock_scope_variable = NULL;

	return NULL;
}

gen_error_t* gen_filesystem_handle_close(gen_filesystem_handle_t* const restrict handle) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_close, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	if(handle->type == GEN_FILESYSTEM_HANDLE_DIRECTORY) {
		int result = closedir(handle->directory_handle);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not close filesystem handle: %t", gen_error_description_from_errno());
	}
    else {
        int result = close(handle->file_handle);
        if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not close filesystem handle: %t", gen_error_description_from_errno());
    }

	error = gen_threads_mutex_unlock_and_destroy(&handle->lock);
	if(error) return error;
#endif

	return NULL;
}

gen_error_t* gen_filesystem_handle_file_size(gen_filesystem_handle_t* const restrict handle, size_t* const restrict out_size) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_file_size, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");
	if(!out_size) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_size` was `NULL`");

	if(handle->type != GEN_FILESYSTEM_HANDLE_FILE && handle->type != GEN_FILESYSTEM_HANDLE_ANONYMOUS) return gen_error_attach_backtrace(GEN_ERROR_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a file");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	off_t offset = lseek(handle->file_handle, 0, SEEK_END);
	if(offset == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not get file size: %t", gen_error_description_from_errno());

	*out_size = (size_t) offset;

	offset = lseek(handle->file_handle, 0, SEEK_SET);
	if(offset == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not get file size: %t", gen_error_description_from_errno());
#endif

	return NULL;
}

gen_error_t* gen_filesystem_handle_file_read(gen_filesystem_handle_t* const restrict handle, const size_t start, const size_t end, unsigned char* restrict out_buffer) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_file_read, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");
	if(!out_buffer) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`output_buffer` was `NULL`");
	if(start > end) return gen_error_attach_backtrace(GEN_ERROR_TOO_SHORT, GEN_LINE_NUMBER, "`start` > `end`");

	if(handle->type != GEN_FILESYSTEM_HANDLE_FILE && handle->type != GEN_FILESYSTEM_HANDLE_ANONYMOUS) return gen_error_attach_backtrace(GEN_ERROR_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a file");

    if(start == end) {
        return NULL;
    }

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	if(handle->file_handle <= STDERR_FILENO) {
        ssize_t result = read(handle->file_handle, out_buffer, (size_t) (end - start));
        if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not read file: %t", gen_error_description_from_errno());
    }
    else {
        ssize_t result = pread(handle->file_handle, out_buffer, (size_t) (end - start), (off_t) start);
        if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not read file: %t", gen_error_description_from_errno());
    }
#endif

	return NULL;
}

gen_error_t* gen_filesystem_handle_file_write(gen_filesystem_handle_t* const restrict handle, const unsigned char* const restrict buffer, GEN_MAYBE_UNUSED const size_t offset, const size_t buffer_size) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_file_write, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");
	if(!buffer) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`buffer` was `NULL`");

	if(handle->type != GEN_FILESYSTEM_HANDLE_FILE && handle->type != GEN_FILESYSTEM_HANDLE_ANONYMOUS) return gen_error_attach_backtrace(GEN_ERROR_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a file");

	if(!buffer_size) return NULL;

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	if(handle->file_handle <= STDERR_FILENO) {
		ssize_t result = write(handle->file_handle, buffer, buffer_size);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not write file: %t", gen_error_description_from_errno());
	}
	else {
		ssize_t result = pwrite(handle->file_handle, buffer, buffer_size, (off_t) offset);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not write file: %t", gen_error_description_from_errno());
	}
#endif

	return NULL;
}

GEN_MAYBE_UNUSED static void gen_filesystem_internal_handle_directory_list_entries_cleanup(char** entries) {
	gen_error_t* error = gen_memory_free((void**) entries);
	if(error) {
		gen_error_print("genfilesystem", error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
}

gen_error_t* gen_filesystem_handle_directory_list(gen_filesystem_handle_t* const restrict handle, char* restrict* const restrict out_list, size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_directory_list, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");
	if(!out_list && !out_length) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_list` and `out_length` were `NULL`");

    // TODO: Add secondary type for watchers
#if GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY == GEN_ENABLED
#if GEN_PLATFORM == GEN_LINUX
    if(handle->type != GEN_FILESYSTEM_HANDLE_DIRECTORY) return gen_error_attach_backtrace(GEN_ERROR_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a directory");
#else
    #error No system library available for file watching
#endif
#else
#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
    if(handle->type != GEN_FILESYSTEM_HANDLE_DIRECTORY && !(handle->type == GEN_FILESYSTEM_HANDLE_WATCHER && handle->directory_handle)) return gen_error_attach_backtrace(GEN_ERROR_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a directory");
#endif
#endif

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	rewinddir(handle->directory_handle);

	size_t entries_length = 0;
	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_directory_list_entries_cleanup)
	char* entries = NULL;

	struct dirent* entry = NULL;
	while(true) {
		// This looks hacky but it's actually what the manpage
		// Says is the correct way to check for errors in `readdir`.
		errno = EOK;
		entry = readdir(handle->directory_handle);
		if(!entry && errno)
			return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not list directory: %t", gen_error_description_from_errno());
		else if(!entry)
			break;

		if(entry->d_name[0] == '.' && entry->d_name[1] == '\0') continue;
		if(entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == '\0') continue;

		if(out_list) {
			error = gen_memory_reallocate_zeroed((void**) &entries, entries_length, entries_length + 1, sizeof(char[GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX]));
			if(error) return error;

			error = gen_string_copy(&entries[entries_length * sizeof(char[GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX])], GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX, entry->d_name, GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX, GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX - 1);
			if(error) return error;
		}
		++entries_length;
	}

	if(out_length) {
		*out_length = entries_length;
	}

	if(out_list) {
		for(size_t i = 0; i < entries_length; ++i) {
			error = gen_string_copy(out_list[i], GEN_MEMORY_NO_BOUNDS, &entries[i * sizeof(char[GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX])], GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX, GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX - 1);
			if(error) return error;
		}
	}

	rewinddir(handle->directory_handle);
#endif

	return NULL;
}

gen_error_t* gen_filesystem_handle_lock(gen_filesystem_handle_t* const restrict handle) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_lock, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	error = gen_threads_mutex_lock(&handle->lock);
	if(error) return error;

	int result = flock(handle->file_handle, LOCK_EX);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not lock file: %t", gen_error_description_from_errno());
#endif

	return NULL;
}

gen_error_t* gen_filesystem_handle_unlock(gen_filesystem_handle_t* const restrict handle) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_unlock, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	error = gen_threads_mutex_unlock(&handle->lock);
	if(error) return error;

	int result = flock(handle->file_handle, LOCK_UN);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not unlock file: %t", gen_error_description_from_errno());
#endif

	return NULL;
}

gen_error_t* gen_filesystem_watcher_create(gen_filesystem_handle_t* const restrict handle, gen_filesystem_watcher_t* const restrict out_watcher) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_watcher_create, GEN_FILE_NAME);
	if(error) return error;

	if(!out_watcher) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_watcher` was `NULL`");
	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");
    if(handle->type == GEN_FILESYSTEM_HANDLE_ANONYMOUS) return gen_error_attach_backtrace(GEN_ERROR_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was an anonymous file");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	error = gen_threads_mutex_create(&out_watcher->lock);
	if(error) return error;

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_lock)
	gen_threads_mutex_t* lock_scope_variable = &out_watcher->lock;
#endif

#if GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY == GEN_ENABLED
#if GEN_PLATFORM == GEN_LINUX
	out_watcher->type = GEN_FILESYSTEM_HANDLE_WATCHER;

	static const char format[] = "/proc/self/fd/%si";

	size_t formatted_length = 0;
	error = gen_string_format(GEN_STRING_NO_BOUNDS, NULL, &formatted_length, format, sizeof(format) - 1, handle->file_handle);
	if(error) return error;

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_directory_list_entries_cleanup)
	char* link = NULL;
	error = gen_memory_allocate_zeroed((void**) &link, formatted_length + 1, sizeof(char));
	if(error) return error;

	error = gen_string_format(GEN_STRING_NO_BOUNDS, link, NULL, format, sizeof(format) - 1, handle->file_handle);
	if(error) return error;

	struct stat link_stat = {0};
	int result = lstat(link, &link_stat);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

	size_t path_size = (size_t) link_stat.st_size + 1;
	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_directory_list_entries_cleanup)
	char* path = NULL;
	error = gen_memory_allocate_zeroed((void**) &path, path_size, sizeof(char));
	if(error) return error;

	ssize_t path_length_result = readlink(link, path, path_size);
	if(path_length_result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

	result = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_file_handle)
	gen_filesystem_file_handle_t* file_handle_scope_variable = &result;
	out_watcher->file_handle = result;

	// TODO: Watchers being handle-agnostic and adding watched paths to the watcher?
	//       Would be a more sensible use of inotify/other syslibs.
	result = inotify_add_watch(out_watcher->file_handle, path, IN_ATTRIB | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

	lock_scope_variable = NULL;
	file_handle_scope_variable = NULL;

	return NULL;
#else
#error No system library available for file watching
#endif
#else
#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	out_watcher->type = GEN_FILESYSTEM_HANDLE_WATCHER;

    // TODO: Why is this neccesary?
    GEN_UNUSED volatile int x = handle->file_handle;

	int result = fcntl(handle->file_handle, F_DUPFD);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_file_handle)
	gen_filesystem_file_handle_t* file_handle_scope_variable = &result;
	out_watcher->file_handle = result;

	if(handle->type == GEN_FILESYSTEM_HANDLE_DIRECTORY) {
		out_watcher->directory_handle = fdopendir(out_watcher->file_handle);
		if(!out_watcher->directory_handle) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

		error = gen_filesystem_handle_directory_list(handle, NULL, &out_watcher->internal_directory_length_cached);
		if(error) return error;
	}

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_directory_handle)
	gen_filesystem_directory_handle_t* directory_handle_scope_variable = &out_watcher->directory_handle;

	result = fstat(out_watcher->file_handle, &out_watcher->internal_stat_cached);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

	lock_scope_variable = NULL;
	file_handle_scope_variable = NULL;
	directory_handle_scope_variable = NULL;

#endif
#endif

	return NULL;
}

gen_error_t* gen_filesystem_watcher_destroy(gen_filesystem_watcher_t* const restrict watcher) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_watcher_destroy, GEN_FILE_NAME);
	if(error) return error;

	if(!watcher) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`watcher` was `NULL`");
	if(watcher->type != GEN_FILESYSTEM_HANDLE_WATCHER) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`watcher` was not a watcher");

	watcher->type = watcher->directory_handle ? GEN_FILESYSTEM_HANDLE_DIRECTORY : GEN_FILESYSTEM_HANDLE_FILE;

	error = gen_filesystem_handle_close(watcher);
	if(error) return error;

	return NULL;
	/*
#if GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY == GEN_ENABLED
	#if GEN_PLATFORM == GEN_LINUX
	#else
		#error No system library available for file watching
	#endif
#else
*/
}

#if GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY == GEN_ENABLED && GEN_PLATFORM == GEN_LINUX
static void gen_filesystem_internal_watcher_poll_raw_events_cleanup(unsigned char** raw_events) {
	gen_error_t* error = gen_memory_free((void**) raw_events);
	if(error) {
		gen_error_print("genfilesystem", error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
}
#endif

gen_error_t* gen_filesystem_watcher_poll(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_watcher_event_t* const restrict out_event) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_watcher_poll, GEN_FILE_NAME);
	if(error) return error;

	if(!watcher) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`watcher` was `NULL`");
	if(!out_event) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_event` was `NULL`");

	*out_event = GEN_FILESYSTEM_WATCHER_EVENT_NONE;

#if GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY == GEN_ENABLED
#if GEN_PLATFORM == GEN_LINUX
	struct pollfd fd = {watcher->file_handle, POLLIN, 0};
	int result = poll(&fd, 1, 0);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not poll for file watcher events: %t", gen_error_description_from_errno());

	if(fd.revents & POLLIN) {
		unsigned int events_size = 0;
		result = ioctl(watcher->file_handle, FIONREAD, &events_size);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not poll for file watcher events: %t", gen_error_description_from_errno());

		GEN_CLEANUP_FUNCTION(gen_filesystem_internal_watcher_poll_raw_events_cleanup)
		alignas(alignof(struct inotify_event*)) unsigned char* raw_events = NULL;
		error = gen_memory_allocate_zeroed_aligned((void**) &raw_events, events_size, sizeof(unsigned char), alignof(struct inotify_event));
		if(error) return error;

		ssize_t events_size_result = read(watcher->file_handle, raw_events, events_size);
		if(events_size_result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not poll for file watcher events: %t", gen_error_description_from_errno());

		unsigned int offset = 0;
		while(offset < events_size) {
#ifdef __ANALYZER
			struct inotify_event const e = {0};
			struct inotify_event* const event = &e;
#else
			struct inotify_event* const event = (struct inotify_event*) &raw_events[offset];
#endif

			if(event->mask & IN_ATTRIB || event->mask & IN_MODIFY) *out_event |= GEN_FILESYSTEM_WATCHER_EVENT_MODIFIED;
			if(event->mask & IN_CREATE) *out_event |= GEN_FILESYSTEM_WATCHER_EVENT_CREATED;
			if(event->mask & IN_DELETE || event->mask & IN_DELETE_SELF) *out_event |= GEN_FILESYSTEM_WATCHER_EVENT_DELETED;
			if(event->mask & IN_MOVE_SELF || event->mask & IN_MOVED_FROM || event->mask & IN_MOVED_TO) *out_event |= GEN_FILESYSTEM_WATCHER_EVENT_MOVED;

			offset += sizeof(struct inotify_event) + event->len;
		}
	}

	return NULL;
#else
#error No system library available for file watching
#endif
#else
#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FILESYSTEM_FORCE_UNIX == GEN_ENABLED
	struct stat watcher_stat = {0};
	int result = fstat(watcher->file_handle, &watcher_stat);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not poll for file watcher events: %t", gen_error_description_from_errno());

#if GEN_PLATFORM == GEN_OSX
	if(    watcher_stat.st_mtimespec.tv_sec  != watcher->internal_stat_cached.st_mtimespec.tv_sec
        || watcher_stat.st_mtimespec.tv_nsec != watcher->internal_stat_cached.st_mtimespec.tv_nsec
        || watcher_stat.st_ctimespec.tv_sec  != watcher->internal_stat_cached.st_ctimespec.tv_sec
        || watcher_stat.st_ctimespec.tv_nsec != watcher->internal_stat_cached.st_ctimespec.tv_nsec) {
#else
	if(    watcher_stat.st_mtime != watcher->internal_stat_cached.st_mtime
        || watcher_stat.st_ctime != watcher->internal_stat_cached.st_ctime) {
#endif
		if(watcher->directory_handle) {
			size_t items_length = 0;
			error = gen_filesystem_handle_directory_list(watcher, NULL, &items_length);
			if(error) return error;

			if(watcher->internal_directory_length_cached > items_length)
				*out_event |= GEN_FILESYSTEM_WATCHER_EVENT_DELETED;
			else if(watcher->internal_directory_length_cached < items_length)
				*out_event |= GEN_FILESYSTEM_WATCHER_EVENT_CREATED;
			else
				*out_event |= GEN_FILESYSTEM_WATCHER_EVENT_MODIFIED;

			watcher->internal_directory_length_cached = items_length;
		}
		else {
			*out_event |= GEN_FILESYSTEM_WATCHER_EVENT_MODIFIED;
		}
	}

	watcher->internal_stat_cached = watcher_stat;

#endif
#endif
	return NULL;
}
