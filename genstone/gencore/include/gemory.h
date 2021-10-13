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
 * Allocates memory on the heap
 * @param out_address pointer to storage for a pointer into the allocated heap block
 * @param size the size of the unit being allocated for
 * @param count the number of units being allocated for
 * @return an error code
 */
extern GEN_ERRORABLE_RETURN galloc(void** const restrict out_address, const size_t size, const size_t count);
/**
 * Allocates memory on the heap
 * Ensures memory is zeroed before returning
 * @param out_address pointer to storage for a pointer into the allocated heap block
 * @param size the size of the unit being allocated for
 * @param count the number of units being allocated for
 * @return an error code
 */
extern GEN_ERRORABLE_RETURN gzalloc(void** const restrict out_address, const size_t size, const size_t count);
/**
 * Resizes a memory block on the heap
 * May move items if new size is bigger than the old block allows
 * @param address the heap pointer to resize. May be resized if block needs to be moved
 * @param size the size of the unit being allocated for
 * @param count the number of units being allocated for
 * @return an error code
 */
extern GEN_ERRORABLE_RETURN grealloc(void** const restrict address, const size_t size, const size_t count);
/**
 * Frees a block of heap memory
 * @param address the heap pointer to free
 * @note contents of the block become undefined after freeing
 * @return an error code
 */
extern GEN_ERRORABLE_RETURN gfree(void* const restrict address);

#endif
