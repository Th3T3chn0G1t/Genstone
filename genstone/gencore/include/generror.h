// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file generrors.h
 * Contains error value enumerations and API for interacting with errors.
 */

#ifndef GEN_ERRORS_H
#define GEN_ERRORS_H

/**
 * The generic type of error encountered.
 * The descriptions on `gen_error_t` enumerations are hints, they may be used in other contexts.
 * @note Some errors will become `GEN_UNKNOWN` on platforms that do not support them or report them nontrivially.
 */
typedef enum
{
	/**
     * No error occurred.
     */
	GEN_OK = 0,
	/**
     * An unknown error occurred.
     */
	GEN_UNKNOWN,
	/**
     * A permission error occurred.
     */
	GEN_PERMISSION,
	/**
     * The provided parameter was invalid.
     */
	GEN_INVALID_PARAMETER,
	/**
     * An IO error occurred.
     */
	GEN_IO,
	/**
     * The provided argument is too long.
     */
	GEN_TOO_LONG,
	/**
     * A nonexistent object was referenced.
     */
	GEN_NO_SUCH_OBJECT,
	/**
     * Program ran out of usable memory.
     */
	GEN_OUT_OF_MEMORY,
	/**
     * An object of the wrong type was provided.
     */
	GEN_WRONG_OBJECT_TYPE,
	/**
     * An object already exists by the provided identifier.
     */
	GEN_ALREADY_EXISTS,
	/**
     * The specified destination is out of space.
     */
	GEN_OUT_OF_SPACE,
	/**
     * Too many platform handles are open.
     */
	GEN_OUT_OF_HANDLES,
	/**
     * The provided argument is too short.
     */
	GEN_TOO_SHORT,
	/**
     * The provided argument contains bad or invalid content.
     */
	GEN_BAD_CONTENT,
	/**
     * A bad or invalid operation was requested.
     */
	GEN_BAD_OPERATION,
	/**
     * The specified target is in use elsewhere.
     */
	GEN_IN_USE,
	/**
	 * An unimplemented feature was used.
	 */
	GEN_NOT_IMPLEMENTED,
	/**
	 * The specified value is out of bounds.
	 */
	GEN_OUT_OF_BOUNDS,
	/**
	 * The program reached an invalid control path.
	 */
	GEN_INVALID_CONTROL,
	/**
	 * The provided data was misaligned.
	 */
	GEN_BAD_ALIGNMENT,
	/**
	 * An operation could not be performed.
	 */
	GEN_OPERATION_FAILED,
	/**
	 * An operation was performed untimely manner.
	 */
	GEN_BAD_TIMING,
	/**
	 * An operation timed out.
	 */
	GEN_TIMEOUT
} gen_error_type_t;

/**
 * Severity levels for an error.
 */
typedef enum {
	GEN_ERROR_SEVERITY_WARNING,
	GEN_ERROR_SEVERITY_NORMAL,
	GEN_ERROR_SEVERITY_FATAL
} gen_error_severity_t;

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
	const char* context;
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
extern gen_error_t gen_error_attach_backtrace(const gen_error_type_t type, const size_t line, const char* const restrict context);

/**
 * Constructs an error and attaches a backtrace down to the caller.
 * Creates the context from a format string.
 * @param[in] type The generic type of the error to construct.
 * @param[in] line The line number at which the error occurred.
 * @param[in] format The format string from which to create the context string.
 * @param[in] ... The parameters to the format string.
 * @return The constructed error.
 */
extern gen_error_t gen_error_attach_backtrace_formatted(const gen_error_type_t type, const size_t line, const char* const restrict format, ...);

/**
 * Prints out an errors details and backtrace.
 * @param[in] error The error to print.
 * @param[in] severity The severity of the error.
 */
extern void gen_error_print(const gen_error_t* const restrict error, const gen_error_severity_t severity);

/**
 * Aborts the program.
 */
extern GEN_NORETURN void gen_error_abort(void);

#endif
