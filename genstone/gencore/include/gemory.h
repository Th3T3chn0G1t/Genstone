// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gemory.h
 * Provides wrappers and abstraction for heap functions
 * Utility for managing memory
 */

#ifndef GEN_MEMORY_H
#define GEN_MEMORY_H

/**
 * Gemory does not permit the allocation of non-zeroed memory due to potential for security relevant bugs to arise from improperly initialized memory
 * Also can help prevent UB if the programmer is careless :^)
 */
#define galloc gzalloc

/**
 * Allocates memory on the heap
 * Ensures memory is zeroed before returning
 * @param address pointer to storage for a pointer into the allocated heap block
 * @param size the size of the unit being allocated for
 * @param count the number of units being allocated for
 * @return an error code
 */
extern GEN_ERRORABLE_RETURN gzalloc(void* restrict * const restrict out_address, const size_t size, const size_t count);
/**
 * Resizes a memory block on the heap
 * May move items if new size is bigger than the old block allows
 * @param address the heap pointer to resize. May be modified if block needs to be moved
 * @param size the size of the unit being allocated for
 * @param count the number of units being allocated for
 * @return an error code
 */
extern GEN_ERRORABLE_RETURN grealloc(void* restrict * const restrict address, const size_t size, const size_t count);
/**
 * Duplicates a string onto the heap
 * @param address pointer to storage for a pointer into the allocated heap block
 * @param str the string to duplicate
 * @param max the maximum number of characters to duplicate
 */
extern GEN_ERRORABLE_RETURN gstrndup(char* restrict * const restrict out_address, const char* const restrict str, const size_t max);
/**
 * Frees a block of heap memory
 * @param address the heap pointer to free
 * @note Contents of the block become undefined after freeing
 * @return an error code
 */
extern GEN_ERRORABLE_RETURN gfree(void* const restrict address);

#endif
