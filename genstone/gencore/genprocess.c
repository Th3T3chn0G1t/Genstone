// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genprocess.h"

#include "include/genmemory.h"
#include "include/genstring.h"

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <spawn.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)
#endif

static void gen_process_internal_create_with_redirect_cleanup_executable_path_copy(char** executable_path_copy) {
    if(!*executable_path_copy) return;

    gen_error_t* error = gen_memory_free((void**) executable_path_copy);
	if(error) gen_error_abort_with_error(error, "genprocess");
}

static void gen_process_internal_create_with_redirect_cleanup_arguments_copy(char*** arguments_copy) {
    if(!*arguments_copy) return;

    for(gen_size_t i = 0; (*arguments_copy)[i]; ++i) {
        gen_error_t* error = gen_memory_free((void**) &((*arguments_copy)[i]));
    	if(error) gen_error_abort_with_error(error, "genprocess");
    }

    gen_error_t* error = gen_memory_free((void**) arguments_copy);
	if(error) gen_error_abort_with_error(error, "genprocess");
}

gen_error_t* gen_process_create_with_redirect(const char* const restrict executable_path, const gen_size_t executable_path_length, const char* const* const restrict arguments, const gen_size_t* const restrict argument_lengths, const gen_size_t arguments_length, const char* const restrict * const restrict environment, const gen_size_t* const restrict environment_lengths, const gen_size_t environment_length, gen_filesystem_handle_t* const restrict filesystem_handle, gen_process_t* const restrict out_process) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_canonicalize, GEN_FILE_NAME);
	if(error) return error;

	if(!executable_path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`executable_path` was `GEN_NULL`");
	if(arguments_length && !arguments) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`arguments` was `GEN_NULL`");
	if(arguments_length && !argument_lengths) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`argument_lengths` was `GEN_NULL`");
	if(environment_length && !environment) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`environment` was `GEN_NULL`");
	if(environment_length && !environment_lengths) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`environment_lengths` was `GEN_NULL`");
	if(!filesystem_handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`filesystem_handle` was `GEN_NULL`");
	if(!out_process) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_process` was `GEN_NULL`");

 	error = gen_filesystem_path_validate(executable_path, executable_path_length);
	if(error) return error;
    
#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
    posix_spawn_file_actions_t actions = {0};

    int result = posix_spawn_file_actions_init(&actions);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create process `%tz`: %t", executable_path, executable_path_length, gen_error_description_from_errno());

    result = posix_spawn_file_actions_adddup2(&actions, filesystem_handle->file_handle, STDOUT_FILENO);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create process `%tz`: %t", executable_path, executable_path_length, gen_error_description_from_errno());

    result = posix_spawn_file_actions_adddup2(&actions, filesystem_handle->file_handle, STDERR_FILENO);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create process `%tz`: %t", executable_path, executable_path_length, gen_error_description_from_errno());

    GEN_CLEANUP_FUNCTION(gen_process_internal_create_with_redirect_cleanup_executable_path_copy) char* executable_path_copy = GEN_NULL;
    error = gen_string_duplicate(executable_path, executable_path_length + 1, executable_path_length, &executable_path_copy, GEN_NULL);
    if(error) return error;

    GEN_CLEANUP_FUNCTION(gen_process_internal_create_with_redirect_cleanup_arguments_copy) char** arguments_copy = GEN_NULL;
    error = gen_memory_allocate_zeroed((void**) &arguments_copy, arguments_length + 2, sizeof(char*));
    if(error) return error;

    arguments_copy[0] = executable_path_copy;
    executable_path_copy = GEN_NULL;

    for(gen_size_t i = 0; i < arguments_length; ++i) {
        error = gen_string_duplicate(arguments[i], argument_lengths[i], GEN_STRING_NO_BOUNDS, &arguments_copy[i + 1], GEN_NULL);
        if(error) return error;
    }

    GEN_CLEANUP_FUNCTION(gen_process_internal_create_with_redirect_cleanup_arguments_copy) char** environment_copy = GEN_NULL;
    if(environment_length) {
        error = gen_memory_allocate_zeroed((void**) &environment_copy, environment_length + 1, sizeof(char*));
        if(error) return error;

        for(gen_size_t i = 0; i < environment_length; ++i) {
            error = gen_string_duplicate(environment[i], environment_lengths[i], GEN_STRING_NO_BOUNDS, &environment_copy[i], GEN_NULL);
            if(error) return error;
        }
    }

    result = posix_spawnp(out_process, executable_path, &actions, GEN_NULL, arguments_copy, environment_copy);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create process `%tz`: %t", executable_path, executable_path_length, gen_error_description_from_errno());

    result = posix_spawn_file_actions_destroy(&actions);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create process `%tz`: %t", executable_path, executable_path_length, gen_error_description_from_errno());
#endif

    return GEN_NULL;
}

gen_error_t* gen_process_wait(const gen_process_t* const restrict process, int* const restrict out_exitcode) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_process_wait, GEN_FILE_NAME);
	if(error) return error;

	if(!process) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`process` was `GEN_NULL`");
	if(!out_exitcode) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_exitcode` was `GEN_NULL`");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
    int status = 0;
    gen_process_t result = waitpid(*process, &status, 0);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not await process %uz: %t", (gen_size_t) *process, gen_error_description_from_errno());

    *out_exitcode = WEXITSTATUS(status);
#endif

    return GEN_NULL;
}

gen_error_t* gen_process_kill(const gen_process_t* const restrict process) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_process_kill, GEN_FILE_NAME);
	if(error) return error;

	if(!process) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`process` was `GEN_NULL`");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
    int result = kill(*process, SIGKILL);
    if(result == -1 && errno == ESRCH) return GEN_NULL;
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not kill process %uz: %t", (gen_size_t) *process, gen_error_description_from_errno());
#endif

    return GEN_NULL;
}

gen_error_t* gen_process_check(const gen_process_t* const restrict process, gen_bool_t* const restrict out_alive) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_process_check, GEN_FILE_NAME);
	if(error) return error;

	if(!process) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`process` was `GEN_NULL`");
	if(!out_alive) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_alive` was `GEN_NULL`");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
    int result = kill(*process, 0);
    if(result == -1 && errno == ESRCH) {
        *out_alive = gen_false;
        return GEN_NULL;
    }
    else if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not check process %uz: %t", (gen_size_t) *process, gen_error_description_from_errno());

    *out_alive = gen_true;
#endif

    return GEN_NULL;
}

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
extern char** environ;
#endif

// TODO: Make this copy-out - output length, then lengths, then the environment 
gen_error_t* gen_process_get_environment(char*** const restrict out_environment, gen_size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_process_kill, GEN_FILE_NAME);
	if(error) return error;

	if(!out_environment) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_environment` was `GEN_NULL`");
	if(!out_length) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_length` was `GEN_NULL`");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
    *out_environment = environ;

    for(*out_length = 0; (*out_environment)[*out_length]; ++*out_length);
#endif

    return GEN_NULL;
}
