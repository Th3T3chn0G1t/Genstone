// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"

#if GEN_CENTRALIZE_EH == ENABLED
gen_error_handler_t gen_error_handler = NULL;
void* gen_error_handler_passthrough = NULL;
#endif

#ifndef GEN_FATAL_ANNEXK_CONSTRAINTS
/**
 * Whether the Genstone-installed Annex K constraint handler should trigger a fatal error and abort the program
 */
#define GEN_FATAL_ANNEXK_CONSTRAINTS ENABLED

GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wmissing-noreturn"
static void gen_internal_annexk_constraint_callback(const char* restrict msg, __unused void* restrict passthrough, errno_t error) {
	gen_error_t gen_error = gen_convert_errno(error);

	GEN_DISPATCH_ERROR_HANDLER(gen_error, "Annex K constraint handler was tripped");
#if GEN_GLOGGIFY_EH == ENABLED
#if GEN_FATAL_ANNEXK_CONSTRAINTS == ENABLED
	glogf(FATAL, "Annex K constraint handler was tripped: %s: %s" GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) GEN_ANSI_COLOR_LIGHT(GEN_ANSI_RED) "\nReason:" GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR) " %s", gen_error_name(gen_error), gen_error_description(gen_error), msg);
#else
	glogf(ERROR, "Annex K constraint handler was tripped: %s: %s" GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) GEN_ANSI_COLOR_LIGHT(GEN_ANSI_RED) "\nReason:" GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR) " %s", gen_error_name(gen_error), gen_error_description(gen_error), msg);
#endif
#else
#if GEN_FATAL_ANNEXK_CONSTRAINTS == ENABLED
	GEN_REQUIRE_NO_REACH;
#endif
#endif
}
GEN_DIAG_REGION_END

__attribute__((constructor)) static void gen_internal_initialize_annexk_constraint_callback(void) {
	set_mem_constraint_handler_s(gen_internal_annexk_constraint_callback);
	set_str_constraint_handler_s(gen_internal_annexk_constraint_callback);
}
#endif

const char* gen_error_name(const gen_error_t error) {
	switch(error) {
		case GEN_OK: return "GEN_OK";
		case GEN_UNKNOWN: return "GEN_UNKNOWN";
		case GEN_PERMISSION: return "GEN_PERMISSION";
		case GEN_INVALID_PARAMETER: return "GEN_INVALID_PARAMETER";
		case GEN_IO: return "GEN_IO";
		case GEN_TOO_LONG: return "GEN_TOO_LONG";
		case GEN_NO_SUCH_OBJECT: return "GEN_NO_SUCH_OBJECT";
		case GEN_OUT_OF_MEMORY: return "GEN_OUT_OF_MEMORY";
		case GEN_WRONG_OBJECT_TYPE: return "GEN_WRONG_OBJECT_TYPE";
		case GEN_ALREADY_EXISTS: return "GEN_ALREADY_EXISTS";
		case GEN_OUT_OF_SPACE: return "GEN_OUT_OF_SPACE";
		case GEN_OUT_OF_HANDLES: return "GEN_OUT_OF_HANDLES";
		case GEN_TOO_SHORT: return "GEN_TOO_SHORT";
		case GEN_BAD_CONTENT: return "GEN_BAD_CONTENT";
		case GEN_BAD_OPERATION: return "GEN_BAD_OPERATION";
		case GEN_IN_USE: return "GEN_IN_USE";
	}
}

const char* gen_error_description(const gen_error_t error) {
	switch(error) {
		case GEN_OK: return "No error occurred";
		case GEN_UNKNOWN: return "An unknown error occurred";
		case GEN_PERMISSION: return "A permission error occurred";
		case GEN_INVALID_PARAMETER: return "The provided parameter was invalid";
		case GEN_IO: return "An IO error occurred";
		case GEN_TOO_LONG: return "The provided argument is too long";
		case GEN_NO_SUCH_OBJECT: return "A nonexistent object was referenced";
		case GEN_OUT_OF_MEMORY: return "Program ran out of usable memory";
		case GEN_WRONG_OBJECT_TYPE: return "An object of the wrong type was provided";
		case GEN_ALREADY_EXISTS: return "An object already exists by the provided identifier";
		case GEN_OUT_OF_SPACE: return "The specified destination is out of space";
		case GEN_OUT_OF_HANDLES: return "Too many platform handles are open";
		case GEN_TOO_SHORT: return "The provided argument is too short";
		case GEN_BAD_CONTENT: return "The provided argument contains bad or invalid content";
		case GEN_BAD_OPERATION: return "A bad or invalid operation was requested";
		case GEN_IN_USE: return "The specified target is in use elsewhere";
	}
}

gen_error_t gen_convert_errno(errno_t error) {
	switch(error) {
		case EACCES: return GEN_PERMISSION;
		case EINVAL: return GEN_INVALID_PARAMETER;
		case EIO: return GEN_IO;
		case ELOOP: return GEN_TOO_LONG;
		case ENAMETOOLONG: return GEN_TOO_LONG;
		case ENOENT: return GEN_NO_SUCH_OBJECT;
		case ENOMEM: return GEN_OUT_OF_MEMORY;
		case ENOTDIR: return GEN_WRONG_OBJECT_TYPE;
		case EOK: return GEN_OK;
		case EDQUOT: return GEN_OUT_OF_SPACE;
		case EEXIST: return GEN_ALREADY_EXISTS;
		case EMLINK: return GEN_TOO_LONG;
		case ENOSPC: return GEN_OUT_OF_SPACE;
		case EPERM: return GEN_PERMISSION;
		case EROFS: return GEN_PERMISSION;
		case EMFILE: return GEN_OUT_OF_HANDLES;
		case ENFILE: return GEN_OUT_OF_HANDLES;
		case EBADF: return GEN_INVALID_PARAMETER;
		case EBUSY: return GEN_IN_USE;
		case EFAULT: return GEN_BAD_CONTENT;
		case EISDIR: return GEN_WRONG_OBJECT_TYPE;
		case ESNULLP: return GEN_INVALID_PARAMETER;
		case ESZEROL: return GEN_TOO_SHORT;
		case ESLEMIN: return GEN_TOO_SHORT;
		case ESLEMAX: return GEN_TOO_LONG;
		case ESOVRLP: return GEN_BAD_OPERATION;
		case ESEMPTY: return GEN_TOO_SHORT;
		case ESNOSPC: return GEN_OUT_OF_SPACE;
		case ESUNTERM: return GEN_TOO_LONG;
		case ESNODIFF: return GEN_BAD_OPERATION;
		case ESNOTFND: return GEN_NO_SUCH_OBJECT;
		case ESLEWRNG: return GEN_INVALID_PARAMETER;
		case EAGAIN: return GEN_BAD_OPERATION;
		default: return GEN_UNKNOWN;
	}
}
