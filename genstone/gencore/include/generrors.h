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

#if PLATFORM != WIN
#include <safe_lib_errno.h>
#include <safe_types.h>
#endif
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
#define GEN_ERRORABLE_RETURN __nodiscard gen_error_t

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
 * Errors out of a function marked `GEN_ERRORABLE_RETURN`
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
 * Errors out of a function marked `GEN_ERRORABLE_RETURN` if `error` is not `GEN_OK`
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

#ifndef GEN_INTERNAL_FUCK_YOU_MICROSOFT_MAX_STRERROR
/**
 * It was warranted
 * Windows CRT asks for `strerror_s` to be used in place of `strerror` but doesn't provide `strerrorlen_s`
 * We use this to allocate a fixed size buffer to store `strerror_s` values
 */
#define GEN_INTERNAL_FUCK_YOU_MICROSOFT_MAX_STRERROR 100
#endif

#if PLATFORM == WIN
#define GEN_INTERNAL_ERROR_OUT_ERRNO_GET_STRERROR(errno) \
    char gen_internal_error_out_native_errno_native_strerror[GEN_INTERNAL_FUCK_YOU_MICROSOFT_MAX_STRERROR]; \
    strerror_s(gen_internal_error_out_native_errno_native_strerror, GEN_INTERNAL_FUCK_YOU_MICROSOFT_MAX_STRERROR, errno); \
    const size_t gen_internal_error_out_native_errno_native_strerror_len = strnlen_s(gen_internal_error_out_native_errno_native_strerror, GEN_INTERNAL_FUCK_YOU_MICROSOFT_MAX_STRERROR); \
    const size_t gen_internal_error_out_native_errno_msg_len = sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING) + gen_internal_error_out_native_errno_native_strerror_len; \
    char gen_internal_error_out_native_errno_msg[gen_internal_error_out_native_errno_msg_len]; \
    strcpy_s(gen_internal_error_out_native_errno_msg, gen_internal_error_out_native_errno_msg_len, GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING); \
    strcpy_s(gen_internal_error_out_native_errno_msg + sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING), gen_internal_error_out_native_errno_msg_len - sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING), gen_internal_error_out_native_errno_native_strerror)
#else
/**
 * Windows doesn't support `strerrorlen_s`
 * End me quickly
 * Gets the `strerror` of an errno value in the most platform-secure way possible
 * @param errno the errno value to get `strerror` for
 */
#define GEN_INTERNAL_ERROR_OUT_ERRNO_GET_STRERROR(errno) \
    /* Block needs to be inline (not `do-while`'d) so we can get the declarations into the main error-out block */ \
    const size_t gen_internal_error_out_native_errno_native_strerror_len = strerrorlen_s(gen_internal_error_out_native_errno_errno); \
    const size_t gen_internal_error_out_native_errno_msg_len = sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING) + gen_internal_error_out_native_errno_native_strerror_len; \
    char gen_internal_error_out_native_errno_msg[gen_internal_error_out_native_errno_msg_len]; \
    strcpy_s(gen_internal_error_out_native_errno_msg, gen_internal_error_out_native_errno_msg_len, GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING); \
    strerror_s(gen_internal_error_out_native_errno_msg + sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING), gen_internal_error_out_native_errno_msg_len - sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING), gen_internal_error_out_native_errno_errno)
#endif

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
 * Errors out of a function marked `GEN_ERRORABLE_RETURN` if `native_errno` is not `EOK` or equivalent
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
 * Horrible macro string manipulation to get some nice output on your winerror
 * @param proc the function which set winerr
 * @param native_errno the winerr value
 */
#define GEN_ERROR_OUT_WINERR(proc, native_errno) \
    do { \
        const unsigned long gen_internal_error_out_native_errno_errno = native_errno; \
        const gen_error_t gen_internal_error_out_native_errno_gen_error = gen_convert_winerr(gen_internal_error_out_native_errno_errno); \
        const static char GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING[] = "`" #proc "` failed: "; \
        size_t gen_internal_error_out_native_errno_native_strerror_len = 0; \
        gen_winerr_as_string(NULL, &gen_internal_error_out_native_errno_native_strerror_len, gen_internal_error_out_native_errno_errno); \
        const size_t gen_internal_error_out_native_errno_msg_len = sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING) + gen_internal_error_out_native_errno_native_strerror_len; \
        char gen_internal_error_out_native_errno_msg[gen_internal_error_out_native_errno_msg_len]; \
        strcpy_s(gen_internal_error_out_native_errno_msg, gen_internal_error_out_native_errno_msg_len, GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING); \
        gen_winerr_as_string(gen_internal_error_out_native_errno_msg + sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING), NULL, gen_internal_error_out_native_errno_errno); \
        GEN_INTERNAL_MSG_EH(gen_internal_error_out_native_errno_gen_error, gen_internal_error_out_native_errno_msg); \
        return gen_internal_error_out_native_errno_gen_error; \
    } while(0)

/**
 * Errors out of a function marked `GEN_ERRORABLE_RETURN` if `native_errno` is not 0
 * Horrible macro string manipulation to get some nice output on your winerror
 * @param proc the function which set winerr
 * @param native_errno the winerr value
 */
#define GEN_ERROR_OUT_IF_WINERR(proc, native_errno) \
    do { \
        if(native_errno) { \
            const unsigned long gen_internal_error_out_native_errno_errno = native_errno; \
            const gen_error_t gen_internal_error_out_native_errno_gen_error = gen_convert_winerr(gen_internal_error_out_native_errno_errno); \
            const static char GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING[] = "`" #proc "` failed: "; \
            size_t gen_internal_error_out_native_errno_native_strerror_len = 0; \
            gen_winerr_as_string(NULL, &gen_internal_error_out_native_errno_native_strerror_len, gen_internal_error_out_native_errno_errno); \
            const size_t gen_internal_error_out_native_errno_msg_len = sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING) + gen_internal_error_out_native_errno_native_strerror_len; \
            char gen_internal_error_out_native_errno_msg[gen_internal_error_out_native_errno_msg_len]; \
            strcpy_s(gen_internal_error_out_native_errno_msg, gen_internal_error_out_native_errno_msg_len, GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING); \
            gen_winerr_as_string(gen_internal_error_out_native_errno_msg + sizeof(GEN_INTERNAL_ERROR_OUT_NATIVE_ERRNO_BASESTRING), NULL, gen_internal_error_out_native_errno_errno); \
            GEN_INTERNAL_MSG_EH(gen_internal_error_out_native_errno_gen_error, gen_internal_error_out_native_errno_msg); \
            return gen_internal_error_out_native_errno_gen_error; \
        } \
    } while(0)

/**
 * Converts an errno into a genstone error
 * @param error the errno value to convert
 * @return the converted error enumeration
 */
extern GEN_ERRORABLE_RETURN gen_convert_errno(errno_t error);

/**
 * Converts a win32 error into a genstone error
 * @param error the win32 error value to convert
 * @return the converted error enumeration
 */
extern GEN_ERRORABLE_RETURN gen_convert_winerr(unsigned long error);

#endif
