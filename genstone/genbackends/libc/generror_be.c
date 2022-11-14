// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <errno.h>
#include <string.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

extern gen_error_type_t gen_error_type_from_errno(void);
gen_error_type_t gen_error_type_from_errno(void) {
	switch(errno) {
		case EACCES: return GEN_ERROR_PERMISSION;
		case EINVAL: return GEN_ERROR_INVALID_PARAMETER;
		case EIO: return GEN_ERROR_IO;
        case ESRCH: return GEN_ERROR_NO_SUCH_OBJECT;
		case ELOOP: return GEN_ERROR_TOO_LONG;
		case ENAMETOOLONG: return GEN_ERROR_TOO_LONG;
		case ENOENT: return GEN_ERROR_NO_SUCH_OBJECT;
		case ENOMEM: return GEN_ERROR_OUT_OF_MEMORY;
		case ENOTDIR: return GEN_ERROR_WRONG_OBJECT_TYPE;
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

extern const char* gen_error_description_from_errno(void);
const char* gen_error_description_from_errno(void) {
    // This is okay over `strerror_r` as the only thread-unsafe case is
    // Where the supplied `errno` value is outside of `errno`'s accepted
    // Range - which we can presume will never be relevant to us.
	return strerror(errno);
}

