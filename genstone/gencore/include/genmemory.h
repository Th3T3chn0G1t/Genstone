// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_MEMORY_H
#define GEN_MEMORY_H

#include "gencommon.h"

/**
 * A value which can be used in place of buffer bounds in the event that no bounding should be performed.
 */
#define GEN_MEMORY_NO_BOUNDS GEN_SIZE_MAX

/**
 * Allocates memory on the heap.
 * Ensures memory is zeroed before returning.
 * @param[out] out_address A pointer to storage for a pointer into the allocated heap block.
 * @param[in] count The number of units to be allocated.
 * @param[in] size The size of the unit being allocated for.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_memory_allocate_zeroed(void* restrict* const restrict out_address, const gen_size_t count, const gen_size_t size);

/**
 * Allocates memory on the heap.
 * Ensures memory is zeroed before returning.
 * Ensures memory is aligned to correct alignment.
 * @param[out] out_address A pointer to storage for a pointer into the allocated heap block.
 * @param[in] count The number of units to be allocated.
 * @param[in] size The size of the unit being allocated for.
 * @param[in] alignment The alignment to align the allocation to.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_memory_allocate_zeroed_aligned(void* restrict* const restrict out_address, const gen_size_t count, const gen_size_t size, const gen_size_t alignment);

/**
 * Resizes a memory block on the heap.
 * Ensures allocated memory is zeroed before returning.
 * May move items if new size is bigger than the old block allows.
 * @param[in,out] address A pointer to the heap pointer to resize. May be modified if block needs to be moved.
 * @param[in] old_count The previous number of units in the block.
 * @param[in] count The number of units to be allocated.
 * @param[in] size The size of the unit being allocated for.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_memory_reallocate_zeroed(void* restrict* const restrict address, const gen_size_t old_count, const gen_size_t count, const gen_size_t size);

/**
 * Frees a block of heap memory.
 * @note Contents of the block become undefined after freeing.
 * @param[in,out] address A pointer to the heap pointer to free. Set to `GEN_NULL` once the block is freed.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_memory_free(void* restrict* const restrict address);

/**
 * Sets a block of memory to a value.
 * @param[out] address The buffer to set.
 * @param[in] bounds The bounds of `buffer`.
 * @param[in] count The amount of `buffer` to set to `value`.
 * @param[in] value The value to set.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_memory_set(void* const restrict address, const gen_size_t bounds, const gen_size_t count, const unsigned char value);

/**
 * Copies the data from one block of memory to another.
 * @param[out] to The block to copy the data to.
 * @param[in] to_bounds The bounds of `to`.
 * @param[in] from The block to copy the data from.
 * @param[in] from_bounds The bounds of `from`.
 * @param[in] count The amount to copy.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_memory_copy(void* const restrict to, const gen_size_t to_bounds, const void* const restrict from, const gen_size_t from_bounds, const gen_size_t count);

/**
 * Compares two blocks of memory.
 * @param[in] a The first block of memory to compare.
 * @param[in] a_bounds The bounds of `a`.
 * @param[in] b The second block of memory to compare.
 * @param[in] b_bounds The bounds of `b`.
 * @param[in] count The amount to compare.
 * @param[out] out_equal A pointer to storage for the result of whether the two blocks are equal.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_memory_compare(const void* const restrict a, const gen_size_t a_bounds, const void* const restrict b, const gen_size_t b_bounds, const gen_size_t count, gen_bool_t* const restrict out_equal);

#endif
