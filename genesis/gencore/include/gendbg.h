// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @internal
 * @file gendbg.h
 * Provides debug configuration
 */

#if MODE == DEBUG
#ifndef GEN_FS_PATH_VALIDATION
#define GEN_FS_PATH_VALIDATION ENABLED
#endif
#else
#ifndef GEN_FS_PATH_VALIDATION
#define GEN_FS_PATH_VALIDATION DISABLED
#endif
#endif
