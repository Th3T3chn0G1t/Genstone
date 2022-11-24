// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

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
 * Pretty wrapper for `__attribute__((no_sanitize_address))`.
 */
#define GEN_DONT_SANITIZE_ADDRESS __attribute__((no_sanitize_address))

/**
 * Pretty wrapper for `__attribute__((constructor))`.
 */
#define GEN_INITIALIZER __attribute__((constructor))

/**
 * Defines a symbol alias.
 * The specific behaviour of this is highly platform dependent.
 */
#define GEN_ALIAS(symbol, underlying) extern __typeof__(underlying) symbol __attribute__((weak, alias(#underlying)))

#ifndef __INTELLISENSE
/**
 * Pretty wrapper for `[[maybe_unused]]`.
 */
#define GEN_MAYBE_UNUSED [[maybe_unused]]
#else
#define GEN_MAYBE_UNUSED
#endif

#ifndef __INTELLISENSE
/**
 * Pretty wrapper for `[[fallthrough]]`.
 */
#define GEN_FALLTHROUGH [[fallthrough]]
#else
#define GEN_FALLTHROUGH
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

// These are all here to prevent the need for includes in the public API.

/**
 * Pretty type for `_Bool`.
 */
typedef _Bool gen_bool_t;
/**
 * Truthy value.
 */
#define gen_true (1)
/**
 * Falsy value.
 */
#define gen_false (0)

/**
 * Size type.
 */
typedef unsigned long long gen_size_t;

/**
 * Maximum value of a `gen_size_t`.
 */
#define GEN_SIZE_MAX ((gen_size_t) -1)

/**
 * Signed size type.
 */
typedef long long gen_ssize_t;

/**
 * Maximum value of a `gen_ssize_t`.
 */
#define GEN_SSIZE_MAX ((gen_ssize_t) 0x7FFFFFFFFFFFFFFF)

/**
 * 32-bit unsigned integer type.
 */
typedef unsigned int gen_uint32_t;

/**
 * 8-bit unsigned integer type.
 */
typedef unsigned char gen_uint8_t;

#ifndef GEN_THREAD_LOCAL
/**
 * Pretty wrapper for `_Thread_local`.
 */
#define GEN_THREAD_LOCAL _Thread_local
#endif

/**
 * Pretty wrapper for `_Alignas`.
 */
#define GEN_ALIGNAS(x) _Alignas(x)

/**
 * Pretty wrapper for `_Alignof`.
 */
#define GEN_ALIGNOF(x) _Alignof(x)

/**
 * Null pointer value.
 */
#define GEN_NULL ((void*) (0))

/**
 * Pretty type for `gen_variadic_list_t`.
 */
typedef __builtin_va_list gen_variadic_list_t;

/**
 * Pretty wrapper for `__builtin_va_arg`.
 */
#define gen_variadic_list_argument(list, type) __builtin_va_arg(list, type)

/**
 * Pretty wrapper for `__builtin_va_start`.
 * TODO: Update once C2x relaxes `va_start`.
 */
#define gen_variadic_list_start(list, param) __builtin_va_start(list, param)

/**
 * Pretty wrapper for `__builtin_va_end`.
 */
#define gen_variadic_list_end(list) __builtin_va_end(list)

/**
 * Pretty wrapper for `__builtin_va_copy`.
 */
#define gen_variadic_list_copy(to, from) __builtin_va_copy(to, from)

#include "generror.h"
#include "gentooling.h"

#endif
