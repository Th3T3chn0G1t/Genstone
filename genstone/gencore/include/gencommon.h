// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @file gencommon.h
 * Includes common helpers and utilities for across the codebase
 * Includes the stdlib and helpers to provide missing headers on noncompliant platforms
 * Defines helper macros for pretty keywords and diagnostic info
 */

#ifndef GEN_COMMON_H
#define GEN_COMMON_H

/**
 * Pretty keyword for _Pragma
 * @param s string-literal argument for `_Pragma`
 */
#define pragma(s) _Pragma(s)
/**
 * Pretty keyword for _Generic
 * @param ... type-switch statement for `_Generic`
 */
#define generic(...) _Generic(__VA_ARGS__)

/**
 * Begins a diagnostic region
 */
#define GEN_DIAG_REGION_BEGIN pragma("clang diagnostic push")
/**
 * Ends a diagnostic region
 */
#define GEN_DIAG_REGION_END pragma("clang diagnostic pop")
/**
 * Ignores all extra compiler warnings in a diagnostic region
 */
#define GEN_DIAG_IGNORE_ALL pragma("clang diagnostic ignored \"-Weverything\"")

/**
 * Ignores floating point equality test compiler warnings in a diagnostic region
 */
#define GEN_DIAG_IGNORE_FP_EQ pragma("clang diagnostic ignored \"-Wfloat-equal\"")

GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#ifndef __unused
/**
 * Defines `__unused` on platforms which do not support it by default
 * Marks a variable or function as unused
 */
#define __unused __attribute__((unused))
#endif
#ifndef __inline
/**
 * Defines `__inline` on platforms which do not support it by default
 * Marks a function to be inlined (force inline)
 */
#define __inline __attribute__((always_inline)) __attribute__((artificial)) inline
#endif
#ifndef __deprecated
/**
 * Defines `__deprecated` on platforms which do not support it by default
 * Marks a function as deprecated
 */
#define __deprecated __attribute__((deprecated))
#endif
#ifndef __nodiscard
/**
 * Defines `__nodiscard` on platforms which do not support it by default
 * Marks a function return value as not to be discarded
 */
#define __nodiscard __attribute__((warn_unused_result))
#endif
#ifndef __likely
/**
 * Specifies a control path as likely
 */
#define __likely [[clang::likely]]  
#endif
#ifndef __unlikely
/**
 * Specifies a control path as unlikely
 */
#define __unlikely [[clang::unlikely]]
#endif
GEN_DIAG_REGION_END

/**
 * Return value specification for functions which use `generror` for error reporting
 */
#define GEN_ERRORABLE_RETURN __nodiscard gen_error_t

#include "gendbg.h"
#include "generrors.h"

GEN_DIAG_REGION_BEGIN
GEN_DIAG_IGNORE_ALL
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <ctype.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <iso646.h>
#include <limits.h>
#if PLATFORM == LNX
#include <linux/limits.h> // Why is PATH_MAX here of all places?
#endif
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdalign.h>
#include <stdatomic.h>
#include <time.h>
#include <threads.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>
#if PLATFORM != WIN // clang tgmath.h is broken under windows
#include <tgmath.h>
#endif
#include <stdnoreturn.h>
#include <dirent.h> // Emulation provided on windows by https://github.com/tronkko/dirent
#include <sys/types.h>
#include <sys/stat.h>

#if PLATFORM != WIN
#include <safe_lib.h>
#include <safe_mem_lib.h>
#include <safe_str_lib.h>
#endif
GEN_DIAG_REGION_END

#if PLATFORM == WIN
/**
 * Provide `ssize_t` on Windows
 */
typedef long long ssize_t;
#endif

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

#if PLATFORM == WIN
/* How do you solve a problem like win32.... */
#undef ERROR
#endif

/**
 * Logging levels for logging functions
 */
typedef enum {
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

#ifndef GEN_TOOLING_H
/**
 * The block to execute if logging level `>=ERROR`
 */
#define GEN_INTERNAL_LOG_ERROR_BLOCK \
    do { \
        putchar(GEN_ASCII_BELL); \
    } while(0)
#else
#define GEN_INTERNAL_LOG_ERROR_BLOCK \
    do { \
        putchar(GEN_ASCII_BELL); \
        gtrace; \
    } while(0)
#endif

/**
 * Basic string logging function
 * @param level a `gen_logging_level_t` to determine the prefix from
 * @param string the string to print
 * @note include `gentooling.h` first to get trace information on error
 */
#define glog(level, string) \
    do { \
        FILE* const restrict gen_internal_streamp = level >= ERROR ? stderr : stdout; \
        fprintf(gen_internal_streamp, "%s", GEN_LOGGER_##level##_PREFIX); \
        fprintf(gen_internal_streamp, "%s\n", string); \
        if(level >= ERROR) GEN_INTERNAL_LOG_ERROR_BLOCK;\
    } while(0)
/**
 * `printf`-style formatted logging function
 * @param level a `gen_logging_level_t` to determine the prefix from
 * @param format a format string
 * @param ... the format arguments to print
 * @note include `gentooling.h` first to get trace information on error
 */
#define glogf(level, format, ...) \
    do { \
        FILE* const restrict gen_internal_streamp = level >= ERROR ? stderr : stdout; \
        fprintf(gen_internal_streamp, "%s", GEN_LOGGER_##level##_PREFIX); \
        fprintf(gen_internal_streamp, format, __VA_ARGS__); \
        fprintf(gen_internal_streamp, "\n"); \
        if(level >= ERROR) GEN_INTERNAL_LOG_ERROR_BLOCK;\
    } while(0)

/**
 * Gets the require message from the expected expressions type
 * @param b the expected expression
 */
#define GEN_INTERNAL_REQUIRE_EQUAL_MESSAGE(b) \
    generic((b), \
        long double: "Require failed - Expected: %s (%Lf) Got: %s (%Lf) at line %i in %s\n", \
        double: "Require failed - Expected: %s (%lf) Got: %s (%lf) at line %i in %s\n", \
        float: "Require failed - Expected: %s (%f) Got: %s (%f) at line %i in %s\n", \
        unsigned long long: "Require failed - Expected: %s (%llu) Got: %s (%llu) at line %i in %s\n", \
        long long: "Require failed - Expected: %s (%lli) Got: %s (%lli) at line %i in %s\n", \
        unsigned long: "Require failed - Expected: %s (%lu) Got: %s (%lu) at line %i in %s\n", \
        long: "Require failed - Expected: %s (%li) Got: %s (%li) at line %i in %s\n", \
        unsigned int: "Require failed - Expected: %s (%u) Got: %s (%u) at line %i in %s\n", \
        int: "Require failed - Expected: %s (%i) Got: %s (%i) at line %i in %s\n", \
        unsigned short: "Require failed - Expected: %s (%hu) Got: %s (%hu) at line %i in %s\n", \
        short: "Require failed - Expected: %s (%hi) Got: %s (%hi) at line %i in %s\n", \
        signed char: "Require failed - Expected: %s (%hhi) Got: %s (%hhi) at line %i in %s\n", \
        unsigned char: "Require failed - Expected: %s (%hhu) Got: %s (%hhu) at line %i in %s\n", \
        char: "Require failed - Expected: %s (%c) Got: %s (%c) at line %i in %s\n", \
        bool: "Require failed - Expected: %s (%i) Got: %s (%i) at line %i in %s\n", \
        default: "Require failed - Expected: %s (%p) Got: %s (%p) at line %i in %s\n" \
    )
    
/**
 * Pretty assertion for equality
 * @param a the expected expression
 * @param b the recieved expression
 * @note Determines type to use correct comparison using `generic` on second argument, this is to avoid having cases for `const`
 * @note Only works for trivial types
 */
#define GEN_REQUIRE_EQUAL(a, b) \
    do { \
        if(!__builtin_constant_p(a)) glogf(WARNING, "Expected expression %s is not constant\n", #a); \
        if(a != b) { \
            glogf(FATAL, GEN_INTERNAL_REQUIRE_EQUAL_MESSAGE(b), #a, a, #b, b, __LINE__, __FILE__); \
            abort(); \
        } \
    } while(0)
 
/**
 * Pretty assertion for equality of strings
 * @param a the expected string
 * @param b the recieved string
 * @note Use `GEN_REQUIRE_EQUAL_MEMREGION` for non-constant expected values
 */
#define GEN_REQUIRE_EQUAL_STRING(a, b) \
    do { \
        if(!__builtin_constant_p(a)) glogf(WARNING, "Expected expression %s (%s) is not constant at line %i in %s\n", #a, a, __LINE__, __FILE__); \
        if(!b || strcmp(a, b)) { \
            glogf(FATAL, "Require failed - Expected: %s (%s) Got: %s (%s) at line %i in %s\n", #a, a, #b, b, __LINE__, __FILE__); \
            abort(); \
        } \
    } while(0)

/**
 * Pretty assertion for equality of memory regions
 * @param a the expected data
 * @param b the recieved data
 * @param s the amount of data in bytes to compare
 */
#define GEN_REQUIRE_EQUAL_MEMREGION(a, b, s) \
    do { \
        if((!b && s) || memcmp(a, b, s)) { \
            glogf(FATAL, "Require failed - Expected: %s (%p) (%c%c%c...) Got: %s (%p) (%c%c%c...) at line %i in %s\n", #a, a, ((char* const restrict) a)[0], ((char* const restrict) a)[1], ((char* const restrict) a)[2], #b, b, ((char* const restrict) b)[0], ((char* const restrict) b)[1], ((char* const restrict) b)[2], __LINE__, __FILE__); \
            abort(); \
        } \
    } while(0)

/**
 * Pretty assertion for bad control paths
 */
#define GEN_REQUIRE_NO_REACH \
    do { \
        glogf(FATAL, "Require failed - Invalid control path reached at line %i in %s\n", __LINE__, __FILE__); \
        abort(); \
    } while(0)

#if GEN_DEBUG_FOREACH_REGISTER == ENABLED
#define GEN_INTERNAL_FOREACH_ITER_DECL register size_t
#else
/**
 * The type used when declaring an iterator in `GEN_FOREACH`
 * @see GEN_DEBUG_FOREACH_REGISTER
 */
#define GEN_INTERNAL_FOREACH_ITER_DECL size_t
#endif

/**
 * Iterates over a container with explicit length
 * @param iter the identifier to use for the iterating index
 * @param memb the identifier to use for the indexed member
 * @param len the length of the container to iterate
 * @param container the container to iterate
 * @see GEN_DEBUG_FOREACH_REGISTER
 * @see GEN_INTERNAL_FOREACH_LOOP_QUALIFIERS
 */
#define GEN_FOREACH(iter, memb, len, container) \
    __typeof__((container)[0]) memb = (container)[0]; \
    for(GEN_INTERNAL_FOREACH_ITER_DECL iter = SIZE_MAX; ++iter < (size_t) (len); memb = (container)[iter + 1])

/**
 * Iterates over a container with explicit length
 * `memb` is a pointer to the indexed member
 * @param iter the identifier to use for the iterating index
 * @param memb the identifier to use for the indexed member
 * @param len the length of the container to iterate
 * @param container the container to iterate
 * @see GEN_DEBUG_FOREACH_REGISTER
 * @see GEN_INTERNAL_FOREACH_LOOP_QUALIFIERS
 */
#define GEN_FOREACH_PTR(iter, memb, len, container) \
    __typeof__((container)[0])* memb = &(container)[0]; \
    for(GEN_INTERNAL_FOREACH_ITER_DECL iter = SIZE_MAX; ++iter < (size_t) (len); memb = &(container)[iter + 1])

/**
 * Iterates over a container with explicit length
 * `memb` is a pointer to the indexed member
 * Uses the container type as the member type directly
 * @param iter the identifier to use for the iterating index
 * @param memb the identifier to use for the indexed member
 * @param len the length of the container to iterate
 * @param container the container to iterate
 * @see GEN_DEBUG_FOREACH_REGISTER
 * @see GEN_INTERNAL_FOREACH_LOOP_QUALIFIERS
 */
#define GEN_FOREACH_DIRECT_PTR(iter, memb, len, container) \
    __typeof__((container)) memb = (container); \
    for(GEN_INTERNAL_FOREACH_ITER_DECL iter = SIZE_MAX; ++iter < (size_t) (len); memb = (container) + (iter + 1))

#define GEN_MICROSECONDS_PER_SECOND 1000000
#define GEN_MILLISECONDS_PER_SECOND 1000

#if PLATFORM == WIN
#include <winsock.h>
/**
 * @see https://pubs.opengroup.org/onlinepubs/000095399/functions/gettimeofday.html
 * @see https://stackoverflow.com/questions/10905892/equivalent-of-gettimeday-for-windows/26085827#26085827
 */
extern int gettimeofday(struct timeval* const restrict tp, __unused const void* const restrict tzp);
#else
#include <sys/time.h>
#endif

/**
 * Converts a timeval to seconds as a `long double`
 * @param timeval the `struct timeval` to convert
 */
#define GEN_TIMEVAL_AS_SECONDS(timeval) ((long double) (timeval).tv_usec + ((long double) (timeval).tv_sec * (long double) GEN_MICROSECONDS_PER_SECOND))

/**
 * Adds two timevals
 * @param a the first value
 * @param b the second value
 * @param result pointer to storage for the result
 * @see https://gist.github.com/vchernov/4774682#file-timeval_add-cpp
 */
extern void gen_timeval_add(const struct timeval* const restrict a, const struct timeval* const restrict b, struct timeval* const restrict result);
/**
 * Subtracts two timevals
 * @param a the first value
 * @param b the second value
 * @param result pointer to storage for the result
 * @see https://gist.github.com/vchernov/4774682#file-timeval_sub-cpp
 */
extern void gen_timeval_sub(const struct timeval* const restrict a, const struct timeval* const restrict b, struct timeval* const restrict result);

#endif
