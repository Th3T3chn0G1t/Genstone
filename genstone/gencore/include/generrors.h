// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file generrors.h
 * Contains error value enumerations.
 * These error values are reported across the library so most headers transiently include this one.
 */

#ifndef GEN_ERRORS_H
#define GEN_ERRORS_H

GEN_DIAG_REGION_BEGIN
GEN_DIAG_IGNORE_ALL
#include <errno.h>
GEN_DIAG_REGION_END

#ifndef EOK
#define EOK (0)
#endif

typedef int errno_t;

/**
 * Return values for errorable functions.
 * The descriptions on `gen_error_t` enumerations are *hints*, they may be used in other contexts.
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
	 * An unimplemented feature was used
	 */
	GEN_NOT_IMPLEMENTED,
	/**
	 * The specified value is out of bounds
	 */
	GEN_OUT_OF_BOUNDS
} gen_error_t;

/**
 * Gets the name of a `gen_error_t`.
 */
extern const char* gen_error_name(const gen_error_t error);
/**
 * @example{lineno} example/gencore/gen_error_name.c
 * Example for how to use `gen_error_name`.
 * `gen_error_name` is used for getting the name of a Genstone error.
 */

/**
 * Gets the generic description of a `gen_error_t`.
 */
extern const char* gen_error_description(const gen_error_t error);
/**
 * @example{lineno} example/gencore/gen_error_description.c
 * Example for how to use `gen_error_description`.
 * `gen_error_description` is used for getting the description of a Genstone error.
 */

/**
 * Return value specification for functions which use `generror` for error reporting.
 */
#define GEN_ERRORABLE extern __nodiscard gen_error_t

#ifndef GEN_GLOGGIFY_EH
/**
 * Whether error sites in Genstone modules are allowed to output string information via. `glog`.
 * @note Depending on the error, this may contain more information than centralized error functions.
 */
#define GEN_GLOGGIFY_EH ENABLED
#endif

#ifndef GEN_CENTRALIZE_EH

/**
 * Whether to define `gen_error_handler_t` and call `gen_error_handler` at error sites in Genstone modules.
 * @note Creates global state if enabled. Set `gen_error_handler_passthrough` to get a passthrough pointer in `gen_error_handler`.
 */
#define GEN_CENTRALIZE_EH DISABLED
#endif

#if GEN_GLOGGIFY_EH == ENABLED
#define GEN_INTERNAL_MSG_EH(error, msg) \
	if(error != GEN_OK) glogf(ERROR, "%s: %s at %s:%i", gen_error_name(error), msg, __FILE__, __LINE__)
#else
/**
 * Internal handling of error messages.
 */
#define GEN_INTERNAL_MSG_EH(error, msg) \
	do { \
		(void) error; \
		(void) msg; \
	} while(0)
#endif

/**
 * Aborts the program with a fatal error.
 * Avoids `gtrace`.
 * Avoids triggering error hander.
 * @param[in] error an error code.
 * @param[in] msg contextual error message.
 */
#define GEN_FATAL_ERROR(error, msg) \
	do { \
		glogf(ERROR, "%s: %s", gen_error_name(error), msg); \
		abort(); \
	} while(0)

/**
 * Handler for library errors.
 * @note Only used if `GEN_CENTRALIZE_EH` is `ENABLED`.
 */
typedef void (*gen_error_handler_t)(const gen_error_t, const char* const restrict, const char* const restrict, const char* const restrict, int, void* const restrict);
#if GEN_CENTRALIZE_EH == ENABLED
/**
 * The currently installed error handler.
 */
extern gen_error_handler_t gen_error_handler;
/**
 * Passthrough for the currently installed error handler.
 */
extern void* gen_error_handler_passthrough;

#define GEN_DISPATCH_ERROR_HANDLER(error, msg) \
	if(error != GEN_OK && gen_error_handler) gen_error_handler(error, msg, __FILE__, __func__, __LINE__, gen_error_handler_passthrough);
#else
/**
 * Runs the installed error handler if defined.
 * @note Only acts if `GEN_CENTRALIZE_EH` is `ENABLED`.
 */
#define GEN_DISPATCH_ERROR_HANDLER(error, msg) \
	do { \
		(void) error; \
		(void) msg; \
	} while(0)
#endif

/**
 * Errors out of a function marked `GEN_ERRORABLE`.
 * Handles centralized vs. decentralized EH.
 * @param[in] error an error code.
 * @param[in] msg contextual error message.
 */
#define GEN_ERROR_OUT(error, msg) \
	do { \
		GEN_INTERNAL_MSG_EH(error, msg); \
		GEN_DISPATCH_ERROR_HANDLER(error, msg); \
		return error; \
	} while(0)

/**
 * Returns from a function marked `GEN_ERRORABLE`.
 */
#define GEN_ALL_OK return GEN_OK

/**
 * Errors out of a function marked `GEN_ERRORABLE` if `error` is not `GEN_OK`.
 * Handles centralized vs. decentralized EH.
 * @param[in] error an error code.
 * @param[in] msg contextual error message.
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
 * Errors out of a function marked `GEN_ERRORABLE` if `native_errno` is not `EOK` or equivalent.
 * Horrible macro string manipulation to get some nice output on your errno.
 * @param[in] proc the function which set errno.
 * @param[in] native_errno the errno value.
 */
#define GEN_ERROR_OUT_IF_ERRNO(proc, native_errno) \
	if(native_errno) GEN_ERROR_OUT_ERRNO(proc, native_errno)

/**
 * Horrible macro string manipulation to get some nice output on your errno.
 * Pretty assertion for errno == EOK.
 * @param[in] proc the function which set errno.
 * @param[in] native_errno the errno value.
 */
#define GEN_REQUIRE_NO_ERRNO(proc, native_errno) \
	do { \
		const errno_t gen_internal_gen_require_no_errno_ernno = native_errno; \
		const gen_error_t gen_internal_gen_require_no_errno_gen_error = gen_convert_errno(gen_internal_gen_require_no_errno_ernno); \
		if(gen_internal_gen_require_no_errno_ernno) glogf(FATAL, "Require failed - %s: `%s` failed: %s", gen_error_name(gen_internal_gen_require_no_errno_gen_error), #proc, strerror(gen_internal_gen_require_no_errno_ernno)); \
		GEN_REQUIRE_EQUAL(GEN_OK, gen_internal_gen_require_no_errno_gen_error); \
	} while(0)

#if GEN_GLOGGIFY_EH == ENABLED
/**
 * Horrible macro string manipulation to get some nice output on your errno.
 * @param[in] proc the function which set errno.
 * @param[in] native_errno the errno value.
 */
#define GEN_ERROR_OUT_ERRNO(proc, native_errno) \
	do { \
		const errno_t gen_internal_gen_require_no_errno_ernno = native_errno; \
		const gen_error_t gen_internal_gen_require_no_errno_gen_error = gen_convert_errno(gen_internal_gen_require_no_errno_ernno); \
		if(gen_internal_gen_require_no_errno_ernno) glogf(FATAL, "%s: `%s` failed: %s at %s:%i", gen_error_name(gen_internal_gen_require_no_errno_gen_error), #proc, strerror(gen_internal_gen_require_no_errno_ernno), __FILE__, __LINE__); \
		return gen_internal_gen_require_no_errno_gen_error; \
	} while(0)
#else
#define GEN_ERROR_OUT_ERRNO(proc, native_errno) return gen_convert_errno(native_errno)
#endif

/**
 * This is to make the static analyzer happy.
 */
#define GEN_INTERNAL_BASIC_PARAM_CHECK(param) \
	if(!param) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`" #param "` was NULL")

/**
 * Converts an errno into a genstone error.
 * @param[in] error the errno value to convert.
 * @return the converted error enumeration.
 */
GEN_ERRORABLE gen_convert_errno(errno_t error);
/**
 * @example{lineno} example/gencore/gen_convert_errno.c
 * Example for how to use `gen_convert_errno`.
 * `gen_convert_errno` is used for getting a libc errno error as a Genstone error.
 */

#endif
