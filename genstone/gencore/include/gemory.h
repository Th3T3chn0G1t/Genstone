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
 * Gemory does not permit the allocation of non-zeroed memory due to potential for security relevant bugs to arise from improperly initialized memory.
 * Also can help prevent UB if the programmer is careless :^).
 */
#define galloc gzalloc
/**
 * Gemory does not permit the allocation of non-zeroed memory due to potential for security relevant bugs to arise from improperly initialized memory.
 * Also can help prevent UB if the programmer is careless :^).
 */
#define galloc_aligned gzalloc_aligned

/**
 * Allocates memory on the heap.
 * Ensures memory is zeroed before returning.
 * @param[out] out_address pointer to storage for a pointer into the allocated heap block.
 * @param[in] size the size of the unit being allocated for.
 * @param[in] count the number of units being allocated for.
 * @return an error code.
 */
GEN_ERRORABLE gzalloc(void* restrict* const restrict out_address, const size_t size, const size_t count);
/**
 * Allocates memory on the heap.
 * Ensures memory is zeroed before returning.
 * Ensures memory is aligned to correct alignment.
 * @param[out] out_address pointer to storage for a pointer into the allocated heap block.
 * @param[in] size the size of the unit being allocated for.
 * @param[in] count the number of units being allocated for.
 * @param[in] align the alignment to align allocation to.
 * @return an error code.
 */
GEN_ERRORABLE gzalloc_aligned(void* restrict* const restrict out_address, const size_t size, const size_t count, const size_t align);
/**
 * Resizes a memory block on the heap.
 * May move items if new size is bigger than the old block allows.
 * @param[in,out] out_address the heap pointer to resize. May be modified if block needs to be moved.
 * @param[in] size the size of the unit being allocated for.
 * @param[in] count the number of units being allocated for.
 * @return an error code.
 */
GEN_ERRORABLE grealloc(void* restrict* const restrict out_address, const size_t size, const size_t count);
/**
 * Frees a block of heap memory.
 * @param[in] address the heap pointer to free.
 * @note Contents of the block become undefined after freeing.
 * @return an error code.
 */
GEN_ERRORABLE gfree(void* const restrict address);

#endif
