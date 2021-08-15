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

#include "gendbg.h"
#include "generrors.h"

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
#define _GEN_REQUIRE_EQUAL_MESSAGE(b) \
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
        if(!__builtin_constant_p(a)) fprintf(stderr, "Expected expression %s is not constant\n", #a); \
        if(a != b) { \
            fprintf(stderr, _GEN_REQUIRE_EQUAL_MESSAGE(b), #a, a, #b, b, __LINE__, __FILE__); \
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
        if(!__builtin_constant_p(a)) fprintf(stderr, "Expected expression %s (%s) is not constant at line %i in %s\n", #a, a, __LINE__, __FILE__); \
        if(!b || strcmp(a, b)) { \
            fprintf(stderr, "Require failed - Expected: %s (%s) Got: %s (%s) at line %i in %s\n", #a, a, #b, b, __LINE__, __FILE__), \
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
            fprintf(stderr, "Require failed - Expected: %s (%p) (%c%c%c...) Got: %s (%p) (%c%c%c...) at line %i in %s\n", #a, a, ((char*) a)[0], ((char*) a)[1], ((char*) a)[2], #b, b, ((char*) b)[0], ((char*) b)[1], ((char*) b)[2], __LINE__, __FILE__); \
            abort(); \
        } \
    } while(0)

/**
 * Pretty assertion for bad control paths
 */
#define GEN_REQUIRE_NO_REACH \
    do { \
        fprintf(stderr, "Require failed - Invalid control path reached at line %i in %s\n", __LINE__, __FILE__); \
        abort(); \
    } while(0)
#if GEN_DEBUG_FOREACH_REGISTER == ENABLED
/**
 * @internal
 * The type used when declaring an iterator in `GEN_FOREACH`
 * @see GEN_DEBUG_FOREACH_REGISTER
 */
#define _GEN_FOREACH_ITER_DECL register size_t
#else
#define _GEN_FOREACH_ITER_DECL size_t
#endif

#if GEN_DEBUG_FOREACH_PRECALC == ENABLED
/**
 * @internal
 * The identifier to use in the loop for accessing `len`
 * @see GEN_DEBUG_FOREACH_PRECALC
 */
#define _GEN_FOREACH_LENGTH_IDENTIFIER(iter, memb, len) (_gen_foreach_length__##iter##__##memb##__##__func__##__LINE__);
/**
 * @internal
 * The identifier to use in the loop for accessing `container`
 * @see GEN_DEBUG_FOREACH_PRECALC
 */
#define _GEN_FOREACH_CONTAINER_IDENTIFIER(iter, memb, container) (_gen_foreach_container__##iter##__##memb##__##__func__##__LINE__);
/**
 * @internal
 * Predeclarations for `GEN_FOREACH` statements
 * @see GEN_DEBUG_FOREACH_PRECALC
 */
#define _GEN_FOREACH_PREDECL(iter, memb, len, container) \
    __typeof__((container)[0])* const _GEN_FOREACH_CONTAINER_IDENTIFIER(iter, memb, container) = (const __typeof__((container)[0])*) (container); \
    const _GEN_FOREACH_ITER_DECL _GEN_FOREACH_LENGTH_IDENTIFIER(iter, memb, len) = (const _GEN_FOREACH_ITER_DECL) (len)
#else
#define _GEN_FOREACH_LENGTH_IDENTIFIER(iter, memb, len) (len)
#define _GEN_FOREACH_CONTAINER_IDENTIFIER(iter, memb, container) (container) 
#define _GEN_FOREACH_PREDECL(iter, memb, len, container) (void) NULL
#endif

/**
 * Iterates over a container with explicit length
 * @param iter the identifier to use for the iterating index
 * @param memb the identifier to use for the indexed member
 * @param len the length of the container to iterate
 * @param container the container to iterate
 * @see GEN_DEBUG_FOREACH_REGISTER
 * @see GEN_DEBUG_FOREACH_PRECALC
 */
#define GEN_FOREACH(iter, memb, len, container) \
    _GEN_FOREACH_PREDECL(iter, memb, len, container); \
    __typeof__(_GEN_FOREACH_CONTAINER_IDENTIFIER(iter, memb, container)[0]) memb = _GEN_FOREACH_CONTAINER_IDENTIFIER(iter, memb, container)[0]; \
    for(_GEN_FOREACH_ITER_DECL iter = SIZE_MAX; ++iter < _GEN_FOREACH_LENGTH_IDENTIFIER(iter, memb, len); memb = _GEN_FOREACH_CONTAINER_IDENTIFIER(iter, memb, container)[iter + 1])
/**
 * Iterates over a container with explicit length
 * `memb` is a pointer to the indexed member
 * @param iter the identifier to use for the iterating index
 * @param memb the identifier to use for the indexed member
 * @param len the length of the container to iterate
 * @param container the container to iterate
 * @see GEN_DEBUG_FOREACH_REGISTER
 */
#define GEN_FOREACH_PTR(iter, memb, len, container) \
    __typeof__((container)[0])* memb = &(container)[0]; \
    for(_GEN_FOREACH_ITER_DECL iter = SIZE_MAX; ++iter < (len); memb = &(container)[iter + 1])

#endif
