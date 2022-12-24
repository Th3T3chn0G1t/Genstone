// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genprocess.h"

#include "include/genmemory.h"
#include "include/genstring.h"

#include <genprocess_be.h>
#include <genbackends.h>

static void gen_process_internal_create_with_redirect_cleanup_native(gen_backends_process_handle_t** native) {
    if(!*native) return;

    gen_error_t* error = gen_memory_free((void**) native);
    if(error) gen_error_abort_with_error(error, "genprocess");
}

gen_error_t* gen_process_create_with_redirect(const char* const restrict executable_path, const gen_size_t executable_path_bounds, const gen_size_t executable_path_length, const char* const* const restrict arguments, const gen_size_t* const restrict argument_lengths, const gen_size_t arguments_length, const char* const restrict * const restrict environment, const gen_size_t* const restrict environment_lengths, const gen_size_t environment_length, gen_filesystem_handle_t* const restrict filesystem_handle, gen_process_handle_t* const restrict out_process) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_path_canonicalize, GEN_FILE_NAME);
	if(error) return error;

	if(!executable_path) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`executable_path` was `GEN_NULL`");
	if(arguments_length && !arguments) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`arguments` was `GEN_NULL`");
	if(arguments_length && !argument_lengths) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`argument_lengths` was `GEN_NULL`");
	if(environment_length && !environment) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`environment` was `GEN_NULL`");
	if(environment_length && !environment_lengths) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`environment_lengths` was `GEN_NULL`");
	if(!filesystem_handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`filesystem_handle` was `GEN_NULL`");
	if(!out_process) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_process` was `GEN_NULL`");

    gen_size_t executable_path_length_checked = 0;
    error = gen_string_length(executable_path, executable_path_bounds, executable_path_length, &executable_path_length_checked);
	if(error) return error;

    gen_bool_t valid = gen_false;
	error = gen_filesystem_path_validate(executable_path, executable_path_bounds, executable_path_length_checked, &valid);
	if(error) return error;
    if(!valid) return gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`path` `%tz` was invalid", executable_path, executable_path_length_checked);

    error = gen_memory_allocate_zeroed((void**) &out_process->native, 1, sizeof(gen_backends_process_handle_t));
	if(error) return error;

    GEN_CLEANUP_FUNCTION(gen_process_internal_create_with_redirect_cleanup_native) gen_backends_process_handle_t* native_scope_variable = out_process->native;

    error = GEN_BACKENDS_CALL(process_create_with_redirect)(executable_path, executable_path_bounds, executable_path_length_checked, arguments, argument_lengths, arguments_length, environment, environment_lengths, environment_length, filesystem_handle, out_process);
	if(error) return error;

    native_scope_variable = GEN_NULL;

    return GEN_NULL;
}

gen_error_t* gen_process_wait(const gen_process_handle_t* const restrict process, int* const restrict out_exitcode) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_process_wait, GEN_FILE_NAME);
	if(error) return error;

	if(!process) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`process` was `GEN_NULL`");
	if(!out_exitcode) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_exitcode` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(process_wait)(process, out_exitcode);
	if(error) return error;

    return GEN_NULL;
}

gen_error_t* gen_process_kill(const gen_process_handle_t* const restrict process) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_process_kill, GEN_FILE_NAME);
	if(error) return error;

	if(!process) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`process` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(process_kill)(process);    
	if(error) return error;

    return GEN_NULL;
}

gen_error_t* gen_process_check(const gen_process_handle_t* const restrict process, gen_bool_t* const restrict out_alive) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_process_check, GEN_FILE_NAME);
	if(error) return error;

	if(!process) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`process` was `GEN_NULL`");
	if(!out_alive) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_alive` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(process_check)(process, out_alive);
	if(error) return error;

    return GEN_NULL;
}
gen_error_t* gen_process_get_environment(char* restrict * const restrict out_environment, gen_size_t* const restrict out_lengths, gen_size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_process_get_environment, GEN_FILE_NAME);
	if(error) return error;

    error = GEN_BACKENDS_CALL(process_get_environment)(out_environment, out_lengths, out_length);
	if(error) return error;

    return GEN_NULL;
}
