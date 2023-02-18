// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genfilesystem.h>
#include <genstring.h>
#include <genmemory.h>
#include <genprocess.h>

#include <genprocess_be.h>
#include "include/genprocess_be.h"

#include <genfilesystem_be.h>
#include <genbackendslibcerror.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <spawn.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

static void gen_process_internal_create_with_redirect_cleanup_executable_path_copy(char** executable_path_copy) {
    if(!*executable_path_copy) return;

    gen_error_t* error = gen_memory_free((void**) executable_path_copy);
	if(error) gen_error_abort_with_error(error, "genprocess");
}

typedef struct {
    char** values;
    gen_size_t bounds;
} gen_backends_process_unix_create_with_redirect_list_t;

static void gen_backends_process_internal_create_with_redirect_cleanup_list(gen_backends_process_unix_create_with_redirect_list_t* list) {
    if(!list->values) return;

    for(gen_size_t i = 0; i < list->bounds; ++i) {
        if(!list->values[i]) break;
        gen_error_t* error = gen_memory_free((void**) &list->values[i]);
    	if(error) gen_error_abort_with_error(error, "genprocess");
    }

    gen_error_t* error = gen_memory_free((void**) &list->values);
	if(error) gen_error_abort_with_error(error, "genprocess");
}

extern gen_error_t* gen_backends_unix_process_handle_create_with_redirect(const char* const restrict executable_path, GEN_UNUSED const gen_size_t executable_path_bounds, const gen_size_t executable_path_length, const char* const* const restrict arguments, const gen_size_t* const restrict argument_lengths, const gen_size_t arguments_length, const char* const restrict * const restrict environment, const gen_size_t* const restrict environment_lengths, const gen_size_t environment_length, gen_filesystem_handle_t* const restrict filesystem_handle, gen_process_handle_t* const restrict out_process);
gen_error_t* gen_backends_unix_process_handle_create_with_redirect(const char* const restrict executable_path, GEN_UNUSED const gen_size_t executable_path_bounds, const gen_size_t executable_path_length, const char* const* const restrict arguments, const gen_size_t* const restrict argument_lengths, const gen_size_t arguments_length, const char* const restrict * const restrict environment, const gen_size_t* const restrict environment_lengths, const gen_size_t environment_length, gen_filesystem_handle_t* const restrict filesystem_handle, gen_process_handle_t* const restrict out_process) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_process_handle_create_with_redirect, GEN_FILE_NAME);
    if(error) return error;

    

    posix_spawn_file_actions_t actions = {0};

    int result = posix_spawn_file_actions_init(&actions);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create process `%tz`: %t", executable_path, executable_path_length, gen_error_description_from_errno());

    result = posix_spawn_file_actions_adddup2(&actions, filesystem_handle->native->file, STDOUT_FILENO);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create process `%tz`: %t", executable_path, executable_path_length, gen_error_description_from_errno());

    result = posix_spawn_file_actions_adddup2(&actions, filesystem_handle->native->file, STDERR_FILENO);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create process `%tz`: %t", executable_path, executable_path_length, gen_error_description_from_errno());

    GEN_CLEANUP_FUNCTION(gen_process_internal_create_with_redirect_cleanup_executable_path_copy) char* executable_path_copy = GEN_NULL;
    error = gen_string_duplicate(executable_path, executable_path_length + 1, executable_path_length, executable_path_length, &executable_path_copy, GEN_NULL);
    if(error) return error;

    GEN_CLEANUP_FUNCTION(gen_backends_process_internal_create_with_redirect_cleanup_list) gen_backends_process_unix_create_with_redirect_list_t arguments_copy = {0};
    error = gen_memory_allocate_zeroed((void**) &arguments_copy.values, arguments_length + 2, sizeof(char*));
    if(error) return error;
    arguments_copy.bounds = arguments_length + 2;

    arguments_copy.values[0] = executable_path_copy;
    executable_path_copy = GEN_NULL;

    for(gen_size_t i = 0; i < arguments_length; ++i) {
        error = gen_string_duplicate(arguments[i], argument_lengths[i] + 1, argument_lengths[i], GEN_STRING_NO_BOUNDS, &arguments_copy.values[i + 1], GEN_NULL);
        if(error) return error;
    }

    GEN_CLEANUP_FUNCTION(gen_backends_process_internal_create_with_redirect_cleanup_list) gen_backends_process_unix_create_with_redirect_list_t environment_copy = {0};
    if(environment_length) {
        error = gen_memory_allocate_zeroed((void**) &environment_copy, environment_length + 1, sizeof(char*));
        if(error) return error;

        for(gen_size_t i = 0; i < environment_length; ++i) {
            error = gen_string_duplicate(environment[i], environment_lengths[i] + 1, environment_lengths[i], GEN_STRING_NO_BOUNDS, &environment_copy.values[i], GEN_NULL);
            if(error) return error;
        }

        arguments_copy.bounds = environment_length + 1;
    }

    result = posix_spawnp(&out_process->native->pid, executable_path, &actions, GEN_NULL, arguments_copy.values, environment_copy.values);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create process `%tz`: %t", executable_path, executable_path_length, gen_error_description_from_errno());

    result = posix_spawn_file_actions_destroy(&actions);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create process `%tz`: %t", executable_path, executable_path_length, gen_error_description_from_errno());

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_process_handle_wait(const gen_process_handle_t* const restrict process, int* const restrict out_exitcode);
gen_error_t* gen_backends_unix_process_handle_wait(const gen_process_handle_t* const restrict process, int* const restrict out_exitcode) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_process_handle_wait, GEN_FILE_NAME);
    if(error) return error;

    

    int status = 0;
    pid_t result = waitpid(process->native->pid, &status, 0);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not await process %uz: %t", (gen_size_t) process->native->pid, gen_error_description_from_errno());

    *out_exitcode = WEXITSTATUS(status);

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_process_handle_kill(const gen_process_handle_t* const restrict process);
gen_error_t* gen_backends_unix_process_handle_kill(const gen_process_handle_t* const restrict process) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_process_handle_kill, GEN_FILE_NAME);
    if(error) return error;

    

    int result = kill(process->native->pid, SIGKILL);
    if(result == -1 && errno == ESRCH) return GEN_NULL;
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not kill process %uz: %t", (gen_size_t) process->native->pid, gen_error_description_from_errno());

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_process_handle_check(const gen_process_handle_t* const restrict process, gen_bool_t* const restrict out_alive);
gen_error_t* gen_backends_unix_process_handle_check(const gen_process_handle_t* const restrict process, gen_bool_t* const restrict out_alive) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_process_handle_check, GEN_FILE_NAME);
    if(error) return error;

    

    int result = kill(process->native->pid, 0);
    if(result == -1 && errno == ESRCH) {
        *out_alive = gen_false;
        return GEN_NULL;
    }
    else if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not check process %uz: %t", (gen_size_t) process->native->pid, gen_error_description_from_errno());

    *out_alive = gen_true;

    return GEN_NULL;
}

extern char** environ;

extern gen_error_t* gen_backends_unix_process_get_environment(char* restrict * const restrict out_environment, gen_size_t* const restrict out_lengths, gen_size_t* const restrict out_length);
gen_error_t* gen_backends_unix_process_get_environment(char* restrict * const restrict out_environment, gen_size_t* const restrict out_lengths, gen_size_t* const restrict out_length) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_process_get_environment, GEN_FILE_NAME);
    if(error) return error;

    gen_size_t length = 0;
    for(; environ[length]; ++length);

    if(out_length) *out_length = length;

    if(out_lengths) {
        for(gen_size_t i = 0; i < length; ++i) {
            error = gen_string_length(environ[i], GEN_STRING_NO_BOUNDS, GEN_STRING_NO_BOUNDS, &out_lengths[i]);
        	if(error) return error;
        }
    }

    if(out_environment) {
        for(gen_size_t i = 0; i < length; ++i) {
            error = gen_string_copy(out_environment[i], GEN_STRING_NO_BOUNDS, environ[i], GEN_STRING_NO_BOUNDS, GEN_STRING_NO_BOUNDS, GEN_STRING_NO_BOUNDS);
        	if(error) return error;
        }
    }

    return GEN_NULL;
}
