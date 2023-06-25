// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genallocator.h>

#include <genbackends.h>

#include <stdlib.h>

GEN_USED gen_error_t* gen_libc_get_system_allocator(
        gen_system_allocator_t* const restrict out_allocator) {

    out_allocator->malloc = (gen_malloc_t) malloc;
    out_allocator->calloc = (gen_calloc_t) calloc;
    out_allocator->aligned_alloc = (gen_aligned_alloc_t) aligned_alloc;
    out_allocator->realloc = (gen_realloc_t) realloc;
    out_allocator->free = (gen_free_t) free;

    return GEN_NULL;
}
