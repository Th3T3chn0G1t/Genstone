// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"

#if GEN_CENTRALIZE_EH == ENABLED
thread_local gen_error_handler_t gen_error_handler = NULL;
thread_local void* gen_error_handler_passthrough = NULL;
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
		case GEN_NOT_IMPLEMENTED: return "GEN_NOT_IMPLEMENTED";
		case GEN_OUT_OF_BOUNDS: return "GEN_OUT_OF_BOUNDS";
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
		case GEN_NOT_IMPLEMENTED: return "An unimplemented feature was used";
		case GEN_OUT_OF_BOUNDS: return "The specified value is out of bounds";
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
		case EAGAIN: return GEN_BAD_OPERATION;
		case ECHILD: return GEN_NO_SUCH_OBJECT;
		default: return GEN_UNKNOWN;
	}
}
