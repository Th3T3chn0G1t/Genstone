// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_ALLOCATOR_H
#define GEN_ALLOCATOR_H

#include "gencommon.h"

typedef void* (*gen_malloc_t)(gen_size_t);
typedef void* (*gen_calloc_t)(gen_size_t, gen_size_t);
typedef void* (*gen_aligned_alloc_t)(gen_size_t, gen_size_t);
typedef void* (*gen_realloc_t)(void*, gen_size_t);
typedef void (*gen_free_t)(void*);

typedef struct {
    gen_malloc_t malloc;
    gen_calloc_t calloc;
    gen_aligned_alloc_t aligned_alloc;
    gen_realloc_t realloc;
    gen_free_t free;
} gen_system_allocator_t;

gen_error_t* gen_get_system_allocator(
                gen_system_allocator_t* const restrict out_allocator);

typedef struct {
    gen_size_t block_size;

    gen_size_t chunk_size;
    gen_uint8_t* chunk;

    gen_size_t used_blocks;
} gen_allocator_chunk_t;

//gen_error_t* gen_allocator_allocate

#endif
