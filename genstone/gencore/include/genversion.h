// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_VERSION_H
#define GEN_VERSION_H

#include "gencommon.h"

/**
 * Representation of a semver version.
 */
typedef struct {
	/**
	 * The major version number.
	 */
    gen_size_t major;
	/**
	 * The minor version number.
	 */
    gen_size_t minor;
	/**
	 * The version patch number.
	 */
    gen_size_t patch;
} gen_version_t;

/**
 * The result of a version comparison.
 */
typedef enum {
	/**
	 * The version is newer.
	 */
    GEN_VERSION_NEWER,
	/**
	 * The version is older.
	 */
    GEN_VERSION_OLDER,
	/**
	 * The version is the same.
	 */
    GEN_VERSION_SAME
} gen_version_comparison_result_t;

/**
 * Compares two versions. Output is in the form "Yours is X than theirs", where X is the result enumeration produced.
 * @param[in] yours The first version in the comparison.
 * @param[in] theirs The second version in the comparison.
 * @param[out] result The result of the comparison.
 */
extern gen_error_t* gen_version_compare(const gen_version_t* const restrict yours, const gen_version_t* const restrict theirs, gen_version_comparison_result_t* const restrict result);

#endif
