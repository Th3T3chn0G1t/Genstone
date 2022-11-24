// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_LOG_H
#define GEN_LOG_H

#include "gencommon.h"


#ifndef GEN_LOG_CONTEXT_PAD
/**
 * The width the pad out context strings to.
 */
#define GEN_LOG_CONTEXT_PAD 18
#endif

#ifndef GEN_LOG_SEVERITY_PAD
/**
 * The width the pad out severity strings to.
 */
#define GEN_LOG_SEVERITY_PAD 8
#endif

#define GEN_LOG_RISING_EDGE_LENGTH (GEN_LOG_CONTEXT_PAD + 2 + 1 + GEN_LOG_SEVERITY_PAD + 2 + 1)

/**
 * Logging severity levels.
 */
typedef enum {
    /**
     * Logging level for backtraces, performance logging etc.
     */
    GEN_LOG_LEVEL_TRACE,
    /**
     * Logging level for debug information.
     */
    GEN_LOG_LEVEL_DEBUG,
    /**
     * Reasonable default logging level.
     */
    GEN_LOG_LEVEL_INFO,
    /**
     * Logging level for non-failing erroneous behaviour.
     */
    GEN_LOG_LEVEL_WARNING,
    /**
     * Logging level for failing erroneous behaviour.
     */
    GEN_LOG_LEVEL_ERROR,
    /**
     * Logging level for failing erroneous behaviour resulting in program termination.
     */
    GEN_LOG_LEVEL_FATAL
} gen_log_level_t;

/**
 * Prints a line of log output to stdout.
 * @param[in] severity The log level to print at.
 * @param[in] context A string containing the context/location of the log.
 * @param[in] string The string to output to the log.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_log(const gen_log_level_t severity, const char* const restrict context, const char* const restrict string);

/**
 * Prints a line of formatted log output to stdout.
 * @param[in] severity The log level to print at.
 * @param[in] context A string containing the context/location of the log.
 * @param[in] format The format string to apply format arguments to.
 * @param[in] ... The format parameters to apply to `format`.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_log_formatted(const gen_log_level_t severity, const char* const restrict context, const char* const restrict format, ...);

#endif
