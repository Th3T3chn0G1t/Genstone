// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genutil.h
 * Includes general Genstone utils which don't belong elsewhere.
 */

#ifndef GEN_UTIL_H
#define GEN_UTIL_H

#if GEN_DEBUG_FOREACH_REGISTER == ENABLED
#define GEN_INTERNAL_FOREACH_ITER_DECL register size_t
#else
/**
 * The type used when declaring an iterator in `GEN_FOREACH`.
 * @see GEN_DEBUG_FOREACH_REGISTER
 */
#define GEN_INTERNAL_FOREACH_ITER_DECL size_t
#endif

/**
 * @example{lineno} example/gencore/GEN_FOREACH.c
 * Example for how to use `GEN_FOREACH`.
 * The `GEN_FOREACH*` family of macros is intended as a way to centralize container iteration for the purpose of introspection and micro-optimization.
 * The `GEN_FOREACH*` family of macros is intended as a Genstone internal utility primarily, but is part of the public API if a program decides to use it.
 */

/**
 * Iterates over a container with explicit length.
 * @param[in] iter the identifier to use for the iterating index.
 * @param[in] memb the identifier to use for the indexed member.
 * @param[in] len the length of the container to iterate.
 * @param[in] container the container to iterate.
 * @see GEN_DEBUG_FOREACH_REGISTER
 */
#define GEN_FOREACH(iter, memb, len, container) \
    __typeof__((container)[0]) memb = (container)[0]; \
    for(GEN_INTERNAL_FOREACH_ITER_DECL iter = SIZE_MAX; ++iter < (size_t) (len); memb = (container)[iter + 1])

/**
 * Iterates over a container with explicit length.
 * `memb` is a pointer to the indexed member.
 * @param[in] iter the identifier to use for the iterating index.
 * @param[in] memb the identifier to use for the indexed member.
 * @param[in] len the length of the container to iterate.
 * @param[in] container the container to iterate.
 * @see GEN_DEBUG_FOREACH_REGISTER
 */
#define GEN_FOREACH_PTR(iter, memb, len, container) \
    __typeof__((container)[0])* memb = &(container)[0]; \
    for(GEN_INTERNAL_FOREACH_ITER_DECL iter = SIZE_MAX; ++iter < (size_t) (len); memb = &(container)[iter + 1])

/**
 * Iterates over a container with explicit length.
 * `memb` is a pointer to the indexed member.
 * Uses the container type as the member type directly.
 * `GEN_FOREACH_DIRECT_PTR` is a variant of `GEN_FOREACH_PTR` added for diagnostic reasons and probably shouldn't be used.
 * @param[in] iter the identifier to use for the iterating index.
 * @param[in] memb the identifier to use for the indexed member.
 * @param[in] len the length of the container to iterate.
 * @param[in] container the container to iterate.
 * @see GEN_DEBUG_FOREACH_REGISTER
 */
#define GEN_FOREACH_DIRECT_PTR(iter, memb, len, container) \
    __typeof__((container)) memb = (container); \
    for(GEN_INTERNAL_FOREACH_ITER_DECL iter = SIZE_MAX; ++iter < (size_t) (len); memb = (container) + (iter + 1))

/**
 * Gets the require message from the expected expressions type.
 * @param[in] b the expected expression.
 */
#define GEN_INTERNAL_REQUIRE_EQUAL_MESSAGE(b) \
    generic((b), \
        long double: "Require failed - Expected: `%s` (%Lf) Got: `%s` (%Lf) at line %i in %s", \
        double: "Require failed - Expected: `%s` (%lf) Got: `%s` (%lf) at line %i in %s", \
        float: "Require failed - Expected: `%s` (%f) Got: `%s` (%f) at line %i in %s", \
        unsigned long long: "Require failed - Expected: `%s` (%llu) Got: `%s` (%llu) at line %i in %s", \
        long long: "Require failed - Expected: `%s` (%lli) Got: `%s` (%lli) at line %i in %s", \
        unsigned long: "Require failed - Expected: `%s` (%lu) Got: `%s` (%lu) at line %i in %s", \
        long: "Require failed - Expected: `%s` (%li) Got: `%s` (%li) at line %i in %s", \
        unsigned int: "Require failed - Expected: `%s` (%u) Got: `%s` (%u) at line %i in %s", \
        int: "Require failed - Expected: `%s` (%i) Got: `%s` (%i) at line %i in %s", \
        unsigned short: "Require failed - Expected: `%s` (%hu) Got: `%s` (%hu) at line %i in %s", \
        short: "Require failed - Expected: `%s` (%hi) Got: `%s` (%hi) at line %i in %s", \
        signed char: "Require failed - Expected: `%s` (%hhi) Got: `%s` (%hhi) at line %i in %s", \
        unsigned char: "Require failed - Expected: `%s` (%hhu) Got: `%s` (%hhu) at line %i in %s", \
        char: "Require failed - Expected: `%s` (%c) Got: `%s` (%c) at line %i in %s", \
        bool: "Require failed - Expected: `%s` (%i) Got: `%s` (%i) at line %i in %s", \
        default: "Require failed - Expected: `%s` (%p) Got: `%s` (%p) at line %i in %s" \
    )
    
/**
 * @example{lineno} example/gencore/GEN_REQUIRE.c
 * Example for how to use the `GEN_REQUIRE*` family of macros.
 * The `GEN_REQUIRE*` family of macros are intended to provide a nice output and interface for tests.
 * The `GEN_REQUIRE*` family of macros are intended as a Genstone internal utility primarily, but is part of the public API if a program decides to use it.
 */

/**
 * Pretty assertion for equality.
 * @param[in] a the expected expression.
 * @param[in] b the recieved expression.
 * @note Determines type to use correct comparison using `generic` on second argument, this is to avoid having cases for `const`.
 * @note Only works for trivial types.
 */
#define GEN_REQUIRE_EQUAL(a, b) \
    do { \
        if(!__builtin_constant_p(a)) glogf(WARNING, "Expected expression `%s` is not constant at line %i in %s", #a, __LINE__, __FILE__); \
        if(a != b) { \
            glogf(FATAL, GEN_INTERNAL_REQUIRE_EQUAL_MESSAGE(b), #a, a, #b, b, __LINE__, __FILE__); \
            abort(); \
        } \
    } while(0)

/**
 * Pretty assertion for equality of strings.
 * @param[in] a the expected string.
 * @param[in] b the recieved string.
 * @note Use `GEN_REQUIRE_EQUAL_MEMREGION` for non-constant expected values.
 */
#define GEN_REQUIRE_EQUAL_STRING(a, b) \
    do { \
        if(!__builtin_constant_p(a)) glogf(WARNING, "Expected expression `%s` (%s) is not constant at line %i in %s", #a, a, __LINE__, __FILE__); \
        if(!b || strcmp(a, b)) { \
            glogf(FATAL, "Require failed - Expected: `%s` (%s) Got: `%s` (%s) at line %i in %s", #a, a, #b, b, __LINE__, __FILE__); \
            abort(); \
        } \
    } while(0)

/**
 * Pretty assertion for equality of memory regions.
 * @param[in] a the expected data.
 * @param[in] b the recieved data.
 * @param[in] s the amount of data in bytes to compare.
 */
#define GEN_REQUIRE_EQUAL_MEMREGION(a, b, s) \
    do { \
        if((!b && s) || memcmp(a, b, s)) { \
            glogf(FATAL, "Require failed - Expected: `%s` (%p) (%c%c%c...) Got: `%s` (%p) (%c%c%c...) at line %i in %s", #a, a, ((char* const restrict) a)[0], ((char* const restrict) a)[1], ((char* const restrict) a)[2], #b, b, ((char* const restrict) b)[0], ((char* const restrict) b)[1], ((char* const restrict) b)[2], __LINE__, __FILE__); \
            abort(); \
        } \
    } while(0)

/**
 * Pretty assertion for bad control paths.
 */
#define GEN_REQUIRE_NO_REACH glogf(FATAL, "Require failed - Invalid control path reached at line %i in %s", __LINE__, __FILE__)

/**
 * Pretty assertion for `result == GEN_OK`.
 */
#define GEN_REQUIRE_NO_ERROR(result) \
    do { \
        if(result != GEN_OK) { \
            glogf(FATAL, "Require failed - Got error: `%s` (%s) at line %i in %s", gen_error_name(result), gen_error_description(result), __LINE__, __FILE__); \
            abort(); \
        } \
    } while(0)

/**
 * Pretty assertion for truthiness.
 * @param[in] a the recieved expression.
 * @note Only works for trivial types.
 */
#define GEN_REQUIRE_NON_NULL(a) \
    do { \
        if(!a) { \
            glogf(FATAL, "Require failed - %s was NULL at line %i in %s", #a, __LINE__, __FILE__); \
            abort(); \
        } \
    } while(0)

#define GEN_MICROSECONDS_PER_SECOND 1000000
#define GEN_MILLISECONDS_PER_SECOND 1000

#include <sys/time.h>

/**
 * Converts a timeval to seconds as a `long double`.
 * @param[in] timeval the `struct timeval` to convert.
 */
#define GEN_TIMEVAL_AS_SECONDS(timeval) ((long double) (timeval).tv_usec + ((long double) (timeval).tv_sec * (long double) GEN_MICROSECONDS_PER_SECOND))

/**
 * Adds two timevals.
 * @see https://gist.github.com/vchernov/4774682#file-timeval_add-cpp
 * @param[in] a the first value.
 * @param[in] b the second value.
 * @param[out] result pointer to storage for the result.
 */
extern void gen_timeval_add(const struct timeval* const restrict a, const struct timeval* const restrict b, struct timeval* const restrict result);
/**
 * Subtracts two timevals.
 * @see https://gist.github.com/vchernov/4774682#file-timeval_sub-cpp
 * @param[in] a the first value.
 * @param[in] b the second value.
 * @param[out] result pointer to storage for the result.
 */
extern void gen_timeval_sub(const struct timeval* const restrict a, const struct timeval* const restrict b, struct timeval* const restrict result);

#endif
