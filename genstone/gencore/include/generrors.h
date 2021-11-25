// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file generrors.h
 * Contains error value enumerations
 * These error values are reported across the library so most headers transiently include this one
 */

#ifndef GEN_ERRORS_H
#define GEN_ERRORS_H

GEN_DIAG_REGION_BEGIN
GEN_DIAG_IGNORE_ALL
#include <errno.h>

#include <safe_lib_errno.h>
#include <safe_types.h>
GEN_DIAG_REGION_END

/**
 * Return values for errorable functions
 * The descriptions on `gen_error_t` enumerations are *hints*, they may be used in other contexts
 * @note Some errors will become `GEN_UNKNOWN` on platforms that do not support them or report them nontrivially
 */
typedef enum {
    /**
     * No error occurred
     */
    GEN_OK = 0,
    /**
     * An unknown error occurred
     */
    GEN_UNKNOWN,
    /**
     * A permission error occurred
     */
    GEN_PERMISSION,
    /**
     * The provided parameter was invalid
     */
    GEN_INVALID_PARAMETER,
    /**
     * An IO error occurred
     */
    GEN_IO,
    /**
     * The provided argument is too long
     */
    GEN_TOO_LONG,
    /**
     * A nonexistent object was referenced
     */
    GEN_NO_SUCH_OBJECT,
    /**
     * Program ran out of usable memory
     */
    GEN_OUT_OF_MEMORY,
    /**
     * An object of the wrong type was provided
     */
    GEN_WRONG_OBJECT_TYPE,
    /**
     * An object already exists by the provided identifier
     */
    GEN_ALREADY_EXISTS,
    /**
     * The specified destination is out of space
     */
    GEN_OUT_OF_SPACE,
    /**
     * Too many platform handles are open
     */
    GEN_OUT_OF_HANDLES,
    /**
     * The provided argument is too short
     */
    GEN_TOO_SHORT,
    /**
     * The provided argument contains bad or invalid content
     */
    GEN_BAD_CONTENT,
    /**
     * A bad or invalid operation was requested
     */
    GEN_BAD_OPERATION,
    /**
     * The specified target is in use elsewhere
     */
    GEN_IN_USE
} gen_error_t;

/**
 * Gets the name of a `gen_error_t`
 */
extern const char* gen_error_name(const gen_error_t error);

/**
 * Gets the generic description of a `gen_error_t`
 */
extern const char* gen_error_description(const gen_error_t error);

/**
 * Return value specification for functions which use `generror` for error reporting
 */
#define GEN_ERRORABLE extern __nodiscard gen_error_t

#ifndef GEN_GLOGGIFY_EH
/**
 * Whether error sites in Genstone modules are allowed to output string information via. `glog`
 * @note Depending on the error, this may contain more information than centralized error functions
 */
#define GEN_GLOGGIFY_EH ENABLED
#endif
#ifndef GEN_CENTRALIZE_EH
/**
 * Whether to define `gen_error_handler_t` and call `gen_error_handler` at error sites in Genstone modules
 * @note Creates global state if enabled. Set `gen_error_handler_passthrough` to get a passthrough pointer in `gen_error_handler`
 */
#define GEN_CENTRALIZE_EH DISABLED
#endif

#if GEN_GLOGGIFY_EH == ENABLED
#define GEN_INTERNAL_MSG_EH(error, msg) if(error != GEN_OK) glogf(ERROR, "%s: %s", gen_error_name(error), msg)
#else
/**
 * Internal handling of error messages
 */
#define GEN_INTERNAL_MSG_EH(error, msg) \
    do { \
        (void) error; \
        (void) msg; \
    } while(0)
#endif

/**
 * Dispatches error handling and aborts the program with a fatal error
 * @param error an error code
 * @param msg contextual error message
 */
#define GEN_FATAL_ERROR(error, msg) \
    do { \
        glogf(FATAL, "%s: %s", gen_error_name(error), msg); \
        GEN_DISPATCH_ERROR_HANDLER(error, msg); \
        GEN_REQUIRE_NO_REACH; \
    } while(0)

/**
 * Handler for library errors
 * @note Only used if `GEN_CENTRALIZE_EH` is `ENABLED`
 */
typedef void (*gen_error_handler_t) (const gen_error_t, const char * const restrict, const char * const restrict, const char * const restrict, int, void* const restrict);
#if GEN_CENTRALIZE_EH == ENABLED
/**
 * The currently installed error handler
 */
extern gen_error_handler_t gen_error_handler;
/**
 * Passthrough for the currently installed error handler
 */
extern void* gen_error_handler_passthrough;

#define GEN_DISPATCH_ERROR_HANDLER(error, msg) if(error != GEN_OK && gen_error_handler) gen_error_handler(error, msg, __FILE__, __func__, __LINE__, gen_error_handler_passthrough);
#else
/**
 * Runs the installed error handler if defined
 * @note Only acts if `GEN_CENTRALIZE_EH` is `ENABLED`
 */
#define GEN_DISPATCH_ERROR_HANDLER(error, msg) \
    do { \
        (void) error; \
        (void) msg; \
    } while(0)
#endif

/**
 * Errors out of a function marked `GEN_ERRORABLE`
 * Handles centralized vs. decentralized EH
 * @param error an error code
 * @param msg contextual error message
 */
#define GEN_ERROR_OUT(error, msg) \
    do { \
        GEN_INTERNAL_MSG_EH(error, msg); \
        GEN_DISPATCH_ERROR_HANDLER(error, msg); \
        return error; \
    } while(0)

/**
 * Returns from a function marked `GEN_ERRORABLE`
 */
#define GEN_ALL_OK return GEN_OK

/**
 * Errors out of a function marked `GEN_ERRORABLE` if `error` is not `GEN_OK`
 * Handles centralized vs. decentralized EH
 * @param error an error code
 * @param msg contextual error message
 */
#define GEN_ERROR_OUT_IF(error, msg) \
    do { \
        if(error != GEN_OK) { \
            GEN_INTERNAL_MSG_EH(error, msg); \
            GEN_DISPATCH_ERROR_HANDLER(error, msg); \
            return error; \
        } \
    } while(0)

/**
 * Gets the `strerror_s` of an errno value
 * @param errno the errno value to get `strerror` for
 */
#define GEN_INTERNAL_ERROR_OUT_ERRNO_GET_STRERROR(errno) \
    /* Block needs to be inline (not `do-while`'d) so we can get the declarations into the main error-out block */ \
    const size_t gen_internal_error_out_native_errno_native_strerror_len = strerrorlen_s(gen_internal_error_out_native_errno_errno); \
    const size_t gen_internal_error_out_native_errno_msg_len = sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING) + gen_internal_error_out_native_errno_native_strerror_len; \
    char gen_internal_error_out_native_errno_msg[gen_internal_error_out_native_errno_msg_len]; \
    strcpy_s(gen_internal_error_out_native_errno_msg, gen_internal_error_out_native_errno_msg_len, GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING); \
    strerror_s(gen_internal_error_out_native_errno_msg + sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING) - 1, gen_internal_error_out_native_errno_msg_len - sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING), gen_internal_error_out_native_errno_errno)

/**
 * Horrible macro string manipulation to get some nice output on your errno
 * @param proc the function which set errno
 * @param native_errno the errno value
 */
#define GEN_ERROR_OUT_ERRNO(proc, native_errno) \
    do { \
        const errno_t gen_internal_error_out_native_errno_errno = native_errno; \
        const gen_error_t gen_internal_error_out_native_errno_gen_error = gen_convert_errno(gen_internal_error_out_native_errno_errno); \
        const static char GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING[] = "`" #proc "` failed: "; \
        GEN_INTERNAL_ERROR_OUT_ERRNO_GET_STRERROR(gen_internal_error_out_native_errno_errno); \
        GEN_INTERNAL_MSG_EH(gen_internal_error_out_native_errno_gen_error, gen_internal_error_out_native_errno_msg); \
        return gen_internal_error_out_native_errno_gen_error; \
    } while(0)

/**
 * Errors out of a function marked `GEN_ERRORABLE` if `native_errno` is not `EOK` or equivalent
 * Horrible macro string manipulation to get some nice output on your errno
 * @param proc the function which set errno
 * @param native_errno the errno value
 */
#define GEN_ERROR_OUT_IF_ERRNO(proc, native_errno) \
    do { \
        if(native_errno) { \
            const errno_t gen_internal_error_out_native_errno_errno = native_errno; \
            const gen_error_t gen_internal_error_out_native_errno_gen_error = gen_convert_errno(gen_internal_error_out_native_errno_errno); \
            const static char GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING[] = "`" #proc "` failed: "; \
            GEN_INTERNAL_ERROR_OUT_ERRNO_GET_STRERROR(gen_internal_error_out_native_errno_errno); \
            GEN_INTERNAL_MSG_EH(gen_internal_error_out_native_errno_gen_error, gen_internal_error_out_native_errno_msg); \
            return gen_internal_error_out_native_errno_gen_error; \
        } \
    } while(0)

/**
 * Converts an errno into a genstone error
 * @param error the errno value to convert
 * @return the converted error enumeration
 */
GEN_ERRORABLE gen_convert_errno(errno_t error);

/**
 * Converts a win32 error into a genstone error
 * @param error the win32 error value to convert
 * @return the converted error enumeration
 */
GEN_ERRORABLE gen_convert_winerr(unsigned long error);

#endif
