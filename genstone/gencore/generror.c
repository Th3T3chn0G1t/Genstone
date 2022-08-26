// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"
#include "include/genmemory.h"
#include "include/genstring.h"
#include "include/genlog.h"

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <errno.h>
#include <stdlib.h>
#include <string.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

const char* gen_error_type_name(const gen_error_type_t error) {
	switch(error) {
		case GEN_OK: return "GEN_OK";
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
	}
}

const char* gen_error_type_description(const gen_error_type_t error) {
	switch(error) {
		case GEN_OK: return "No error occurred";
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
	}
}

gen_error_type_t gen_error_type_from_errno(void) {
	switch(errno) {
		case EACCES: return GEN_ERROR_PERMISSION;
		case EINVAL: return GEN_ERROR_INVALID_PARAMETER;
		case EIO: return GEN_ERROR_IO;
		case ELOOP: return GEN_ERROR_TOO_LONG;
		case ENAMETOOLONG: return GEN_ERROR_TOO_LONG;
		case ENOENT: return GEN_ERROR_NO_SUCH_OBJECT;
		case ENOMEM: return GEN_ERROR_OUT_OF_MEMORY;
		case ENOTDIR: return GEN_ERROR_WRONG_OBJECT_TYPE;
		case EOK: return GEN_OK;
		case EDQUOT: return GEN_ERROR_OUT_OF_SPACE;
		case EEXIST: return GEN_ERROR_ALREADY_EXISTS;
		case EMLINK: return GEN_ERROR_TOO_LONG;
		case ENOSPC: return GEN_ERROR_OUT_OF_SPACE;
		case EPERM: return GEN_ERROR_PERMISSION;
		case EROFS: return GEN_ERROR_PERMISSION;
		case EMFILE: return GEN_ERROR_OUT_OF_HANDLES;
		case ENFILE: return GEN_ERROR_OUT_OF_HANDLES;
		case EBADF: return GEN_ERROR_INVALID_PARAMETER;
		case EBUSY: return GEN_ERROR_IN_USE;
		case EFAULT: return GEN_ERROR_BAD_CONTENT;
		case EISDIR: return GEN_ERROR_WRONG_OBJECT_TYPE;
		case EAGAIN: return GEN_ERROR_BAD_OPERATION;
		case ECHILD: return GEN_ERROR_NO_SUCH_OBJECT;
		default: return GEN_ERROR_UNKNOWN;
	}
}

const char* gen_error_description_from_errno(void) {
	return strerror(errno); // This is okay over `strerror_r` as the only thread-unsafe case is
		// Where the supplied `errno` value is outside of `errno`'s accepted
		// Range - which we can presume will never be relevant to us
}

gen_error_t gen_error_attach_backtrace(const gen_error_type_t type, const size_t line, const char* const restrict string) {
	gen_error_t retval = {type, line, NULL, NULL, 0};

	gen_error_t error = gen_string_duplicate(string, GEN_STRING_NO_BOUNDS, GEN_STRING_NO_BOUNDS, &retval.context, NULL);
	if(error.type) {
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}

	error = gen_tooling_get_backtrace(NULL, &retval.backtrace_length);
	if(error.type) {
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
	error = gen_memory_allocate_zeroed((void**) &retval.backtrace, retval.backtrace_length, sizeof(gen_tooling_frame_t));
	if(error.type) {
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
	error = gen_tooling_get_backtrace(retval.backtrace, NULL);
	if(error.type) {
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}

	return retval;
}

gen_error_t gen_error_attach_backtrace_formatted(const gen_error_type_t type, const size_t line, const char* const restrict format, ...) {
	gen_error_t retval = {0};

	va_list list;
	va_start(list, format);
	va_list list_copy;
	va_copy(list_copy, list);

	size_t formatted_length = 0;
	gen_error_t error = gen_string_format(NULL, &formatted_length, format, list);
	if(error.type) {
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}

	char* formatted = NULL;
	error = gen_memory_allocate_zeroed((void**) &formatted, formatted_length + 1, sizeof(char));
	if(error.type) {
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}

	error = gen_string_format(formatted, NULL, format, list_copy);
	if(error.type) {
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}

	error = gen_error_attach_backtrace(type, line, formatted);
	if(error.type) {
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}

	error = gen_memory_free((void**) &formatted);
	if(error.type) {
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}

	return retval;
}

#ifndef GEN_ERROR_ABORT_FUNCTION
/**
 * The function to call to exit the program in the case of a fatal error.
 */
#define GEN_ERROR_ABORT_FUNCTION abort
#endif

void gen_error_abort(void) {
	GEN_ERROR_ABORT_FUNCTION();
}
