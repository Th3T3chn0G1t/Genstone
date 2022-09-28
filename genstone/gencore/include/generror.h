// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

/**
 * @file generror.h
 * Contains error value enumerations and API for interacting with errors.
 */

#ifndef GEN_ERROR_H
#define GEN_ERROR_H

#include "gentoolingframe.h"

#ifndef EOK
#define EOK (0)
#endif

/**
 * The generic type of error encountered.
 * The descriptions on `gen_error_t` enumerations are hints, they may be used in other contexts.
 * @note Some errors will become `GEN_ERROR_UNKNOWN` on platforms that do not support them or report them nontrivially.
 */
typedef enum
{
    /**
     * An unknown error occurred.
     */
    GEN_ERROR_UNKNOWN,

    /**
     * A permission error occurred.
     */
    GEN_ERROR_PERMISSION,

    /**
     * The provided parameter was invalid.
     */
    GEN_ERROR_INVALID_PARAMETER,

    /**
     * An IO error occurred.
     */
    GEN_ERROR_IO,

    /**
     * The provided argument is too long.
     */
    GEN_ERROR_TOO_LONG,

    /**
     * A nonexistent object was referenced.
     */
    GEN_ERROR_NO_SUCH_OBJECT,

    /**
     * Program ran out of usable memory.
     */
    GEN_ERROR_OUT_OF_MEMORY,

    /**
     * An object of the wrong type was provided.
     */
    GEN_ERROR_WRONG_OBJECT_TYPE,

    /**
     * An object already exists by the provided identifier.
     */
    GEN_ERROR_ALREADY_EXISTS,

    /**
     * The specified destination is out of space.
     */
    GEN_ERROR_OUT_OF_SPACE,

    /**
     * Too many platform handles are open.
     */
    GEN_ERROR_OUT_OF_HANDLES,

    /**
     * The provided argument is too short.
     */
    GEN_ERROR_TOO_SHORT,

    /**
     * The provided argument contains bad or invalid content.
     */
    GEN_ERROR_BAD_CONTENT,

    /**
     * A bad or invalid operation was requested.
     */
    GEN_ERROR_BAD_OPERATION,

    /**
     * The specified target is in use elsewhere.
     */
    GEN_ERROR_IN_USE,

    /**
     * An unimplemented feature was used.
     */
    GEN_ERROR_NOT_IMPLEMENTED,

    /**
     * The specified value is out of bounds.
     */
    GEN_ERROR_OUT_OF_BOUNDS,

    /**
     * The program reached an invalid control path.
     */
    GEN_ERROR_INVALID_CONTROL,

    /**
     * The provided data was misaligned.
     */
    GEN_ERROR_BAD_ALIGNMENT,

    /**
     * An operation could not be performed.
     */
    GEN_ERROR_OPERATION_FAILED,

    /**
     * An operation was performed untimely manner.
     */
    GEN_ERROR_BAD_TIMING,

    /**
     * An operation timed out.
     */
    GEN_ERROR_TIMEOUT
} gen_error_type_t;

/**
 * Severity levels for an error.
 */
typedef enum {
    /**
     * A non-failure case issue.
     */
    GEN_ERROR_SEVERITY_WARNING,

    /**
     * A failure case issue.
     */
    GEN_ERROR_SEVERITY_NORMAL,

    /**
     * A failure case issue causing program termination.
     */
    GEN_ERROR_SEVERITY_FATAL
} gen_error_severity_t;

#ifndef GEN_ERROR_MAXIMUM_CONTEXT_LENGTH
/**
 * The maximum length for an error context string.
 */
#define GEN_ERROR_MAXIMUM_CONTEXT_LENGTH 2048
#endif

/**
 * The return value for an errorable function.
 */
typedef struct {
    /**
     * The generic type of error encountered.
     */
    gen_error_type_t type;

    /**
     * The line number at which the error occurred.
     */
    size_t line;

    /**
     * A string explaining the context behind an error.
     */
    char context[GEN_ERROR_MAXIMUM_CONTEXT_LENGTH + 1];

    /**
     * An array of tooling frames forming a backtrace.
     */
    gen_tooling_frame_t backtrace[GEN_TOOLING_DEPTH];

    /**
     * The length of the backtrace.
     */
    size_t backtrace_length;
} gen_error_t;

/**
 * Gets the name of a `gen_error_type_t`.
 * @param[in] error The error type to get the name of.
 * @return The name of the error.
 */
extern const char* gen_error_type_name(const gen_error_type_t error);

/**
 * Gets the generic description of a `gen_error_type_t`.
 * @param[in] error The error type to get the description of.
 * @return A description of the error.
 */
extern const char* gen_error_type_description(const gen_error_type_t error);

/**
 * Converts the value of errno into a genstone error type.
 * @return The converted error type.
 */
extern gen_error_type_t gen_error_type_from_errno(void);

/**
 * Converts the value of errno into a string description.
 * @return The converted error description.
 */
const char* gen_error_description_from_errno(void);

/**
 * Constructs an error and attaches a backtrace down to the caller.
 * @param[in] type The generic type of the error to construct.
 * @param[in] line The line number at which the error occurred.
 * @param[in] context The context behind the error.
 * @return The constructed error.
 */
extern gen_error_t* gen_error_attach_backtrace(const gen_error_type_t type, const size_t line, const char* const restrict context);

/**
 * Constructs an error and attaches a backtrace down to the caller.
 * Creates the context from a format string.
 * @param[in] type The generic type of the error to construct.
 * @param[in] line The line number at which the error occurred.
 * @param[in] format The format string from which to create the context string.
 * @param[in] ... The parameters to the format string.
 * @return The constructed error.
 */
extern gen_error_t* gen_error_attach_backtrace_formatted(const gen_error_type_t type, const size_t line, const char* const restrict format, ...);

/**
 * Prints out an errors details and backtrace.
 * @param[in] context The context/location in which the error occurred.
 * @param[in] error The error to print.
 * @param[in] severity The severity of the error.
 */
extern void gen_error_print(const char* const restrict context, const gen_error_t* const restrict error, const gen_error_severity_t severity);

/**
 * Frees a backpropogated error.
 * @param[in] error A pointer to the error pointer to free.
 */
extern void gen_error_free(gen_error_t* restrict * const restrict error);

/**
 * Aborts the program.
 */
extern GEN_NORETURN void gen_error_abort(void);

#endif
