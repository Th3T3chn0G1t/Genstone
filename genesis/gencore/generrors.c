#include "include/generrors.h"

#if PLATFORM == WIN
#include <windows.h>
#endif
#include <errno.h>

gen_error_t gen_convert_errno(int error) {
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

gen_error_t gen_convert_winerr(int error) {
#if PLATFORM == WIN
    switch(error) {
        case ERROR_ALREADY_EXISTS: return GEN_ALREADY_EXISTS;
        case ERROR_PATH_NOT_FOUND: return GEN_NO_SUCH_OBJECT;
        default: return GEN_UNKNOWN;
    }
#else
    return GEN_UNKNOWN;
#endif
}

