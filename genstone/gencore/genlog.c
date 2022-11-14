// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genlog.h"

#include "include/gencommon.h"
#include "include/genfilesystem.h"
#include "include/genmemory.h"
#include "include/genstring.h"

#include <genbackends.h>

gen_error_t* gen_log(const gen_log_level_t severity, const char* const restrict context, const char* const restrict string) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_log, GEN_FILE_NAME);
	if(error) return error;

	if(!context) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`context` was `GEN_NULL`");
	if(!string) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(log)(severity, context, string);
	if(error) return error;

	return GEN_NULL;
}

static void gen_log_internal_cleanup_formatted(char** formatted) {
	gen_error_t* error = gen_memory_free((void**) formatted);
	if(error) gen_error_abort_with_error(error, "genlog");
}

gen_error_t* gen_log_formatted(const gen_log_level_t severity, const char* const restrict context, const char* const restrict format, ...) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_log_formatted, GEN_FILE_NAME);
	if(error) return error;

	if(!context) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`context` was `GEN_NULL`");
	if(!format) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `GEN_NULL`");

	gen_variadic_list_t args;
	gen_variadic_list_t args_copy;
	gen_variadic_list_start(args, format);
	gen_variadic_list_copy(args_copy, args);

	gen_size_t format_length = 0;
	error = gen_string_length(format, GEN_STRING_NO_BOUNDS, GEN_STRING_NO_BOUNDS, &format_length);
	if(error) return error;

	gen_size_t formatted_length = 0;
	error = gen_string_formatv(GEN_STRING_NO_BOUNDS, GEN_NULL, &formatted_length, format, GEN_STRING_NO_BOUNDS, format_length, args);
	if(error) return error;

	GEN_CLEANUP_FUNCTION(gen_log_internal_cleanup_formatted) char* formatted = GEN_NULL;
	error = gen_memory_allocate_zeroed((void**) &formatted, formatted_length + 1, sizeof(char));
	if(error) return error;

	error = gen_string_formatv(GEN_STRING_NO_BOUNDS, formatted, GEN_NULL, format, GEN_STRING_NO_BOUNDS, format_length, args_copy);
	if(error) return error;

	error = gen_log(severity, context, formatted);
	if(error) return error;

    return GEN_NULL;
}
