// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @internal
 * @file gendbg.h
 * Provides debug configuration
 */

#if MODE == DEBUG
#ifndef GEN_DEBUG_PATH_VALIDATION
/**
 * Whether to validate paths passed to genfs functions with `gen_path_validate`
 * @note Does not affect the presence of `gen_path_validate`
 */
#define GEN_DEBUG_PATH_VALIDATION ENABLED
#endif
#ifndef GEN_DEBUG_FOREACH_REGISTER
/**
 * Whether to use register variables for iteration in `GEN_FOREACH` loops
 * @see Disabling this can sometimes help with printing iterator values from a debugger
 */
#define GEN_DEBUG_FOREACH_REGISTER ENABLED
#endif
#else
#ifndef GEN_DEBUG_PATH_VALIDATION
#define GEN_DEBUG_PATH_VALIDATION DISABLED
#endif
#ifndef GEN_DEBUG_FOREACH_REGISTER
#define GEN_DEBUG_FOREACH_REGISTER DISABLED
#endif
#endif
