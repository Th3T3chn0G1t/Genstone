// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_VERSION_H
#define GEN_VERSION_H

#include "gencommon.h"

// TODO: Proper documentation here

typedef struct {
    gen_size_t major;
    gen_size_t minor;
    gen_size_t patch;
} gen_version_t;

typedef enum {
    GEN_VERSION_NEWER,
    GEN_VERSION_OLDER,
    GEN_VERSION_SAME
} gen_version_comparison_result_t;

/**
 * Compares two versions. Output is in the form "Yours is X than theirs", where X is the result enumeration produced.
 */
extern gen_error_t* gen_version_compare(const gen_version_t* const restrict yours, const gen_version_t* const restrict theirs, gen_version_comparison_result_t* const restrict result);

#endif
