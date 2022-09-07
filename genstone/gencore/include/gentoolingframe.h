// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

/**
 * @file gentoolingframe.h
 * Provides common definitions for tooling between generror and gentooling.
 */

#ifndef GEN_TOOLING_FRAME_H
#define GEN_TOOLING_FRAME_H

#ifndef GEN_TOOLING_DEPTH
/**
 * The maximum depth of a tooled call stack.
 */
#define GEN_TOOLING_DEPTH 256
#endif

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