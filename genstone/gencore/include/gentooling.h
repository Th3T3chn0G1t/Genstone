// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_TOOLING_H
#define GEN_TOOLING_H

#include "gentoolingframe.h"

void gen_tooling_internal_auto_cleanup(
        GEN_UNUSED const void* const restrict p);

#define GEN_TOOLING_AUTO \
    GEN_CLEANUP_FUNCTION(gen_tooling_internal_auto_cleanup)

void gen_tooling_push(
        const char* const restrict frame, const char* const restrict file);
void gen_tooling_pop(void);

void gen_tooling_get_backtrace(
        gen_tooling_frame_t* const restrict out_backtrace,
        gen_size_t* const restrict out_length);

#endif
