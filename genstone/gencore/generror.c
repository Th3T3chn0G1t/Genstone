// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"
#include "include/genlog.h"
#include "include/genmemory.h"
#include "include/genstring.h"
#include "include/genthreads.h"

#include <genbackends.h>

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_PLATFORM == GEN_WINDOWS || GEN_FORCE_UNIX == GEN_ENABLED
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)
#endif

// TODO: Add some sort of `setjmp`/`longjmp` mechanism for returning errors after cleanup attribute

const char* gen_error_type_name(const gen_error_type_t error) {
	switch(error) {
		case GEN_ERROR_UNKNOWN: return "GEN_ERROR_UNKNOWN";
		case GEN_ERROR_PERMISSION: return "GEN_ERROR_PERMISSION";
		case GEN_ERROR_INVALID_PARAMETER: return "GEN_ERROR_INVALID_PARAMETER";
		case GEN_ERROR_IO: return "GEN_ERROR_IO";
		case GEN_ERROR_TOO_LONG: return "GEN_ERROR_TOO_LONG";
		case GEN_ERROR_NO_SUCH_OBJECT: return "GEN_ERROR_NO_SUCH_OBJECT";
		case GEN_ERROR_OUT_OF_MEMORY: return "GEN_ERROR_OUT_OF_MEMORY";
		case GEN_ERROR_WRONG_OBJECT_TYPE: return "GEN_ERROR_WRONG_OBJECT_TYPE";
		case GEN_ERROR_ALREADY_EXISTS: return "GEN_ERROR_ALREADY_EXISTS";
		case GEN_ERROR_OUT_OF_SPACE: return "GEN_ERROR_OUT_OF_SPACE";
		case GEN_ERROR_OUT_OF_HANDLES: return "GEN_ERROR_OUT_OF_HANDLES";
		case GEN_ERROR_TOO_SHORT: return "GEN_ERROR_TOO_SHORT";
		case GEN_ERROR_BAD_CONTENT: return "GEN_ERROR_BAD_CONTENT";
		case GEN_ERROR_BAD_OPERATION: return "GEN_ERROR_BAD_OPERATION";
		case GEN_ERROR_IN_USE: return "GEN_ERROR_IN_USE";
		case GEN_ERROR_NOT_IMPLEMENTED: return "GEN_ERROR_NOT_IMPLEMENTED";
		case GEN_ERROR_OUT_OF_BOUNDS: return "GEN_ERROR_OUT_OF_BOUNDS";
		case GEN_ERROR_INVALID_CONTROL: return "GEN_ERROR_INVALID_CONTROL";
		case GEN_ERROR_BAD_ALIGNMENT: return "GEN_ERROR_BAD_ALIGNMENT";
		case GEN_ERROR_OPERATION_FAILED: return "GEN_ERROR_OPERATION_FAILED";
		case GEN_ERROR_BAD_TIMING: return "GEN_ERROR_BAD_TIMING";
		case GEN_ERROR_TIMEOUT: return "GEN_ERROR_TIMEOUT";
        case GEN_ERROR_DOES_NOT_MATCH: return "GEN_ERROR_DOES_NOT_MATCH";
	}
}

const char* gen_error_type_description(const gen_error_type_t error) {
	switch(error) {
		case GEN_ERROR_UNKNOWN: return "An unknown error occurred";
		case GEN_ERROR_PERMISSION: return "A permission error occurred";
		case GEN_ERROR_INVALID_PARAMETER: return "The provided parameter was invalid";
		case GEN_ERROR_IO: return "An IO error occurred";
		case GEN_ERROR_TOO_LONG: return "The provided argument is too long";
		case GEN_ERROR_NO_SUCH_OBJECT: return "A nonexistent object was referenced";
		case GEN_ERROR_OUT_OF_MEMORY: return "Program ran out of usable memory";
		case GEN_ERROR_WRONG_OBJECT_TYPE: return "An object of the wrong type was provided";
		case GEN_ERROR_ALREADY_EXISTS: return "An object already exists by the provided identifier";
		case GEN_ERROR_OUT_OF_SPACE: return "The specified destination is out of space";
		case GEN_ERROR_OUT_OF_HANDLES: return "Too many platform handles are open";
		case GEN_ERROR_TOO_SHORT: return "The provided argument is too short";
		case GEN_ERROR_BAD_CONTENT: return "The provided argument contains bad or invalid content";
		case GEN_ERROR_BAD_OPERATION: return "A bad or invalid operation was requested";
		case GEN_ERROR_IN_USE: return "The specified target is in use elsewhere";
		case GEN_ERROR_NOT_IMPLEMENTED: return "An unimplemented feature was used";
		case GEN_ERROR_OUT_OF_BOUNDS: return "The specified value is out of bounds";
		case GEN_ERROR_INVALID_CONTROL: return "The program reached an invalid control path";
		case GEN_ERROR_BAD_ALIGNMENT: return "The provided data was misaligned";
		case GEN_ERROR_OPERATION_FAILED: return "An operation could not be performed";
		case GEN_ERROR_BAD_TIMING: return "An operation was performed untimely manner";
		case GEN_ERROR_TIMEOUT: return "An operation timed out";
        case GEN_ERROR_DOES_NOT_MATCH: return "The specified values did not match";
	}
}

static GEN_THREAD_LOCAL gen_error_t error_buffer = {0};

gen_error_t* gen_error_attach_backtrace(const gen_error_type_t type, const gen_size_t line, const char* const restrict string) {
	gen_error_t* retval = &error_buffer;

    error_buffer = (gen_error_t) {0};

	retval->type = type;
	retval->line = line;

    for(gen_size_t i = 0; i < GEN_ERROR_MAXIMUM_CONTEXT_LENGTH && string[i]; ++i) retval->context[i] = string[i];

	gen_error_t* error = gen_tooling_get_backtrace(GEN_NULL, &retval->backtrace_length);
	if(error) gen_error_abort_with_error(error, "generror");

	error = gen_tooling_get_backtrace(retval->backtrace, GEN_NULL);
	if(error) gen_error_abort_with_error(error, "generror");

	return retval;
}

gen_error_t* gen_error_attach_backtrace_formatted(const gen_error_type_t type, const gen_size_t line, const char* const restrict format, ...) {
	gen_error_t* retval = gen_error_attach_backtrace(type, line, "");

	gen_variadic_list_t list;
	gen_variadic_list_start(list, format);

	gen_size_t length = 0;
	gen_error_t* error = gen_string_length(format, GEN_STRING_NO_BOUNDS, GEN_STRING_NO_BOUNDS, &length);
	if(error) gen_error_abort_with_error(error, "generror");

	error = gen_string_formatv(GEN_ERROR_MAXIMUM_CONTEXT_LENGTH, retval->context, GEN_NULL, format, GEN_STRING_NO_BOUNDS, length, list);
	if(error) gen_error_abort_with_error(error, "generror");

	return retval;
}

static gen_log_level_t level_map[] = {
	[GEN_ERROR_SEVERITY_WARNING] = GEN_LOG_LEVEL_WARNING,
	[GEN_ERROR_SEVERITY_NORMAL] = GEN_LOG_LEVEL_ERROR,
	[GEN_ERROR_SEVERITY_FATAL] = GEN_LOG_LEVEL_FATAL};

void gen_error_print(const char* const restrict context, const gen_error_t* const restrict error, const gen_error_severity_t severity) {
    gen_error_t* internal_error = gen_log_formatted(level_map[severity], context, "`%t`: %t - \"%tz\" %t:%uz", gen_error_type_name(error->type), gen_error_type_description(error->type), error->context, GEN_ERROR_MAXIMUM_CONTEXT_LENGTH, error->backtrace[error->backtrace_length - 1].file, error->line);
	if(internal_error) gen_error_abort_with_error(internal_error, "generror");

	for(gen_size_t i = 0; i < error->backtrace_length; ++i) {
		gen_size_t backtrace_index = error->backtrace_length - (i + 1);
    
    	internal_error = gen_log_formatted(GEN_LOG_LEVEL_TRACE, context, "#%uz: %p %t() %t", i, error->backtrace[backtrace_index].address, error->backtrace[backtrace_index].function, error->backtrace[backtrace_index].file);
    	if(internal_error) gen_error_abort_with_error(internal_error, "generror");
	}
}

void gen_error_abort(void) {
    GEN_BACKENDS_CALL(error_abort)();
    __builtin_unreachable();
}

void gen_error_abort_with_error(const gen_error_t* const restrict error, const char* const restrict context) {
    gen_error_print(context, error, GEN_ERROR_SEVERITY_FATAL);
    gen_error_abort();
}
