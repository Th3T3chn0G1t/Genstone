// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gentoolingframe.h
 * Provides utilities for tooling and profiling.
 */

#ifndef GEN_TOOLING_FRAME_H
#define GEN_TOOLING_FRAME_H

/**
 * A single frame of tooling.
 */
typedef struct {
    /**
     * The function name.
     */
    const char* function;

    /**
     * The address.
     */
    const void* address;

    /**
     * The file name.
     */
    const char* file;
} gen_tooling_frame_t;

#endif
