// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genfilesystem.h"

#include "include/genstring.h"
#include "include/genmemory.h"

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <stdlib.h> // TODO: For realpath
#include <errno.h>

#if GEN_PLATFORM != GEN_WINDOWS
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/file.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#if GEN_PLATFORM == GEN_LINUX
#include <sys/inotify.h>
#endif
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

void gen_filesystem_internal_scoped_file_lock_cleanup(gen_filesystem_handle_t* const * handle) {
	gen_error_t error = gen_filesystem_handle_unlock(*handle);
	if(error.type) {
        gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
        gen_error_abort();
	}
}

gen_error_t gen_filesystem_path_canonicalize(const char* const restrict path, const size_t path_length, char* restrict out_canonical, size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_canonicalize, GEN_FILE_NAME);
	if(error.type) return error;

	if(!path) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(!out_canonical && !out_length) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "Both `out_canonical` and `out_length` were `NULL`");
	if(path[path_length] != '\0') return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

	error = gen_filesystem_path_validate(path, path_length);
	if(error.type) return error;

	const char* canonicalized = realpath(path, NULL); // TODO: Replace with in-house implementation
												//       Maybe use `fcntl`'s `F_GETPATH` as described in the Darwin manpage?
	if(!canonicalized) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Failed to resolve canonical representation for path `%zs`: %s", path, path_length, gen_error_description_from_errno());

	size_t canonicalized_length = 0;
	error = gen_string_length(canonicalized, GEN_STRING_NO_BOUNDS, GEN_STRING_NO_BOUNDS, &canonicalized_length);
	if(error.type) return error;

	if(out_length) *out_length = canonicalized_length;

	if(out_canonical) {
		error = gen_string_copy(out_canonical, GEN_STRING_NO_BOUNDS, canonicalized, canonicalized_length + 1, canonicalized_length);
		if(error.type) return error;
	}

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_filesystem_path_exists(const char* const restrict path, const size_t path_length, bool* const restrict out_exists) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_exists, GEN_FILE_NAME);
	if(error.type) return error;

	if(!path) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(!out_exists) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_exists` was `NULL`");
	if(path[path_length] != '\0') return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

	error = gen_filesystem_path_validate(path, path_length);
	if(error.type) return error;

	const int result = access(path, F_OK);
	if(result == -1 && errno != ENOENT) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not check whether path `%zs` exists: %s", path, path_length, gen_error_description_from_errno());

	*out_exists = !result;

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_filesystem_path_validate(const char* const restrict path, const size_t path_length) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_validate, GEN_FILE_NAME);
	if(error.type) return error;

	if(!path) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(path[path_length] != '\0') return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

	if(path_length == 0) return gen_error_attach_backtrace_formatted(GEN_TOO_SHORT, GEN_LINE_NUMBER, "Path `%zs` was too short", path, path_length);
	if(path_length > PATH_MAX /* TODO: This number is usually bogus - Use `pathconf` instead */) return gen_error_attach_backtrace_formatted(GEN_TOO_LONG, GEN_LINE_NUMBER, "Path `%zs` was too long", path, path_length);

	// TODO: Windows has path content limitations

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_filesystem_path_create_file(const char* const restrict path, const size_t path_length) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_create_file, GEN_FILE_NAME);
	if(error.type) return error;

	if(!path) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(path[path_length] != '\0') return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

	error = gen_filesystem_path_validate(path, path_length);
	if(error.type) return error;

	int result = open(path, O_RDWR | O_CREAT | O_CLOEXEC, 0777);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file at path `%zs`: %s", path, path_length, gen_error_description_from_errno());
	result = close(result);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file at path `%zs`: %s", path, path_length, gen_error_description_from_errno());

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_filesystem_path_create_directory(const char* const restrict path, const size_t path_length) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_create_directory, GEN_FILE_NAME);
	if(error.type) return error;

	if(!path) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(path[path_length] != '\0') return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

	error = gen_filesystem_path_validate(path, path_length);
	if(error.type) return error;

	int result = mkdir(path, 0777);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create directory at path `%zs`: %s", path, path_length, gen_error_description_from_errno());

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_filesystem_path_delete(const char* const restrict path, const size_t path_length) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_delete, GEN_FILE_NAME);
	if(error.type) return error;

	if(!path) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(path[path_length] != '\0') return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

	error = gen_filesystem_path_validate(path, path_length);
	if(error.type) return error;

	int result = rmdir(path);
	if(result == -1 && errno != ENOTDIR) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not remove path `%zs`: %s", path, path_length, gen_error_description_from_errno());

	if(result == -1 && errno == ENOTDIR) {
		result = unlink(path);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not remove path `%zs`: %s", path, path_length, gen_error_description_from_errno());
	}

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

static void gen_filesystem_internal_handle_open_cleanup_lock(gen_threads_mutex_t* const restrict * const restrict * const restrict mutex) {
	if(!*mutex) return;

	gen_error_t error = gen_threads_mutex_unlock_and_destroy(**mutex);
	if(error.type) {
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
}

static void gen_filesystem_internal_handle_open_cleanup_file_handle(gen_filesystem_file_handle_t** file_handle) {
	if(!*file_handle) return;
	if(**file_handle == -1) return;

	int result = close(**file_handle);
	if(result == -1) {
		gen_error_t error = gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path: %s", path, path_length, gen_error_description_from_errno());
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
}

static void gen_filesystem_internal_handle_open_cleanup_directory_handle(gen_filesystem_directory_handle_t** directory_handle) {
	if(!*directory_handle) return;

	int result = closedir(**directory_handle);
	if(result == -1) {
		gen_error_t error = gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path: %s", path, path_length, gen_error_description_from_errno());
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
}

gen_error_t gen_filesystem_handle_open(const char* const restrict path, const size_t path_length, gen_filesystem_handle_t* restrict out_handle) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_open, GEN_FILE_NAME);
	if(error.type) return error;

	if(!path) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `NULL`");
	if(!out_handle) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_handle` was `NULL`");
	if(path[path_length] != '\0') return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was not NULL-terminated");

	error = gen_filesystem_path_validate(path, path_length);
	if(error.type) return error;

	error = gen_threads_mutex_create(&out_handle->lock);
	if(error.type) return error;

	error = gen_threads_mutex_lock(&out_handle->lock);
	if(error.type) {
		gen_error_t free_error = gen_threads_mutex_destroy(&out_handle->lock);
		if(free_error.type) return free_error;

		return error;
	}

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_lock) gen_threads_mutex_t** lock_scope_variable = &out_handle->lock;

	int result = open(path, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
	if(result == -1 && errno != ENOTDIR) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path `%zs`: %s", path, path_length, gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_file_handle) gen_filesystem_file_handle_t* file_handle_scope_variable = &result;

	if(result == -1 && errno == ENOTDIR) {
		result = open(path, O_RDWR | O_CLOEXEC);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path `%zs`: %s", path, path_length, gen_error_description_from_errno());

		out_handle->type = GEN_FILESYSTEM_HANDLE_FILE;
		out_handle->file_handle = result;

		lock_scope_variable = NULL; // Prevent error-cleanup

		error = gen_threads_mutex_unlock(&out_handle->lock);
		if(error.type) {
			gen_error_t free_error = gen_threads_mutex_destroy(&out_handle->lock);
			if(free_error.type) return free_error;

			return error;
		}

		file_handle_scope_variable = NULL; // Prevent error-cleanup

		return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
	}

	out_handle->type = GEN_FILESYSTEM_HANDLE_DIRECTORY;
	out_handle->file_handle = result;
	out_handle->directory_handle = fdopendir(result);
	if(!out_handle->directory_handle) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path `%zs`: %s", path, path_length, gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_directory_handle) gen_filesystem_directory_handle_t* directory_handle_scope_variable = &out_handle->directory_handle;

	lock_scope_variable = NULL;

	error = gen_threads_mutex_unlock(&out_handle->lock);
	if(error.type) {
		gen_error_t free_error = gen_threads_mutex_destroy(&out_handle->lock);
		if(free_error.type) return free_error;

		return error;
	}

	file_handle_scope_variable = NULL;
	directory_handle_scope_variable = NULL;

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_filesystem_handle_close(gen_filesystem_handle_t* const restrict handle) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_close, GEN_FILE_NAME);
	if(error.type) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");

	error = GEN_THREADS_MUTEX_SCOPED_LOCK(&handle->lock);
	if(error.type) return error;

	if(handle->type == GEN_FILESYSTEM_HANDLE_DIRECTORY) {
		int result = closedir(handle->directory_handle);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not close filesystem handle: %s", gen_error_description_from_errno());
	}
	else {
		int result = close(handle->file_handle);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not close filesystem handle: %s", gen_error_description_from_errno());
	}

	// TODO: Expose this in a nicer API?
	gen_threads_internal_mutex_scoped_lock_scope_variable = NULL;

	error = gen_threads_mutex_unlock_and_destroy(&handle->lock);
	if(error.type) return error;

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_filesystem_handle_file_size(gen_filesystem_handle_t* const restrict handle, size_t* const restrict out_size) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_file_size, GEN_FILE_NAME);
	if(error.type) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");
	if(!out_size) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_size` was `NULL`");

	if(handle->type != GEN_FILESYSTEM_HANDLE_FILE) return gen_error_attach_backtrace(GEN_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a file");

	error = GEN_THREADS_MUTEX_SCOPED_LOCK(&handle->lock);
	if(error.type) return error;

	off_t offset = lseek(handle->file_handle, 0, SEEK_END);
	if(offset == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not get file size: %s", gen_error_description_from_errno());

	*out_size = (size_t) offset;

	offset = lseek(handle->file_handle, 0, SEEK_SET);
	if(offset == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not get file size: %s", gen_error_description_from_errno());

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_filesystem_handle_file_read(gen_filesystem_handle_t* const restrict handle, const size_t start, const size_t end, unsigned char* restrict out_buffer) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_file_read, GEN_FILE_NAME);
	if(error.type) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");
	if(!out_buffer) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`output_buffer` was `NULL`");
	if(start > end) return gen_error_attach_backtrace(GEN_TOO_SHORT, GEN_LINE_NUMBER, "`start` > `end`");

	if(handle->type != GEN_FILESYSTEM_HANDLE_FILE) return gen_error_attach_backtrace(GEN_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a file");

	error = GEN_THREADS_MUTEX_SCOPED_LOCK(&handle->lock);
	if(error.type) return error;

	if(start == end) {
		off_t offset = lseek(handle->file_handle, 0, SEEK_SET);
		if(offset == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not read file: %s", gen_error_description_from_errno());

		return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
	}

	ssize_t result = pread(handle->file_handle, out_buffer, (size_t) (end - start), (off_t) start);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not read file: %s", gen_error_description_from_errno());

	off_t offset = lseek(handle->file_handle, 0, SEEK_SET);
	if(offset == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not read file: %s", gen_error_description_from_errno());

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_filesystem_handle_file_write(gen_filesystem_handle_t* const restrict handle, const unsigned char* const restrict buffer, const size_t buffer_size) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_file_write, GEN_FILE_NAME);
	if(error.type) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");
	if(!buffer) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`buffer` was `NULL`");

	if(handle->type != GEN_FILESYSTEM_HANDLE_FILE) return gen_error_attach_backtrace(GEN_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a file");

	if(!buffer_size) return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};

	error = GEN_THREADS_MUTEX_SCOPED_LOCK(&handle->lock);
	if(error.type) return error;

	ssize_t result = pwrite(handle->file_handle, buffer, buffer_size, 0);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not write file: %s", gen_error_description_from_errno());

	offset = lseek(handle->file_handle, 0, SEEK_SET);
	if(offset == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not write file: %s", gen_error_description_from_errno());

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

typedef struct {
	char*** entries;
	size_t* length;
} gen_filesystem_internal_handle_directory_list_entries_cleanup_context_t;

static void gen_filesystem_internal_handle_directory_list_entries_cleanup(gen_filesystem_internal_handle_directory_list_entries_cleanup_context_t* context) {
	char** entries = *context->entries;
	for(size_t i = 0; i < context->length; ++i) {
		if(!entries[i]) continue;
		gen_error_t error = gen_memory_free((void**) &entries[i]);
		if(error.type) {
			gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
			gen_error_abort();
		}
	}

	gen_error_t error = gen_memory_free((void**) context->entries);
	if(error.type) {
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
}

gen_error_t gen_filesystem_handle_directory_list(gen_filesystem_handle_t* const restrict handle, char* restrict * const restrict out_list, size_t* const restrict out_lengths, size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_directory_list, GEN_FILE_NAME);
	if(error.type) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");
	if(!out_list && !out_lengths && !out_length) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_list`, `out_lengths` and `out_length` were `NULL`");

	if(handle->type != GEN_FILESYSTEM_HANDLE_DIRECTORY) return gen_error_attach_backtrace(GEN_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a directory");

	error = GEN_THREADS_MUTEX_SCOPED_LOCK(&handle->lock);
	if(error.type) return error;

	rewinddir(handle->directory_handle);

	const long name_max = fpathconf(handle->file_handle, _PC_NAME_MAX);
	if(name_max == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not list directory: %s", gen_error_description_from_errno());

	size_t entries_length = 0;
	GEN_CLEANUP_FUNCTION(gen_memory_free) size_t* entries_lengths = NULL;
	char** entries = NULL;
	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_directory_list_entries_cleanup) gen_filesystem_internal_handle_directory_list_entries_cleanup_context_t = {&entries, &entries_length};

	GEN_CLEANUP_FUNCTION(gen_memory_free) unsigned char* buffer = NULL;
	const size_t entry_size = offsetof(struct dirent, d_name) + name_max + 1;
	error = gen_memory_allocate_zeroed_aligned((void**) &buffer, entry_size, sizeof(unsigned char), GEN_ALIGNOF(struct dirent));
	if(error.type) return error;

	struct dirent* entry = NULL;
	while(true) {
		int result = readdir_r(handle->directory_handle, (struct dirent*) buffer, &entry);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not list directory: %s", gen_error_description_from_errno());

		if(!entry) break;

		if(entry->d_name[0] == '.' && entry->d_name[1] == '\0') continue;
		if(entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == '\0') continue;

		size_t length = 0;
		error = gen_string_length(entry->d_name, name_max + 1, GEN_STRING_NO_BOUNDS, &length);
		if(error.type) return error;

		if(out_list || out_lengths) {
			error = gen_memory_reallocate_zeroed((void**) &entries_lengths, entries_length, entries_length + 1, sizeof(size_t));
			if(error.type) return error;

			entries_lengths[entries_length] = length;
		}
		if(out_list) {
			error = gen_memory_reallocate_zeroed((void**) &entries, entries_length, entries_length + 1, sizeof(char*));
			if(error.type) return error;

			error = gen_memory_allocate_zeroed((void**) &entries[entries_length], length + 1, sizeof(char));
			if(error.type) return error;

			error = gen_string_copy(entries[entries_length], length + 1, entry->d_name, name_max + 1, length);
			if(error.type) return error;
		}
		++entries_length;
	}

	*out_length = entries_length;

	if(out_list) {
		for(size_t i = 0; i < entries_length; ++i) {
			error = gen_string_copy(out_list[i], GEN_MEMORY_NO_BOUNDS, entries[i], entries_lengths[i] + 1, entries_lengths[i]);
			if(error.type) return error;

			error = gen_memory_free((void**) &entries[i]);
			if(error.type) return error;
		}
	}

	if(out_lengths) {
		error = gen_memory_copy(out_lengths, GEN_MEMORY_NO_BOUNDS, entries_lengths, entries_length, entries_length);
		if(error.type) return error;
	}

	rewinddir(handle->directory_handle);

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_filesystem_watcher_create(const gen_filesystem_handle_t* const restrict handle, gen_filesystem_watcher_t* const restrict out_watcher) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_watcher_create, GEN_FILE_NAME);
	if(error.type) return error;

	if(!out_watcher) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_watcher` was `NULL`");
	if(!handle) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");

	error = GEN_THREADS_MUTEX_SCOPED_LOCK(&handle->lock);
	if(error.type) return error;

#if GEN_FILESYSTEM_FILEWATCH_USE_SYSTEM_LIBRARY == ENABLED
	#if GEN_PLATFORM == GEN_LINUX
		out_watcher->type = GEN_FILESYSTEM_HANDLE_DIRECTORY;

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
		#error No system library available
	#endif
#else
	out_handle->file_handle = dup(handle->file_handle);
	gen_error_attach_backtrace_IF_ERRNO(dup, errno);
	out_handle->is_directory = handle->is_directory;
	if(out_handle->is_directory) {
		out_handle->directory_handle = fdopendir(out_handle->file_handle);
		gen_error_attach_backtrace_IF_ERRNO(fdopendir, errno);

		gen_error_t error = gen_directory_list(handle, gen_internal_filewatch_dwn_dircount, &out_handle->internal_directory_length);
		if(error.type) return error;
	}

	fstat(out_handle->file_handle, &out_handle->internal_descriptor_details);
	gen_error_attach_backtrace_IF_ERRNO(fstat, errno);
#endif

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_filewatch_poll(gen_filesystem_handle_t* const restrict handle, gen_filewatch_event_t* const restrict out_event) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filewatch_poll, GEN_FILE_NAME);
	if(error.type) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");
	if(!out_event) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_event` was `NULL`");

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
		if(error.type) return error;

		read(handle->file_handle, raw_events, events_size);
		if(errno != EOK) {
			error = gfree(raw_events);
			if(error.type) return error;

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
		if(error.type) return error;
	}

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
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
			if(error.type) return error;

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

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
#endif
}

gen_error_t gen_filewatch_destroy(gen_filesystem_handle_t* const restrict handle) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filewatch_destroy, GEN_FILE_NAME);
	if(error.type) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `NULL`");

#if PLATFORM == LNX && GEN_FILESYSTEM_FILEWATCH_USE_SYSTEM_LIBRARY == ENABLED
	close(handle->file_handle);
	gen_error_attach_backtrace_IF_ERRNO(close, errno);
#else
	gen_error_t error = gen_filesystem_handle_close(handle);
	if(error.type) return error;
#endif

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}
