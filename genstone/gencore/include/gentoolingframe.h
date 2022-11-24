// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_TOOLING_FRAME_H
#define GEN_TOOLING_FRAME_H

#ifndef GEN_TOOLING_DEPTH
/**
 * The maximum depth of a tooled call stack.
 */
#define GEN_TOOLING_DEPTH 1024
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

typedef gen_tooling_frame_t gen_tooling_backtrace_t[GEN_TOOLING_DEPTH];

#endif
