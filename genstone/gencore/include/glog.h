// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file glog.h
 * Includes Genstone logging utils.
 */

#ifndef GEN_GLOG_H
#define GEN_GLOG_H

/**
 * ANSI color value for gray.
 */
#define GEN_ANSI_GRAY "30"
/**
 * ANSI color value for red.
 */
#define GEN_ANSI_RED "31"
/**
 * ANSI color value for green.
 */
#define GEN_ANSI_GREEN "32"
/**
 * ANSI color value for yellow.
 */
#define GEN_ANSI_YELLOW "33"
/**
 * ANSI color value for blue.
 */
#define GEN_ANSI_BLUE "34"
/**
 * ANSI color value for purple.
 */
#define GEN_ANSI_PURPLE "35"
/**
 * ANSI color value for cyan.
 */
#define GEN_ANSI_CYAN "36"
/**
 * ANSI color value for white.
 */
#define GEN_ANSI_WHITE "37"

/**
 * ANSI code for bold.
 */
#define GEN_ANSI_BOLD "1"

/**
 * ANSI code for clear.
 */
#define GEN_ANSI_CLEAR "0"

/**
 * Prefix for ANSI sequences.
 */
#define GEN_INTERNAL_ANSI_SEQUENCE_PREFIX "\033["
/**
 * Suffix for ANSI sequences.
 */
#define GEN_INTERNAL_ANSI_SEQUENCE_SUFFIX "m"
/**
 * Dark modifier prefix for ANSI color codes.
 */
#define GEN_INTERNAL_ANSI_COLOR_DARK_PREFIX "0;"
/**
 * Light modifier prefix for ANSI color codes.
 */
#define GEN_INTERNAL_ANSI_COLOR_LIGHT_PREFIX "1;"

/**
 * Applies ANSI sequence prefix and suffix.
 * @param[in] code the ANSI code to apply the prefix and suffix to.
 */
#define GEN_ANSI_SEQUENCE(code) GEN_INTERNAL_ANSI_SEQUENCE_PREFIX code GEN_INTERNAL_ANSI_SEQUENCE_SUFFIX

/**
 * Applies dark color modifier prefix.
 * @param[in] color the ANSI color code to apply the prefix to.
 */
#define GEN_ANSI_COLOR_DARK(color) GEN_ANSI_SEQUENCE(GEN_INTERNAL_ANSI_COLOR_DARK_PREFIX color)
/**
 * Applies light color modifier prefix to ANSI color code.
 * @param[in] color the ANSI color code to apply the prefix to.
 */
#define GEN_ANSI_COLOR_LIGHT(color) GEN_ANSI_SEQUENCE(GEN_INTERNAL_ANSI_COLOR_LIGHT_PREFIX color)

/**
 * The ASCII code `BEL`.
 */
#define GEN_ASCII_BELL '\a'

/**
 * Logging levels for logging functions.
 */
typedef enum
{
	/**
     * Trace logging severity level.
     */
	TRACE,
	/**
     * Performance logging severity level.
     */
	PERFORMANCE,
	/**
     * Note logging severity level.
     */
	NOTE,
	/**
     * Info logging severity level.
     */
	INFO,
	/**
     * Warning logging severity level.
     */
	WARNING,
	/**
     * Error logging severity level.
     */
	ERROR,
	/**
     * Fatal logging severity level.
     */
	FATAL
} gen_logging_level_t;

/**
 * Logger prefix for backtraces.
 */
#define GEN_LOGGER_TRACE_PREFIX GEN_ANSI_COLOR_LIGHT(GEN_ANSI_GRAY) \
GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Trace: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)

/**
 * Logger prefix for performance level logging.
 */
#define GEN_LOGGER_PERFORMANCE_PREFIX GEN_ANSI_COLOR_LIGHT(GEN_ANSI_GRAY) \
GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Performance: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)
/**
 * Logger prefix for debug level logging.
 */
#define GEN_LOGGER_DEBUG_PREFIX GEN_ANSI_COLOR_DARK(GEN_ANSI_GREEN) \
GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Debug: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)
/**
 * Logger prefix for note level logging.
 */
#define GEN_LOGGER_NOTE_PREFIX GEN_ANSI_COLOR_DARK(GEN_ANSI_CYAN) \
GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Note: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)
/**
 * Logger prefix for info level logging.
 */
#define GEN_LOGGER_INFO_PREFIX GEN_ANSI_COLOR_DARK(GEN_ANSI_BLUE) \
GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Info: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)
/**
 * Logger prefix for warning level logging.
 */
#define GEN_LOGGER_WARNING_PREFIX GEN_ANSI_COLOR_DARK(GEN_ANSI_YELLOW) \
GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Warning: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)
/**
 * Logger prefix for error level logging.
 */
#define GEN_LOGGER_ERROR_PREFIX GEN_ANSI_COLOR_LIGHT(GEN_ANSI_RED) \
GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Error: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)
/**
 * Logger prefix for fatal level logging.
 */
#define GEN_LOGGER_FATAL_PREFIX GEN_ANSI_COLOR_DARK(GEN_ANSI_PURPLE) \
GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Fatal: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)

/**
 * @example{lineno} example/gencore/glog.c
 * Example for how to use `glog` and `glogf`.
 * The `glog*` family of macros is intended as a simple logger for use throughout Genstone primarily, and so is missing some features a more general logger may have such as sinks.
 */

/**
 * Basic string logging function.
 * @param[in] level a `gen_logging_level_t` to determine the prefix from.
 * @param[in] string the string to print.
 * @see gen_glog_out_streams
 * @see gen_glog_err_streams
 */
#define glog(level, string) \
	do { \
		GEN_DIAG_REGION_BEGIN \
		pragma("clang diagnostic ignored \"-Wshadow\"") \
			fprintf(level >= ERROR ? stderr : stdout, "%s%s\n", GEN_LOGGER_##level##_PREFIX, string); \
		if(level >= ERROR) gtrace; \
		GEN_DIAG_REGION_END \
	} while(0)

/**
 * `printf`-style formatted logging function.
 * @param[in] level a `gen_logging_level_t` to determine the prefix from.
 * @param[in] format a format string.
 * @param[in] ... the format arguments to print.
 * @see gen_glog_out_streams
 * @see gen_glog_err_streams
 */
#define glogf(level, format, ...) \
	do { \
		GEN_DIAG_REGION_BEGIN \
		pragma("clang diagnostic ignored \"-Wshadow\"") \
			fputs(GEN_LOGGER_##level##_PREFIX, level >= ERROR ? stderr : stdout); \
		fprintf(level >= ERROR ? stderr : stdout, format, __VA_ARGS__); \
		fputc('\n', level >= ERROR ? stderr : stdout); \
		if(level >= ERROR) gtrace; \
		GEN_DIAG_REGION_END \
	} while(0)

#endif
