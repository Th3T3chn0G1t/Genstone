// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_TOOLING_FRAME_H
#define GEN_TOOLING_FRAME_H

#ifndef GEN_TOOLING_DEPTH
#define GEN_TOOLING_DEPTH 1024
#endif

typedef struct {
    const char* function;
    const void* address;
    const char* file;
} gen_tooling_frame_t;

typedef gen_tooling_frame_t gen_backtrace_t[GEN_TOOLING_DEPTH];

#endif
