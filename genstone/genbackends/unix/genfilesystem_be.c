// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genfilesystem.h>
#include <genstring.h>
#include <genmemory.h>

#include <genfilesystem_be.h>
#include "include/genfilesystem_be.h"

#include <genbackendslibcerror.h>
#include <genbackendsunix.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <dirent.h>
#include <unistd.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

gen_filesystem_handle_t gen_filesystem_standard_input = {GEN_FILESYSTEM_HANDLE_ANONYMOUS, {0}, &(gen_backends_filesystem_handle_t) {GEN_NULL, STDIN_FILENO}};
gen_filesystem_handle_t gen_filesystem_standard_output = {GEN_FILESYSTEM_HANDLE_ANONYMOUS, {0}, &(gen_backends_filesystem_handle_t) {GEN_NULL, STDOUT_FILENO}};
gen_filesystem_handle_t gen_filesystem_standard_error = {GEN_FILESYSTEM_HANDLE_ANONYMOUS, {0}, &(gen_backends_filesystem_handle_t) {GEN_NULL, STDERR_FILENO}};

GEN_INITIALIZER static void gen_backends_filesystem_internal_setup_standard_streams(void) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_filesystem_internal_setup_standard_streams, GEN_FILE_NAME);
    if(error) gen_error_abort_with_error(error, "genfilesystem");

    error = gen_threads_mutex_create(&gen_filesystem_standard_input.lock);
    if(error) gen_error_abort_with_error(error, "genfilesystem");

    error = gen_threads_mutex_create(&gen_filesystem_standard_output.lock);
    if(error) gen_error_abort_with_error(error, "genfilesystem");

    error = gen_threads_mutex_create(&gen_filesystem_standard_error.lock);
    if(error) gen_error_abort_with_error(error, "genfilesystem");
}

static void gen_backends_filesystem_internal_cleanup_path(char** path) {
    if(!*path) return;

    gen_error_t* error = gen_memory_free((void**) path);
	if(error) gen_error_abort_with_error(error, "genfilesystem");
}

static void gen_backends_filesystem_internal_cleanup_fd(int** fd) {
	if(!*fd) return;
	if(**fd == -1) return;

	int result = close(**fd);
	if(result == -1) gen_error_abort_with_error(gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not close fd `%si`: %t", **fd, gen_error_description_from_errno()), "genfilesystem");
}

extern gen_error_t* gen_backends_unix_filesystem_path_canonicalize(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length, char* const restrict out_canonical, gen_size_t* const restrict out_length);
gen_error_t* gen_backends_unix_filesystem_path_canonicalize(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length, char* const restrict out_canonical, gen_size_t* const restrict out_length) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_path_canonicalize, GEN_FILE_NAME);
    if(error) return error;

    

    int fd = open(path, O_RDONLY | O_DIRECTORY);
    if(fd == -1 && errno == ENOTDIR) {
        fd = open(path, O_RDONLY);
        if(fd == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not canonicalize path `%tz`: %t", path, path_length, gen_error_description_from_errno());
    }
    else if(fd == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not canonicalize path `%tz`: %t", path, path_length, gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_backends_filesystem_internal_cleanup_fd) GEN_UNUSED int* fd_scope_variable = &fd;

    long value = pathconf("/", _PC_PATH_MAX);
    if(value == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not canonicalize path `%tz`: %t", path, path_length, gen_error_description_from_errno());

    char* canonicalized = GEN_NULL;
    gen_size_t canonicalized_bounds = (gen_size_t) (value + 1);
    error = gen_memory_allocate_zeroed((void**) &canonicalized, canonicalized_bounds, sizeof(char));
    if(error) return error;
    
    GEN_CLEANUP_FUNCTION(gen_backends_filesystem_internal_cleanup_path) GEN_UNUSED char* path_cleanup_scope_var = canonicalized;

    error = gen_backends_unix_filesystem_internal_path_from_fd(fd, canonicalized, (gen_size_t) value + 1);
    if(error) return error;

	gen_size_t canonicalized_length = 0;
	error = gen_string_length(canonicalized, canonicalized_bounds, GEN_STRING_NO_BOUNDS, &canonicalized_length);
	if(error) return error;

	if(out_length) *out_length = canonicalized_length;

	if(out_canonical) {
		error = gen_string_copy(out_canonical, GEN_STRING_NO_BOUNDS, canonicalized, canonicalized_length + 1, canonicalized_length, canonicalized_length);
		if(error) return error;
	}

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_path_exists(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length, gen_bool_t* const restrict out_exists);
gen_error_t* gen_backends_unix_filesystem_path_exists(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length, gen_bool_t* const restrict out_exists) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_path_exists, GEN_FILE_NAME);
    if(error) return error;

    

	const int result = access(path, F_OK);
	if(result == -1 && errno != ENOENT) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not check whether path `%tz` exists: %t", path, path_length, gen_error_description_from_errno());

	*out_exists = !result;

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_path_validate(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length, gen_bool_t* const restrict out_valid);
gen_error_t* gen_backends_unix_filesystem_path_validate(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length, gen_bool_t* const restrict out_valid) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_path_validate, GEN_FILE_NAME);
    if(error) return error;

    

	if(path_length == 0) return gen_error_attach_backtrace_formatted(GEN_ERROR_TOO_SHORT, GEN_LINE_NUMBER, "Path `%tz` was too short", path, path_length);

    long value = pathconf("/", _PC_NAME_MAX);
    if(value == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not validate path `%tz`: %t", gen_error_description_from_errno());

    *out_valid = gen_false;

	if(path_length > (gen_size_t) value) return GEN_NULL;

    *out_valid = gen_true;
    
    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_path_create_file(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length);
gen_error_t* gen_backends_unix_filesystem_path_create_file(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_path_create_file, GEN_FILE_NAME);
    if(error) return error;

    

	int result = open(path, O_RDWR | O_CREAT, 0777);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file at path `%tz`: %t", path, path_length, gen_error_description_from_errno());

	result = close(result);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file at path `%tz`: %t", path, path_length, gen_error_description_from_errno());

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_path_create_directory(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length);
gen_error_t* gen_backends_unix_filesystem_path_create_directory(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_path_create_directory, GEN_FILE_NAME);
    if(error) return error;

    

	int result = mkdir(path, 0777);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create directory at path `%tz`: %t", path, path_length, gen_error_description_from_errno());

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_path_delete(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length);
gen_error_t* gen_backends_unix_filesystem_path_delete(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_path_delete, GEN_FILE_NAME);
    if(error) return error;

    

	int result = rmdir(path);
	if(result == -1 && errno == ENOTDIR) {
		result = unlink(path);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not remove path `%tz`: %t", path, path_length, gen_error_description_from_errno());
	}
	else if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not remove path `%tz`: %t", path, path_length, gen_error_description_from_errno());

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_handle_open_anonymous(gen_filesystem_handle_t* const restrict out_handle);
gen_error_t* gen_backends_unix_filesystem_handle_open_anonymous(gen_filesystem_handle_t* const restrict out_handle) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_handle_open_anonymous, GEN_FILE_NAME);
    if(error) return error;

    

	int fd = open("__genstone_anon_file", O_CREAT | O_RDWR, 0777);
	if(fd == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle for an anonymous file: %t", gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_backends_filesystem_internal_cleanup_fd) int* fd_scope_variable = &fd;

    int result = unlink("__genstone_anon_file");
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle for an anonymous file: %t", gen_error_description_from_errno());

    out_handle->type = GEN_FILESYSTEM_HANDLE_ANONYMOUS;
    out_handle->native->file = fd;

    fd_scope_variable = GEN_NULL;

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_handle_open(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length, gen_filesystem_handle_t* const restrict out_handle);
gen_error_t* gen_backends_unix_filesystem_handle_open(const char* const restrict path, GEN_UNUSED const gen_size_t path_bounds, const gen_size_t path_length, gen_filesystem_handle_t* const restrict out_handle) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_handle_open, GEN_FILE_NAME);
    if(error) return error;

    

    out_handle->type = GEN_FILESYSTEM_HANDLE_DIRECTORY;

	int fd = open(path, O_DIRECTORY | O_RDONLY);
    if(fd == -1 && errno == ENOTDIR) {
		out_handle->type = GEN_FILESYSTEM_HANDLE_FILE;

		fd = open(path, O_RDWR);
    	if(fd == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path `%tz`: %t", path, path_length, gen_error_description_from_errno());
    }
	else if(fd == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path `%tz`: %t", path, path_length, gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_backends_filesystem_internal_cleanup_fd) int* fd_scope_variable = &fd;

    out_handle->native->file = fd;

    if(out_handle->type == GEN_FILESYSTEM_HANDLE_DIRECTORY) {
        DIR* handle = fdopendir(fd);
        if(!handle) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not open a handle to path `%tz`: %t", path, path_length, gen_error_description_from_errno());

        out_handle->native->directory = handle;
    }

	fd_scope_variable = GEN_NULL;

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_handle_close(gen_filesystem_handle_t* const restrict handle);
gen_error_t* gen_backends_unix_filesystem_handle_close(gen_filesystem_handle_t* const restrict handle) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_handle_close, GEN_FILE_NAME);
    if(error) return error;

    

	if(handle->type == GEN_FILESYSTEM_HANDLE_DIRECTORY) {
		int result = closedir(handle->native->directory);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not close filesystem handle: %t", gen_error_description_from_errno());
	}
    else if(handle->type == GEN_FILESYSTEM_HANDLE_FILE) {
        int result = close(handle->native->file);
        if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not close filesystem handle: %t", gen_error_description_from_errno());
    }

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_handle_file_size(gen_filesystem_handle_t* const restrict handle, gen_size_t* const restrict out_size);
gen_error_t* gen_backends_unix_filesystem_handle_file_size(gen_filesystem_handle_t* const restrict handle, gen_size_t* const restrict out_size) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_handle_file_size, GEN_FILE_NAME);
    if(error) return error;

    

    if(handle->native->file <= STDERR_FILENO) {
        *out_size = 0;
        return GEN_NULL;
    }

	off_t offset = lseek(handle->native->file, 0, SEEK_END);
	if(offset == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not get file size: %t", gen_error_description_from_errno());

	*out_size = (gen_size_t) offset;

	offset = lseek(handle->native->file, 0, SEEK_SET);
	if(offset == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not get file size: %t", gen_error_description_from_errno());

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_handle_file_read(gen_filesystem_handle_t* const restrict handle, const gen_size_t offset, const gen_size_t length, unsigned char* restrict out_buffer);
gen_error_t* gen_backends_unix_filesystem_handle_file_read(gen_filesystem_handle_t* const restrict handle, const gen_size_t offset, const gen_size_t length, unsigned char* restrict out_buffer) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_handle_file_read, GEN_FILE_NAME);
    if(error) return error;

    // `seek`/`pread` are not applicable to standard streams.
	if(handle->native->file <= STDERR_FILENO) {
        gen_ssize_t result = read(handle->native->file, out_buffer, length);
        if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not read file: %t", gen_error_description_from_errno());
    }
    else {
        gen_ssize_t result = pread(handle->native->file, out_buffer, length, (off_t) offset);
        if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not read file: %t", gen_error_description_from_errno());
    }

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_handle_file_write(gen_filesystem_handle_t* const restrict handle, const gen_size_t offset, const unsigned char* const restrict buffer, const gen_size_t length);
gen_error_t* gen_backends_unix_filesystem_handle_file_write(gen_filesystem_handle_t* const restrict handle, const gen_size_t offset, const unsigned char* const restrict buffer, const gen_size_t length) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_handle_file_write, GEN_FILE_NAME);
    if(error) return error;

    

	if(handle->native->file <= STDERR_FILENO) {
		gen_ssize_t result = write(handle->native->file, buffer, length);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not write file: %t", gen_error_description_from_errno());
	}
	else {
		gen_ssize_t result = pwrite(handle->native->file, buffer, length, (off_t) offset);
		if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not write file: %t", gen_error_description_from_errno());
	}

    return GEN_NULL;
}

typedef struct {
    char** entries;
    gen_size_t* lengths;
    gen_size_t bounds;
} gen_backends_filesystem_internal_directory_list_entries_t;

static void gen_backends_filesystem_internal_handle_directory_list_cleanup_entries(gen_backends_filesystem_internal_directory_list_entries_t* entries) {
    for(gen_size_t i = 0; i < entries->bounds; ++i) {
        if(entries->entries[i]) {
            gen_error_t* error = gen_memory_free((void**) &entries->entries[i]);
            if(error) gen_error_abort_with_error(error, "genfilesystem");
        }
    }

    if(entries->lengths) {
        gen_error_t* error = gen_memory_free((void**) &entries->lengths);
        if(error) gen_error_abort_with_error(error, "genfilesystem");
    }

    if(entries->entries) {
        gen_error_t* error = gen_memory_free((void**) &entries->entries);
        if(error) gen_error_abort_with_error(error, "genfilesystem");
    }
}

extern gen_error_t* gen_backends_unix_filesystem_handle_directory_list(gen_filesystem_handle_t* const restrict handle, char* restrict * const restrict out_list, gen_size_t* const restrict out_lengths, gen_size_t* const restrict out_length);
gen_error_t* gen_backends_unix_filesystem_handle_directory_list(gen_filesystem_handle_t* const restrict handle, char* restrict * const restrict out_list, gen_size_t* const restrict out_lengths, gen_size_t* const restrict out_length) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_handle_directory_list, GEN_FILE_NAME);
    if(error) return error;

	rewinddir(handle->native->directory);

	GEN_CLEANUP_FUNCTION(gen_backends_filesystem_internal_handle_directory_list_cleanup_entries) gen_backends_filesystem_internal_directory_list_entries_t entries = {0};

	struct dirent* entry = GEN_NULL;
	while(gen_true) {
		// This looks hacky but it's actually what the manpage
		// Says is the correct way to check for errors in `readdir`.
		errno = 0;
		entry = readdir(handle->native->directory);
		if(!entry && errno) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not list directory: %t", gen_error_description_from_errno());
		else if(!entry) break;

        // Exclude `.` and `..` entries.
		if(entry->d_name[0] == '.' && entry->d_name[1] == '\0') continue;
		if(entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == '\0') continue;

        const gen_size_t d_name_bounds = sizeof(((struct dirent*) GEN_NULL)->d_name);

        gen_size_t length = 0;
        error = gen_string_length(entry->d_name, d_name_bounds, GEN_STRING_NO_BOUNDS, &length);
        if(error) return error;

        error = gen_memory_reallocate_zeroed((void**) &entries.lengths, entries.bounds, entries.bounds + 1, sizeof(gen_size_t));
        if(error) return error;

        entries.lengths[entries.bounds] = length;

        error = gen_memory_reallocate_zeroed((void**) &entries.entries, entries.bounds, entries.bounds + 1, sizeof(char*));
        if(error) return error;

        error = gen_memory_allocate_zeroed((void**) &entries.entries[entries.bounds], length + 1, sizeof(char));
        if(error) return error;

        error = gen_string_copy(entries.entries[entries.bounds], length + 1, entry->d_name, d_name_bounds, length, length);
        if(error) return error;
        
		++entries.bounds;
	}

	if(out_length) {
		*out_length = entries.bounds;
	}

    if(out_lengths) {
		for(gen_size_t i = 0; i < entries.bounds; ++i) {
            out_lengths[i] = entries.lengths[i];
        }
    }

	if(out_list) {
		for(gen_size_t i = 0; i < entries.bounds; ++i) {
			error = gen_string_copy(out_list[i], GEN_MEMORY_NO_BOUNDS, entries.entries[i], entries.lengths[i] + 1, entries.lengths[i], entries.lengths[i]);
			if(error) return error;
		}
	}

	rewinddir(handle->native->directory);

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_handle_lock(gen_filesystem_handle_t* const restrict handle);
gen_error_t* gen_backends_unix_filesystem_handle_lock(gen_filesystem_handle_t* const restrict handle) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_handle_lock, GEN_FILE_NAME);
    if(error) return error;    

    if(handle->type == GEN_FILESYSTEM_HANDLE_FILE) {
        int result = flock(handle->native->file, LOCK_EX);
        if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not lock file: %t", gen_error_description_from_errno());
    }

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_handle_unlock(gen_filesystem_handle_t* const restrict handle);
gen_error_t* gen_backends_unix_filesystem_handle_unlock(gen_filesystem_handle_t* const restrict handle) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_handle_unlock, GEN_FILE_NAME);
    if(error) return error;

    

    if(handle->type == GEN_FILESYSTEM_HANDLE_FILE) {
        int result = flock(handle->native->file, LOCK_UN);
        if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not lock file: %t", gen_error_description_from_errno());
    }

    return GEN_NULL;
}
