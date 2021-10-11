// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @file generrors.h
 * Contains error value enumerations
 * These error values are reported across the library so most headers transiently include this one
 */

#ifndef GEN_ERRORS_H
#define GEN_ERRORS_H

#include <errno.h>
#ifndef __STDC_LIB_EXT1__
typedef int errno_t;
#endif

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
     * An invalid parameter was passed
     */
    GEN_INVALID_PARAMETER,
    /**
     * An IO error occurred
     */
    GEN_IO,
    /**
     * A provided argument is too long
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
     * An object of the wrong type was passed
     */
    GEN_WRONG_OBJECT_TYPE,
    /**
     * An object already exists by a passed identifier
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
     * A provided argument is too short
     */
    GEN_TOO_SHORT,
    /**
     * The provided argument contains bad or invalid content
     */
    GEN_BAD_CONTENT
} gen_error_t;

/**
 * Gets the name of a `gen_error_t`
 */
extern const char* gen_error_name(const gen_error_t error);

/**
 * Return value specification for functions which use `generror` for error reporting
 */
#define GEN_ERRORABLE_RETURN __nodiscard gen_error_t

#ifndef GEN_GLOGGIFY_EH
#define GEN_GLOGGIFY_EH ENABLED
#endif
#ifndef GEN_CENTRALIZE_EH
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

#if GEN_CENTRALIZE_EH == ENABLED
typedef void (*gen_error_handler_t) (const gen_error_t, const char * const restrict, const char * const restrict, const char * const restrict, int, void* const restrict)
extern gen_error_handler_t gen_error_handler;
extern void* gen_error_handler_passthrough;

#define GEN_ERROR_OUT(error, msg) \
    do { \
        GEN_INTERNAL_MSG_EH(error, msg); \
        if(error != GEN_OK) if(gen_error_handler) gen_error_handler(error, msg, __FILE__, __func__, __LINE__, gen_error_handler_passthrough); \
        return error; \
    } while(0)
#else
/**
 * Errors out of a function marked `GEN_ERRORABLE_RETURN`
 * Handles centralized vs. decentralized EH
 */
#define GEN_ERROR_OUT(error, msg) \
    do { \
        GEN_INTERNAL_MSG_EH(error, msg); \
        return error; \
    } while(0)
#endif

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
