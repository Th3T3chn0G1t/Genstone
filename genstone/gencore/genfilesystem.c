// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genfilesystem.h"

#include "include/genmemory.h"
#include "include/genstring.h"

#include <genbackends.h>
#include <genfilesystem_be.h>
#include <genfilesystem_be_watcher.h>

gen_error_t* gen_filesystem_path_canonicalize(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length, char* restrict out_canonical, gen_size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_canonicalize, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `GEN_NULL`");

    gen_size_t path_length_checked = 0;
    error = gen_string_length(path, path_bounds, path_length, &path_length_checked);
	if(error) return error;

    gen_bool_t valid = gen_false;
	error = gen_filesystem_path_validate(path, path_bounds, path_length_checked, &valid);
	if(error) return error;
    if(!valid) return gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` `%tz` was invalid", path, path_length_checked);

    gen_bool_t exists = gen_false;
    error = gen_filesystem_path_exists(path, path_bounds, path_length_checked, &exists);
	if(error) return error;
    if(!exists) return gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` `%tz` does not exist", path, path_length_checked);

    error = GEN_BACKENDS_CALL(filesystem_path_canonicalize)(path, path_bounds, path_length_checked, out_canonical, out_length);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_path_exists(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length, gen_bool_t* const restrict out_exists) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_exists, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `GEN_NULL`");
	if(!out_exists) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_exists` was `GEN_NULL`");

    gen_size_t path_length_checked = 0;
    error = gen_string_length(path, path_bounds, path_length, &path_length_checked);
	if(error) return error;

    gen_bool_t valid = gen_false;
	error = gen_filesystem_path_validate(path, path_bounds, path_length_checked, &valid);
	if(error) return error;
    if(!valid) return gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` `%tz` was invalid", path, path_length_checked);

    error = GEN_BACKENDS_CALL(filesystem_path_exists)(path, path_bounds, path_length_checked, out_exists);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_path_validate(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length, gen_bool_t* const restrict out_valid) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_validate, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `GEN_NULL`");
	if(!out_valid) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_valid` was `GEN_NULL`");

    gen_size_t path_length_checked = 0;
    error = gen_string_length(path, path_bounds, path_length, &path_length_checked);
	if(error) return error;

    error = GEN_BACKENDS_CALL(filesystem_path_validate)(path, path_bounds, path_length_checked, out_valid);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_path_create_file(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_create_file, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `GEN_NULL`");

    gen_size_t path_length_checked = 0;
    error = gen_string_length(path, path_bounds, path_length, &path_length_checked);
	if(error) return error;

    gen_bool_t valid = gen_false;
	error = gen_filesystem_path_validate(path, path_bounds, path_length_checked, &valid);
	if(error) return error;
    if(!valid) return gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` `%tz` was invalid", path, path_length_checked);

    error = GEN_BACKENDS_CALL(filesystem_path_create_file)(path, path_bounds, path_length_checked);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_path_create_directory(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_create_directory, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `GEN_NULL`");

    gen_size_t path_length_checked = 0;
    error = gen_string_length(path, path_bounds, path_length, &path_length_checked);
	if(error) return error;

    gen_bool_t valid = gen_false;
	error = gen_filesystem_path_validate(path, path_bounds, path_length_checked, &valid);
	if(error) return error;
    if(!valid) return gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` `%tz` was invalid", path, path_length_checked);

    error = GEN_BACKENDS_CALL(filesystem_path_create_directory)(path, path_bounds, path_length_checked);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_path_delete(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_delete, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `GEN_NULL`");

    gen_size_t path_length_checked = 0;
    error = gen_string_length(path, path_bounds, path_length, &path_length_checked);
	if(error) return error;

    gen_bool_t valid = gen_false;
	error = gen_filesystem_path_validate(path, path_bounds, path_length_checked, &valid);
	if(error) return error;
    if(!valid) return gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` `%tz` was invalid", path, path_length_checked);

    gen_bool_t exists = gen_false;
    error = gen_filesystem_path_exists(path, path_bounds, path_length_checked, &exists);
	if(error) return error;
    if(!exists) return gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` `%tz` does not exist", path, path_length_checked);

    error = GEN_BACKENDS_CALL(filesystem_path_delete)(path, path_bounds, path_length_checked);
	if(error) return error;

	return GEN_NULL;
}

static void gen_filesystem_internal_handle_open_cleanup_lock(gen_threads_mutex_t** mutex) {
	if(!*mutex) return;

	gen_error_t* error = gen_threads_mutex_destroy(*mutex);
	if(error) gen_error_abort_with_error(error, "genfilesystem");
}

static void gen_filesystem_internal_handle_open_cleanup_native(gen_backends_filesystem_handle_t** native) {
	if(!*native) return;

	gen_error_t* error = gen_memory_free((void**) *native);
	if(error) gen_error_abort_with_error(error, "genfilesystem");
}

gen_error_t* gen_filesystem_handle_open_anonymous(gen_filesystem_handle_t* restrict out_handle) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_open_anonymous, GEN_FILE_NAME);
	if(error) return error;

	if(!out_handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_handle` was `GEN_NULL`");

	error = gen_threads_mutex_create(&out_handle->lock);
	if(error) return error;

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_lock) gen_threads_mutex_t* lock_scope_variable = &out_handle->lock;

    error = gen_memory_allocate_zeroed((void**) &out_handle->native, 1, sizeof(gen_backends_filesystem_handle_t));
	if(error) return error;

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_native) gen_backends_filesystem_handle_t* native_scope_variable = out_handle->native;

    error = GEN_BACKENDS_CALL(filesystem_handle_open_anonymous)(out_handle);
	if(error) return error;

    native_scope_variable = GEN_NULL;
    lock_scope_variable = GEN_NULL;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_handle_open(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length, gen_filesystem_handle_t* const restrict out_handle) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_open, GEN_FILE_NAME);
	if(error) return error;

	if(!path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` was `GEN_NULL`");
	if(!out_handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_handle` was `GEN_NULL`");

    gen_size_t path_length_checked = 0;
    error = gen_string_length(path, path_bounds, path_length, &path_length_checked);
	if(error) return error;

    gen_bool_t valid = gen_false;
	error = gen_filesystem_path_validate(path, path_bounds, path_length_checked, &valid);
	if(error) return error;
    if(!valid) return gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` `%tz` was invalid", path, path_length_checked);

    gen_bool_t exists = gen_false;
    error = gen_filesystem_path_exists(path, path_bounds, path_length_checked, &exists);
	if(error) return error;
    if(!exists) return gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` `%tz` does not exist", path, path_length_checked);

	error = gen_threads_mutex_create(&out_handle->lock);
	if(error) return error;

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_lock) gen_threads_mutex_t* lock_scope_variable = &out_handle->lock;

    error = gen_memory_allocate_zeroed((void**) &out_handle->native, 1, sizeof(gen_backends_filesystem_handle_t));
	if(error) return error;

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_native) gen_backends_filesystem_handle_t* native_scope_variable = out_handle->native;

    error = GEN_BACKENDS_CALL(filesystem_handle_open)(path, path_bounds, path_length_checked, out_handle);
	if(error) return error;

    native_scope_variable = GEN_NULL;
	lock_scope_variable = GEN_NULL;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_handle_close(gen_filesystem_handle_t* const restrict handle) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_close, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(filesystem_handle_close)(handle);
	if(error) return error;

    error = gen_memory_free((void**) &handle->native);
	if(error) return error;
	
    error = gen_threads_mutex_destroy(&handle->lock);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_handle_file_size(gen_filesystem_handle_t* const restrict handle, gen_size_t* const restrict out_size) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_file_size, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");
	if(!out_size) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_size` was `GEN_NULL`");

	if(handle->type != GEN_FILESYSTEM_HANDLE_FILE && handle->type != GEN_FILESYSTEM_HANDLE_ANONYMOUS) return gen_error_attach_backtrace(GEN_ERROR_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a file");

    error = GEN_BACKENDS_CALL(filesystem_handle_file_size)(handle, out_size);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_handle_file_read(gen_filesystem_handle_t* const restrict handle, const gen_size_t offset, const gen_size_t length, unsigned char* restrict out_buffer) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_file_read, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");
	if(!out_buffer) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`output_buffer` was `GEN_NULL`");

	if(handle->type != GEN_FILESYSTEM_HANDLE_FILE && handle->type != GEN_FILESYSTEM_HANDLE_ANONYMOUS) return gen_error_attach_backtrace(GEN_ERROR_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a file");

    error = GEN_BACKENDS_CALL(filesystem_handle_file_read)(handle, offset, length, out_buffer);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_handle_file_write(gen_filesystem_handle_t* const restrict handle, const gen_size_t offset, const unsigned char* const restrict buffer, const gen_size_t length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_file_write, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");
	if(!buffer) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`buffer` was `GEN_NULL`");

	if(handle->type != GEN_FILESYSTEM_HANDLE_FILE && handle->type != GEN_FILESYSTEM_HANDLE_ANONYMOUS) return gen_error_attach_backtrace(GEN_ERROR_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a file");

    error = GEN_BACKENDS_CALL(filesystem_handle_file_write)(handle, offset, buffer, length);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_handle_directory_list(gen_filesystem_handle_t* const restrict handle, char* restrict * const restrict out_list, gen_size_t* const restrict out_lengths, gen_size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_directory_list, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");

    if(handle->type != GEN_FILESYSTEM_HANDLE_DIRECTORY) return gen_error_attach_backtrace(GEN_ERROR_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was not a directory");

    error = GEN_BACKENDS_CALL(filesystem_handle_directory_list)(handle, out_list, out_lengths, out_length);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_handle_lock(gen_filesystem_handle_t* const restrict handle) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_lock, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");

	error = gen_threads_mutex_lock(&handle->lock);
	if(error) return error;

    error = GEN_BACKENDS_CALL(filesystem_handle_lock)(handle);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_filesystem_handle_unlock(gen_filesystem_handle_t* const restrict handle) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_handle_unlock, GEN_FILE_NAME);
	if(error) return error;

	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(filesystem_handle_unlock)(handle);
	if(error) return error;

	error = gen_threads_mutex_unlock(&handle->lock);
	if(error) return error;

	return GEN_NULL;
}

static void gen_filesystem_internal_watcher_create_cleanup_native(gen_backends_filesystem_watcher_handle_t** native) {
	if(!*native) return;

	gen_error_t* error = gen_memory_free((void**) *native);
	if(error) gen_error_abort_with_error(error, "genfilesystem");
}

gen_error_t* gen_filesystem_watcher_create(gen_filesystem_watcher_t* const restrict out_watcher) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_watcher_create, GEN_FILE_NAME);
    if(error) return error;

    if(!out_watcher) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_watcher` was `GEN_NULL`");

    error = gen_memory_allocate_zeroed((void**) &out_watcher->native, 1, sizeof(gen_backends_filesystem_watcher_handle_t));
    if(error) return error;

    GEN_CLEANUP_FUNCTION(gen_filesystem_internal_watcher_create_cleanup_native) gen_backends_filesystem_watcher_handle_t* native_scope_variable = out_watcher->native;

    error = GEN_BACKENDS_CALL(filesystem_watcher_create)(out_watcher);
    if(error) return error;

    native_scope_variable = GEN_NULL;

    return GEN_NULL;
}

gen_error_t* gen_filesystem_watcher_destroy(gen_filesystem_watcher_t* const restrict watcher) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_watcher_destroy, GEN_FILE_NAME);
    if(error) return error;

    if(!watcher) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`watcher` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(filesystem_watcher_destroy)(watcher);
    if(error) return error;

    return GEN_NULL;
}

gen_error_t* gen_filesystem_watcher_add(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_handle_t* const restrict handle, gen_filesystem_watcher_id_t* const restrict out_id) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_watcher_add, GEN_FILE_NAME);
    if(error) return error;

    if(!watcher) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`watcher` was `GEN_NULL`");
    if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(filesystem_watcher_add)(watcher, handle, out_id);
    if(error) return error;

    return GEN_NULL;
}

gen_error_t* gen_filesystem_watcher_remove(gen_filesystem_watcher_t* const restrict watcher, const gen_filesystem_watcher_id_t id) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_watcher_remove, GEN_FILE_NAME);
    if(error) return error;

    if(!watcher) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`watcher` was `GEN_NULL`");
    if(id == GEN_FILESYSTEM_WATCHER_ID_NONE) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`id` was `GEN_FILESYSTEM_WATCHER_ID_NONE`");

    error = GEN_BACKENDS_CALL(filesystem_watcher_remove)(watcher, id);
    if(error) return error;

    return GEN_NULL;
}

gen_error_t* gen_filesystem_watcher_poll(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_watcher_event_t* const restrict out_event, gen_filesystem_watcher_id_t* const restrict out_id) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_watcher_poll, GEN_FILE_NAME);
    if(error) return error;

    if(!watcher) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`watcher` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(filesystem_watcher_poll)(watcher, out_event, out_id);
    if(error) return error;

    return GEN_NULL;
}


/*
gen_error_t* gen_filesystem_watcher_create(gen_filesystem_handle_t* const restrict handle, gen_filesystem_watcher_t* const restrict out_watcher) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_watcher_create, GEN_FILE_NAME);
	if(error) return error;

	if(!out_watcher) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_watcher` was `GEN_NULL`");
	if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");

    if(handle->type == GEN_FILESYSTEM_HANDLE_ANONYMOUS) return gen_error_attach_backtrace(GEN_ERROR_WRONG_OBJECT_TYPE, GEN_LINE_NUMBER, "`handle` was an anonymous file");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
	error = gen_threads_mutex_create(&out_watcher->lock);
	if(error) return error;

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_lock)
	gen_threads_mutex_t* lock_scope_variable = &out_watcher->lock;
#endif

#if GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY == GEN_ENABLED
#if GEN_PLATFORM == GEN_LINUX
	out_watcher->type = GEN_FILESYSTEM_HANDLE_WATCHER;

	static const char format[] = "/proc/self/fd/%si";

	gen_size_t formatted_length = 0;
	error = gen_string_format(GEN_STRING_NO_BOUNDS, GEN_NULL, &formatted_length, format, sizeof(format) - 1, handle->file_handle);
	if(error) return error;

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_directory_list_entries_cleanup)
	char* link = GEN_NULL;
	error = gen_memory_allocate_zeroed((void**) &link, formatted_length + 1, sizeof(char));
	if(error) return error;

	error = gen_string_format(GEN_STRING_NO_BOUNDS, link, GEN_NULL, format, sizeof(format) - 1, handle->file_handle);
	if(error) return error;

	struct stat link_stat = {0};
	int result = lstat(link, &link_stat);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

	gen_size_t path_size = (gen_size_t) link_stat.st_size + 1;
	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_directory_list_entries_cleanup)
	char* path = GEN_NULL;
	error = gen_memory_allocate_zeroed((void**) &path, path_size, sizeof(char));
	if(error) return error;

	gen_ssize_t path_length_result = readlink(link, path, path_size);
	if(path_length_result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

	result = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_file_handle)
	gen_filesystem_file_handle_t* file_handle_scope_variable = &result;
	out_watcher->file_handle = result;

	result = inotify_add_watch(out_watcher->file_handle, path, IN_ATTRIB | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

	lock_scope_variable = GEN_NULL;
	file_handle_scope_variable = GEN_NULL;

	return GEN_NULL;
#endif
#else
#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
	out_watcher->type = GEN_FILESYSTEM_HANDLE_WATCHER;

	int result = fcntl(handle->file_handle, F_DUPFD);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_file_handle)
	gen_filesystem_file_handle_t* file_handle_scope_variable = &result;
	out_watcher->file_handle = result;

	if(handle->type == GEN_FILESYSTEM_HANDLE_DIRECTORY) {
		out_watcher->directory_handle = fdopendir(out_watcher->file_handle);
		if(!out_watcher->directory_handle) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

		error = gen_filesystem_handle_directory_list(handle, GEN_NULL, &out_watcher->internal_directory_length_cached);
		if(error) return error;
	}

	GEN_CLEANUP_FUNCTION(gen_filesystem_internal_handle_open_cleanup_directory_handle)
	gen_filesystem_directory_handle_t* directory_handle_scope_variable = &out_watcher->directory_handle;

    error = gen_memory_allocate_zeroed((void**) &out_watcher->internal_stat_cached, 1, sizeof(struct stat));
    if(error) return error;

    GEN_CLEANUP_FUNCTION(gen_filesystem_internal_watcher_create_cleanup_stat)
    struct stat* stat_scope_variable = out_watcher->internal_stat_cached;

	result = fstat(out_watcher->file_handle, out_watcher->internal_stat_cached);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create file watcher: %t", gen_error_description_from_errno());

	lock_scope_variable = GEN_NULL;
	file_handle_scope_variable = GEN_NULL;
	directory_handle_scope_variable = GEN_NULL;
    stat_scope_variable = GEN_NULL;

#endif
#endif

	return GEN_NULL;
}

gen_error_t* gen_filesystem_watcher_destroy(gen_filesystem_watcher_t* const restrict watcher) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_watcher_destroy, GEN_FILE_NAME);
	if(error) return error;

	if(!watcher) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`watcher` was `GEN_NULL`");
	if(watcher->type != GEN_FILESYSTEM_HANDLE_WATCHER) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`watcher` was not a watcher");

	watcher->type = watcher->directory_handle ? GEN_FILESYSTEM_HANDLE_DIRECTORY : GEN_FILESYSTEM_HANDLE_FILE;

	error = gen_filesystem_handle_close(watcher);
	if(error) return error;

#if GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY == GEN_ENABLED
#if GEN_PLATFORM == GEN_LINUX
#endif
#else
#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
    error = gen_memory_free(watcher->internal_stat_cached);
    if(error) return error;
#endif
#endif

	return GEN_NULL;
}

#if GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY == GEN_ENABLED && GEN_PLATFORM == GEN_LINUX
static void gen_filesystem_internal_watcher_poll_raw_events_cleanup(unsigned char** raw_events) {
	gen_error_t* error = gen_memory_free((void**) raw_events);
	if(error) gen_error_abort_with_error(error, "genfilesystem");
}
#endif

gen_error_t* gen_filesystem_watcher_poll(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_watcher_event_t* const restrict out_event) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_watcher_poll, GEN_FILE_NAME);
	if(error) return error;

	if(!watcher) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`watcher` was `GEN_NULL`");
	if(!out_event) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_event` was `GEN_NULL`");

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
		GEN_ALIGNAS(GEN_ALIGNOF(struct inotify_event*)) unsigned char* raw_events = GEN_NULL;
		error = gen_memory_allocate_zeroed_aligned((void**) &raw_events, events_size, sizeof(unsigned char), GEN_ALIGNOF(struct inotify_event));
		if(error) return error;

		gen_ssize_t events_size_result = read(watcher->file_handle, raw_events, events_size);
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

	return GEN_NULL;
#endif
#else
#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
	struct stat watcher_stat = {0};
	int result = fstat(watcher->file_handle, &watcher_stat);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not poll for file watcher events: %t", gen_error_description_from_errno());

#if GEN_PLATFORM == GEN_OSX
	if(    watcher_stat.st_mtimespec.tv_sec  != ((struct stat*) watcher->internal_stat_cached)->st_mtimespec.tv_sec
        || watcher_stat.st_mtimespec.tv_nsec != ((struct stat*) watcher->internal_stat_cached)->st_mtimespec.tv_nsec
        || watcher_stat.st_ctimespec.tv_sec  != ((struct stat*) watcher->internal_stat_cached)->st_ctimespec.tv_sec
        || watcher_stat.st_ctimespec.tv_nsec != ((struct stat*) watcher->internal_stat_cached)->st_ctimespec.tv_nsec) {
#else
	if(    watcher_stat.st_mtime != ((struct stat*) watcher->internal_stat_cached).st_mtime
        || watcher_stat.st_ctime != ((struct stat*) watcher->internal_stat_cached).st_ctime) {
#endif
		if(watcher->directory_handle) {
			gen_size_t items_length = 0;
			error = gen_filesystem_handle_directory_list(watcher, GEN_NULL, &items_length);
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

    error = gen_memory_copy(watcher->internal_stat_cached, sizeof(struct stat), &watcher_stat, sizeof(struct stat), sizeof(struct stat));
    if(error) return error;

#endif
#endif
	return GEN_NULL;
}
*/
