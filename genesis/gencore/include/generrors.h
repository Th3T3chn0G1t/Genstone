// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @file generrors.h
 * Contains error value enumerations
 * These error values are reported across the library so most headers transiently include this one
 * But it is still recommended to explicitly include
 */

#ifndef GEN_ERRORS_H
#define GEN_ERRORS_H

/**
 * Return values for errorable functions
 * @note Some errors will become GEN_UNKNOWN on platforms that do not support them or report them nontrivially
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
    GEN_TOO_SHORT
} gen_error_t;

/**
 * Converts an errno into a genesis error
 * @param error the errno value to convert
 * @return the converted error enumeration
 */
extern gen_error_t gen_convert_errno(int error);

/**
 * Converts a win32 error into a genesis error
 * @param error the win32 error value to convert
 * @return the converted error enumeration
 */
extern gen_error_t gen_convert_winerr(int error);

#endif
