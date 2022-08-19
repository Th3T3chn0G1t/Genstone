// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

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
 * @param w The compiler flag for the warning to ignore.
 */
#define GEN_DIAGNOSTIC_REGION_IGNORE(w) clang diagnostic ignored w
/**
 * Forcibly stringifies its parameter. Used for preprocessor ordering shenanigans.
 * @param s The parameter to be stringified.
 */
#define GEN_STRINGIFY(s) #s
/**
 * Wraps `#pragma`/`_Pragma` to allow use of `GEN_PRAGMA_*` wrappers.
 * @param p The pragma statement to wrap.
 */
#define GEN_PRAGMA(p) _Pragma(GEN_STRINGIFY(p))
/**
 * Pretty wrapper for `__FILE__`.
 */
#define GEN_FILENAME __FILE__
/**
 * Pretty wrapper for `__LINE__`.
 */
#define GEN_LINENO __LINE__

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

#include "generror.h"
#include "gentooling.h"

#endif
