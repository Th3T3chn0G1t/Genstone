// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

/**
 * @file gencommon.h
 * Includes common helpers and utilities for across the codebase.
 */

#ifndef GEN_COMMON_H
#define GEN_COMMON_H

/**
 * Begins a diagnostic region.
 */
#define GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN clang diagnostic push

/**
 * Ends a diagnostic region.
 */
#define GEN_PRAGMA_DIAGNOSTIC_REGION_END clang diagnostic pop

/**
 * Ignores a class of compiler warnings in a diagnostic region.
 * @param[in] w The compiler flag for the warning to ignore.
 */
#define GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE(w) clang diagnostic ignored w

/**
 * Forcibly stringifies its parameter. Used for preprocessor ordering shenanigans.
 * @param[in] s The parameter to be stringified.
 */
#define GEN_STRINGIFY(s) #s

/**
 * Wraps `#pragma`/`_Pragma` to allow use of `GEN_PRAGMA_*` wrappers.
 * @param[in] p The pragma statement to wrap.
 */
#define GEN_PRAGMA(p) _Pragma(GEN_STRINGIFY(p))

/**
 * Pretty wrapper for `__FILE__`.
 */
#define GEN_FILE_NAME __FILE__

/**
 * Pretty wrapper for `__LINE__`.
 */
#define GEN_LINE_NUMBER __LINE__

/**
 * Pretty wrapper for `__func__`.
 */
#define GEN_FUNCTION_NAME __func__

/**
 * Pretty wrapper for `_Noreturn`. Alternative to `stdnoreturn.h` which breaks `Windows.h`.
 */
#define GEN_NORETURN _Noreturn

/**
 * Pretty wrapper for `__attribute__((unused))`.
 */
#define GEN_UNUSED __attribute__((unused))

/**
 * Pretty wrapper for `__attribute__((packed))`.
 */
#define GEN_PACKED __attribute__((packed))

/**
 * Pretty wrapper for `__attribute__((no_sanitize_address)) __attribute__((no_sanitize_memory))`
 */
#define GEN_DONT_SANITIZE __attribute__((no_sanitize_address)) __attribute__((no_sanitize_memory))

/**
 * Pretty wrapper for `__attribute__((constructor))`
 */
#define GEN_INITIALIZER __attribute__((constructor))

#ifndef __INTELLISENSE
/**
 * Pretty wrapper for `__attribute__((maybe_unused))`.
 */
#define GEN_MAYBE_UNUSED [[maybe_unused]]
#else
#define GEN_MAYBE_UNUSED
#endif

/**
 * Pretty wrapper for `_Generic`.
 */
#define GEN_GENERIC _Generic

/**
 * Pretty wrapper for `__attribute__((cleanup))`.
 * @param[in] function The cleanup function to call.
 */
#define GEN_CLEANUP_FUNCTION(function) __attribute__((cleanup(function)))

/**
 * Returns the lowest of 2 values.
 * @param[in] a The first value.
 * @param[in] b The second value.
 * @return The lower value.
 */
#define GEN_MINIMUM(a, b) ((a) < (b) ? (a) : (b))

/**
 * Returns the highest of 2 values.
 * @param[in] a The first value.
 * @param[in] b The second value.
 * @return The higher value.
 */
#define GEN_MAXIMUM(a, b) ((a) > (b) ? (a) : (b))

#ifndef GEN_FORCE_UNIX
/**
 * Forces genstone backends to use Unix-compatible APIs.
 */
#define GEN_FORCE_UNIX GEN_DISABLED
#endif

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>
#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
#include <sys/types.h>
#else
typedef long ssize_t;
#endif
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

#include "generror.h"
#include "gentooling.h"

#endif
