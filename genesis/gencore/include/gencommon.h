// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @file gencommon.h
 * Includes common helpers and utilities for across the codebase
 * Includes the stdlib and helpers to provide missing headers on noncompliant platforms
 * Defines helper macros for pretty keywords and diagnostic info
 * Defines common data structures and operations
 */

#ifndef GEN_COMMON_H
#define GEN_COMMON_H

/**
 * Pretty keyword _Pragma
 * @param s string-literal argument for _Pragma
 */
#define pragma(s) _Pragma(s)
/**
 * Pretty keyword _Generic
 * @param ... type-switch statement for _Generic
 */
#define generic(...) _Generic(__VA_ARGS__)

/**
 * Diagnostic helper
 * Begins a diagnostic region
 */
#define GEN_DIAG_REGION_BEGIN pragma("clang diagnostic push")
/**
 * Diagnostic helper
 * Ends a diagnostic region
 */
#define GEN_DIAG_REGION_END pragma("clang diagnostic pop")
/**
 * Diagnostic helper
 * Ignores all extra compiler warnings in a diagnostic region
 */
#define GEN_DIAG_IGNORE_ALL pragma("clang diagnostic ignored \"-Weverything\"")

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
GEN_DIAG_REGION_END

/**
 * @internal
 * Gets the require message from the expected expressions type
 * @param b the expected expression
 */
#define _gen_require_equal_message(b) \
    generic((b), \
        long double: "Require failed - Expected: %s (%Lf) Got: %s (%Lf)\n", \
        double: "Require failed - Expected: %s (%lf) Got: %s (%lf)\n", \
        float: "Require failed - Expected: %s (%f) Got: %s (%f)\n", \
        unsigned long long: "Require failed - Expected: %s (%llu) Got: %s (%llu)\n", \
        long long: "Require failed - Expected: %s (%lli) Got: %s (%lli)\n", \
        unsigned long: "Require failed - Expected: %s (%lu) Got: %s (%lu)\n", \
        long: "Require failed - Expected: %s (%li) Got: %s (%li)\n", \
        unsigned int: "Require failed - Expected: %s (%u) Got: %s (%u)\n", \
        int: "Require failed - Expected: %s (%i) Got: %s (%i)\n", \
        unsigned short: "Require failed - Expected: %s (%hu) Got: %s (%hu)\n", \
        short: "Require failed - Expected: %s (%hi) Got: %s (%hi)\n", \
        signed char: "Require failed - Expected: %s (%hhi) Got: %s (%hhi)\n", \
        unsigned char: "Require failed - Expected: %s (%hhu) Got: %s (%hhu)\n", \
        char: "Require failed - Expected: %s (%c) Got: %s (%c)\n", \
        bool: "Require failed - Expected: %s (%i) Got: %s (%i)\n", \
        default: "Require failed - Expected: %s (%p) Got: %s (%p)\n" \
    )
    
/**
 * Pretty assertion for equality
 * @param a the expected expression
 * @param b the recieved expression
 * @note Determines type to use correct comparison using `generic` on second argument, this is to avoid having cases for `const`
 * @note Only works for trivial types
 */
#define gen_require_equal(a, b) \
    if(!__builtin_constant_p(a)) printf("Expected expression %s is not constant\n", #a); \
    if(a != b) { \
        printf(_gen_require_equal_message(b), #a, a, #b, b); \
        abort(); \
    }

/**
 * Pretty assertion for equality of strings
 * @param a the expected string
 * @param b the recieved string
 * @note Use `gen_require_equal_memregion` for non-constant expected values
 */
#define gen_require_equal_string(a, b) \
    if(!__builtin_constant_p(a)) printf("Expected expression %s (%s) is not constant\n", #a, a); \
    if(strcmp(a, b)) { \
        printf("Require failed - Expected: %s (%s) Got: %s (%s)\n", #a, a, #b, b), \
        abort(); \
    }

/**
 * Pretty assertion for equality of memory regions
 * @param a the expected data
 * @param b the recieved data
 * @param s the amount of data in bytes to compare
 */
#define gen_require_equal_memregion(a, b, s) \
    if(memcmp(a, b, s)) { \
        printf("Require failed - Expected: %s (%p) (%c%c%c...) Got: %s (%p) (%c%c%c...)\n", #a, a, ((char*) a)[0], ((char*) a)[1], ((char*) a)[2], #b, b, ((char*) b)[0], ((char*) b)[1], ((char*) b)[2]); \
        abort(); \
    }

#endif
