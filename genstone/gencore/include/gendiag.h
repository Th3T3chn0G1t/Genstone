// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gendiag.h
 * Includes diagnostic utilities and compiler-specific intrinsics.
 */

#ifndef GEN_DIAG_H
#define GEN_DIAG_H

/**
 * Pretty keyword for `_Pragma`.
 */
#define pragma _Pragma
/**
 * Pretty keyword for `_Generic`.
 */
#define generic _Generic

/**
 * Begins a diagnostic region.
 */
#define GEN_DIAG_REGION_BEGIN pragma("clang diagnostic push")
/**
 * Ends a diagnostic region.
 */
#define GEN_DIAG_REGION_END pragma("clang diagnostic pop")
/**
 * Ignores all extra compiler warnings in a diagnostic region.
 */
#define GEN_DIAG_IGNORE_ALL pragma("clang diagnostic ignored \"-Weverything\"")

/**
 * Ignores floating point equality test compiler warnings in a diagnostic region.
 */
#define GEN_DIAG_IGNORE_FP_EQ pragma("clang diagnostic ignored \"-Wfloat-equal\"")

/**
 * Ignores deprecation compiler warnings in a diagnostic region.
 * Pretty much solely for MS Secure CRT warnings.
 */
#define GEN_DIAG_IGNORE_DEPRECATION pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")

GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#ifndef __unused
/**
 * Defines `__unused` on platforms which do not support it by default.
 * Marks a variable or function as unused.
 */
#define __unused __attribute__((unused))
#endif
#ifndef __inline
/**
 * Defines `__inline` on platforms which do not support it by default.
 * Marks a function to be inlined (force inline).
 */
#define __inline __attribute__((always_inline)) __attribute__((artificial)) inline
#endif
#ifndef __deprecated
/**
 * Defines `__deprecated` on platforms which do not support it by default.
 * Marks a function as deprecated.
 */
#define __deprecated(msg) __attribute__((deprecated(msg)))
#endif
#ifndef __unavailable
/**
 * Defines `__deprecated` on platforms which do not support it by default.
 * Marks a function as unavailable.
 */
#define __unavailable(msg) __attribute__((unavailable(msg)))
#endif
#ifndef __nodiscard
/**
 * Defines `__nodiscard` on platforms which do not support it by default.
 * Marks a function return value as not to be discarded.
 */
#define __nodiscard __attribute__((warn_unused_result))
#endif
#ifndef __used
/**
 * Defines `__used` on platforms which do not support it by default.
 * Marks a function as being used to the compiler.
 */
#define __used __attribute__((used))
#endif
#ifndef __packed
/**
 * Defines `__packed` on platforms which do not support it by default.
 * Marks a structure or array type to exclude padding.
 */
#define __packed __attribute__((packed))
#endif
GEN_DIAG_REGION_END

#endif
