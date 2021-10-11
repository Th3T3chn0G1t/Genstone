// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#if PLATFORM == WIN
#include <Windows.h>
#endif
#include "include/gencommon.h"

#if GEN_CENTRALIZE_EH == ENABLED
gen_error_handler_t gen_error_handler = NULL;
void* gen_error_handler_passthrough = NULL;
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
	}
}

GEN_ERRORABLE_RETURN gen_convert_errno(errno_t error) {
	switch(error) {
		case EACCES: return GEN_PERMISSION;
		case EINVAL: return GEN_INVALID_PARAMETER;
		case EIO: return GEN_IO;
		case ELOOP: return GEN_TOO_LONG;
		case ENAMETOOLONG: return GEN_TOO_LONG;
		case ENOENT: return GEN_NO_SUCH_OBJECT;
		case ENOMEM: return GEN_OUT_OF_MEMORY;
		case ENOTDIR: return GEN_WRONG_OBJECT_TYPE;
#if PLATFORM != WIN // Bit random to be missing but okay
		case EDQUOT: return GEN_OUT_OF_SPACE;
#endif
		case EEXIST: return GEN_ALREADY_EXISTS;
		case EMLINK: return GEN_TOO_LONG;
		case ENOSPC: return GEN_OUT_OF_SPACE;
		case EPERM: return GEN_PERMISSION;
		case EROFS: return GEN_PERMISSION;
		case EMFILE: return GEN_OUT_OF_HANDLES;
		case ENFILE: return GEN_OUT_OF_HANDLES;
		case EBADF: return GEN_INVALID_PARAMETER;
		default: return GEN_UNKNOWN;
	}
}

GEN_ERRORABLE_RETURN gen_convert_winerr(unsigned long error) {
#if PLATFORM == WIN
	switch(error) {
		case ERROR_ALREADY_EXISTS: return GEN_ALREADY_EXISTS;
		case ERROR_PATH_NOT_FOUND: return GEN_NO_SUCH_OBJECT;
		default: return GEN_UNKNOWN;
	}
#else
	(void) error;
	glog(WARNING, "Calling gen_convert_winerr() on non-Windows platform");
	return GEN_UNKNOWN;
#endif
}
