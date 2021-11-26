// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gendbg.h
 * Provides debug configuration.
 */

#ifndef GEN_DEBUG_H
#define GEN_DEBUG_H

#if MODE == DEBUG
#ifndef GEN_DEBUG_PATH_VALIDATION
/**
 * Whether to validate paths passed to `genfs` functions with `gen_path_validate`.
 * @note Does not affect the presence of `gen_path_validate`.
 */
#define GEN_DEBUG_PATH_VALIDATION ENABLED
#endif
#ifndef GEN_DEBUG_FOREACH_REGISTER
/**
 * Whether to use register variables for iteration in `GEN_FOREACH` statements.
 * @note Disabling this can sometimes help with printing iterator values from a debugger.
 */
#define GEN_DEBUG_FOREACH_REGISTER DISABLED
#endif
#else
#ifndef GEN_DEBUG_PATH_VALIDATION
#define GEN_DEBUG_PATH_VALIDATION DISABLED
#endif
#ifndef GEN_DEBUG_FOREACH_REGISTER
#define GEN_DEBUG_FOREACH_REGISTER ENABLED
#endif
#endif

#endif
