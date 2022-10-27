// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

/**
 * @file genmemory.h
 * Provides wrappers and abstraction for heap functions.
 * Utility for managing memory.
 */

#ifndef GEN_MEMORY_H
#define GEN_MEMORY_H

#include "gencommon.h"

/**
 * A value which can be used in place of buffer bounds in the event that no bounding should be performed.
 */
#define GEN_MEMORY_NO_BOUNDS SIZE_MAX

/**
 * Allocates memory on the heap.
 * Ensures memory is zeroed before returning.
 * @param[out] out_address A pointer to storage for a pointer into the allocated heap block.
 * @param[in] count The number of units being allocated for.
 * @param[in] size The size of the unit being allocated for.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_memory_allocate_zeroed(void* restrict* const restrict out_address, const size_t count, const size_t size);

/**
 * Allocates memory on the heap.
 * Ensures memory is zeroed before returning.
 * Ensures memory is aligned to correct alignment.
 * @param[out] out_address A pointer to storage for a pointer into the allocated heap block.
 * @param[in] count The number of units being allocated for.
 * @param[in] size The size of the unit being allocated for.
 * @param[in] alignment The alignment to align the allocation to.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_memory_allocate_zeroed_aligned(void* restrict* const restrict out_address, const size_t count, const size_t size, const size_t alignment);

/**
 * Resizes a memory block on the heap.
 * Ensures allocated memory is zeroed before returning.
 * May move items if new size is bigger than the old block allows.
 * @param[in,out] address The heap pointer to resize. May be modified if block needs to be moved.
 * @param[in] old_count The previous number of units in the block.
 * @param[in] count The number of units being allocated for.
 * @param[in] size The size of the unit being allocated for.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_memory_reallocate_zeroed(void* restrict* const restrict address, const size_t old_count, const size_t count, const size_t size);

/**
 * Frees a block of heap memory.
 * @note Contents of the block become undefined after freeing.
 * @param[in,out] address The heap pointer to free. Set to `NULL` once the block is freed.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_memory_free(void* restrict* const restrict address);

/**
 * Sets a block of memory to a value.
 * @param[out] address The buffer to set.
 * @param[in] length The length of the buffer to set in bytes.
 * @param[in] value The value to set the buffer to.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_memory_set(void* const restrict address, const size_t length, const unsigned char value);

/**
 * Copies the data from one block of memory to another.
 * @param[out] to The block to which to copy the data.
 * @param[in] to_size The size of the destination block in bytes.
 * @param[in] from The block from which to source the data.
 * @param[in] from_size The size of the source block in bytes.
 * @param[in] limit The number of bytes to copy.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_memory_copy(void* const restrict to, const size_t to_size, const void* const restrict from, const size_t from_size, const size_t limit);

/**
 * Compares two blocks of memory.
 * @param[in] a The first block of memory to compare.
 * @param[in] a_bounds The bounds of the first block of memory to compare.
 * @param[in] b The second block of memory to compare.
 * @param[in] b_bounds The bounds of the second block of memory to compare.
 * @param[in] limit The number of bytes to compare.
 * @param[out] out_equal A pointer to storage for the result of whether the two blocks are equal.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_memory_compare(const void* const restrict a, const size_t a_bounds, const void* const restrict b, const size_t b_bounds, const size_t limit, bool* const restrict out_equal);

#endif
