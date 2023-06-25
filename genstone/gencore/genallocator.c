// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genallocator.h"

#include <genbackends.h>

GEN_BACKENDS_PROC(get_system_allocator, gen_error_t*)
gen_error_t* gen_get_system_allocator(
        gen_system_allocator_t* const restrict out_allocator) {

    gen_tooling_push(GEN_FUNCTION_NAME, GEN_FILE_NAME);
    GEN_TOOLING_AUTO gen_error_t* error;

    if(!out_allocator) {
        return gen_error_attach_backtrace(
                GEN_ERROR_INVALID_PARAMETER, GEN_LINE_STRING,
                "`out_allocator` was `GEN_NULL`");
    }

    return gen_backends_get_system_allocator(out_allocator);
}
