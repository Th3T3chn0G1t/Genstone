// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file glog.h
 * Includes Genstone logging utils
 */

#ifndef GEN_GLOG_H
#define GEN_GLOG_H

/**
 * ANSI color value for gray
 */
#define GEN_ANSI_GRAY "30"
/**
 * ANSI color value for red
 */
#define GEN_ANSI_RED "31"
/**
 * ANSI color value for green
 */
#define GEN_ANSI_GREEN "32"
/**
 * ANSI color value for yellow
 */
#define GEN_ANSI_YELLOW "33"
/**
 * ANSI color value for blue
 */
#define GEN_ANSI_BLUE "34"
/**
 * ANSI color value for purple
 */
#define GEN_ANSI_PURPLE "35"
/**
 * ANSI color value for cyan
 */
#define GEN_ANSI_CYAN "36"
/**
 * ANSI color value for white
 */
#define GEN_ANSI_WHITE "37"

/**
 * ANSI code for bold
 */
#define GEN_ANSI_BOLD "1"

/**
 * ANSI code for clear
 */
#define GEN_ANSI_CLEAR "0"

/**
 * Prefix for ANSI sequences
 */
#define GEN_INTERNAL_ANSI_SEQUENCE_PREFIX "\033["
/**
 * Suffix for ANSI sequences
 */
#define GEN_INTERNAL_ANSI_SEQUENCE_SUFFIX "m"
/**
 * Dark modifier prefix for ANSI color codes
 */
#define GEN_INTERNAL_ANSI_COLOR_DARK_PREFIX "0;"
/**
 * Light modifier prefix for ANSI color codes
 */
#define GEN_INTERNAL_ANSI_COLOR_LIGHT_PREFIX "1;"

/**
 * Applies ANSI sequence prefix and suffix
 * @param code the ANSI code to apply the prefix and suffix to
 */
#define GEN_ANSI_SEQUENCE(code) GEN_INTERNAL_ANSI_SEQUENCE_PREFIX code GEN_INTERNAL_ANSI_SEQUENCE_SUFFIX

/**
 * Applies dark color modifier prefix
 * @param color the ANSI color code to apply the prefix to
 */
#define GEN_ANSI_COLOR_DARK(color) GEN_ANSI_SEQUENCE(GEN_INTERNAL_ANSI_COLOR_DARK_PREFIX color)
/**
 * Applies light color modifier prefix to ANSI color code
 * @param color the ANSI color code to apply the prefix to
 */
#define GEN_ANSI_COLOR_LIGHT(color) GEN_ANSI_SEQUENCE(GEN_INTERNAL_ANSI_COLOR_LIGHT_PREFIX color)

/**
 * The ASCII code `BEL`
 */
#define GEN_ASCII_BELL '\a'

/**
 * Logging levels for logging functions
 */
typedef enum {
    /**
     * Trace logging severity level
     */
    TRACE,
    /**
     * Performance logging severity level
     */
    PERFORMANCE,
    /**
     * Note logging severity level
     */
    NOTE,
    /**
     * Info logging severity level
     */
    INFO,
    /**
     * Warning logging severity level
     */
    WARNING,
    /**
     * Error logging severity level
     */
    ERROR,
    /**
     * Fatal logging severity level
     */
    FATAL
} gen_logging_level_t;

/**
 * Logger prefix for backtraces
 */
#define GEN_LOGGER_TRACE_PREFIX GEN_ANSI_COLOR_LIGHT(GEN_ANSI_GRAY) GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Trace: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)

/**
 * Logger prefix for performance level logging
 */
#define GEN_LOGGER_PERFORMANCE_PREFIX GEN_ANSI_COLOR_LIGHT(GEN_ANSI_GRAY) GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Performance: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)
/**
 * Logger prefix for debug level logging
 */
#define GEN_LOGGER_DEBUG_PREFIX GEN_ANSI_COLOR_DARK(GEN_ANSI_GREEN) GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Debug: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)
/**
 * Logger prefix for note level logging
 */
#define GEN_LOGGER_NOTE_PREFIX GEN_ANSI_COLOR_DARK(GEN_ANSI_CYAN) GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Note: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)
/**
 * Logger prefix for info level logging
 */
#define GEN_LOGGER_INFO_PREFIX GEN_ANSI_COLOR_DARK(GEN_ANSI_BLUE) GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Info: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)
/**
 * Logger prefix for warning level logging
 */
#define GEN_LOGGER_WARNING_PREFIX GEN_ANSI_COLOR_DARK(GEN_ANSI_YELLOW) GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Warning: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)
/**
 * Logger prefix for error level logging
 */
#define GEN_LOGGER_ERROR_PREFIX GEN_ANSI_COLOR_LIGHT(GEN_ANSI_RED) GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Error: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)
/**
 * Logger prefix for fatal level logging
 */
#define GEN_LOGGER_FATAL_PREFIX GEN_ANSI_COLOR_DARK(GEN_ANSI_PURPLE) GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Fatal: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)

#ifndef GEN_GLOG_STREAM_COUNT
/**
 * The number of output/error streams available to glog
 */
#define GEN_GLOG_STREAM_COUNT 8
#endif

/**
 * glog output streams
 * @note Buffer is null terminated for simplicity
 */
extern FILE* gen_glog_out_streams[GEN_GLOG_STREAM_COUNT + 1];
/**
 * glog error streams
 * @note Buffer is null terminated for simplicity
 */
extern FILE* gen_glog_err_streams[GEN_GLOG_STREAM_COUNT + 1];

/**
 * Basic string logging function
 * @param level a `gen_logging_level_t` to determine the prefix from
 * @param string the string to print
 * @see gen_glog_out_streams for configuring of output streams
 * @see gen_glog_err_streams for configuring of output streams
 */
#define glog(level, string) \
    do { \
        GEN_DIAG_REGION_BEGIN \
        pragma("clang diagnostic ignored \"-Wshadow\"") \
        bool gen_internal_glog_level_is_error = level >= ERROR; \
        GEN_FOREACH(gen_internal_glog_streams_iterator_index, gen_internal_glog_streams_iterator_value, GEN_GLOG_STREAM_COUNT, gen_internal_glog_level_is_error ? gen_glog_err_streams : gen_glog_out_streams) { \
            if(!gen_internal_glog_streams_iterator_value) break; \
            fprintf_s(gen_internal_glog_streams_iterator_value, "%s%s\n", GEN_LOGGER_##level##_PREFIX, string); \
        } \
        if(gen_internal_glog_level_is_error) gtrace; \
        GEN_DIAG_REGION_END \
    } while(0) \

/**
 * `printf`-style formatted logging function
 * @param level a `gen_logging_level_t` to determine the prefix from
 * @param format a format string
 * @param ... the format arguments to print
 * @see gen_glog_out_streams for configuring of output streams
 * @see gen_glog_err_streams for configuring of output streams
 */
#define glogf(level, format, ...) \
    do { \
        GEN_DIAG_REGION_BEGIN \
        pragma("clang diagnostic ignored \"-Wshadow\"") \
        bool gen_internal_glog_level_is_error = level >= ERROR; \
        GEN_FOREACH(gen_internal_glog_streams_iterator_index, gen_internal_glog_streams_iterator_value, GEN_GLOG_STREAM_COUNT, gen_internal_glog_level_is_error ? gen_glog_err_streams : gen_glog_out_streams) { \
            if(!gen_internal_glog_streams_iterator_value) break; \
            fputs(GEN_LOGGER_##level##_PREFIX, gen_internal_glog_streams_iterator_value); \
            fprintf_s(gen_internal_glog_streams_iterator_value, format, __VA_ARGS__); \
            fputc('\n', gen_internal_glog_streams_iterator_value); \
        } \
        if(gen_internal_glog_level_is_error) gtrace; \
        if(level >= FATAL) abort(); \
        GEN_DIAG_REGION_END \
    } while(0) \

#endif
