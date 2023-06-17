// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_FORMAT_H
#define GEN_FORMAT_H

#include "gencommon.h"

typedef gen_size_t gen_format_count_t;

#define GEN_FORMAT_COUNT(l) ((gen_format_count_t) (l))

gen_error_t* gen_format(
        char* const restrict out_buffer, gen_size_t* const restrict out_len,
        const gen_size_t limit, const char* const restrict format, ...);

gen_error_t* gen_format_variadic_list(
        char* const restrict out_buffer, gen_size_t* const restrict out_len,
        const gen_size_t limit, const char* const restrict format,
        gen_variadic_list_t list);

#endif
