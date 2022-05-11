// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gemory.h
 * Provides wrappers and abstraction for heap functions.
 * Utility for managing memory.
 */

#ifndef GEN_MEMORY_H
#define GEN_MEMORY_H

/**
 * Allocates memory on the heap.
 * Ensures memory is zeroed before returning.
 * @param[out] out_address pointer to storage for a pointer into the allocated heap block.
 * @param[in] count the number of units being allocated for.
 * @param[in] size the size of the unit being allocated for.
 * @return an error code.
 */
GEN_ERRORABLE gzalloc(void* restrict* const restrict out_address, const size_t count, const size_t size);
/**
 * Allocates memory on the heap.
 * Ensures memory is zeroed before returning.
 * Ensures memory is aligned to correct alignment.
 * @param[out] out_address pointer to storage for a pointer into the allocated heap block.
 * @param[in] count the number of units being allocated for.
 * @param[in] size the size of the unit being allocated for.
 * @param[in] align the alignment to align allocation to.
 * @return an error code.
 */
GEN_ERRORABLE gzalloc_aligned(void* restrict* const restrict out_address, const size_t count, const size_t size, const size_t align);
/**
 * Resizes a memory block on the heap.
 * May move items if new size is bigger than the old block allows.
 * @param[in,out] out_address the heap pointer to resize. May be modified if block needs to be moved.
 * @param[in] old_count the previous number of units.
 * @param[in] count the number of units being allocated for.
 * @param[in] size the size of the unit being allocated for.
 * @return an error code.
 */
GEN_ERRORABLE grealloc(void* restrict* const restrict out_address, const size_t old_count, const size_t count, const size_t size);
/**
 * Frees a block of heap memory.
 * @param[in] address the heap pointer to free.
 * @note Contents of the block become undefined after freeing.
 * @return an error code.
 */
GEN_ERRORABLE gfree(void* const restrict address);

/**
 * Sets a block of memory to a value.
 * @param[out] address the buffer to set.
 * @param[in] length the length of the buffer to set in bytes.
 * @param[in] value the value to set the buffer to.
 * @return an error code.
 */
GEN_ERRORABLE gen_memory_set(void* const restrict address, const size_t length, const unsigned char value);

/**
 * Copies the data from one block of memory to another.
 * @param[in] from the block from which to source the data.
 * @param[in] from_size the size of the source block in bytes.
 * @param[out] to the block to which to copy the data.
 * @param[in] to_size the size of the destination block in bytes.
 * @param[in] limit the number of bytes to copy.
 * @return an error code.
 */
GEN_ERRORABLE gen_memory_copy(const void* const restrict from, const size_t from_size, void* const restrict to, const size_t to_size, const size_t limit);

#endif
