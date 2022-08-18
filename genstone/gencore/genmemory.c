// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gemory.h"
#include "include/gencommon.h"

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <stdlib.h>
#include <string.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

#ifndef GEN_MEMORY_CALLOC
/**
 * The function to use for allocation.
 * Must match the signature of `calloc` and follow equivalent semantics.
 */
#define GEN_MEMORY_CALLOC calloc
#endif
#ifndef GEN_MEMORY_ALIGNED_ALLOC
/**
 * The function to use for aligned allocation.
 * Must match the signature of `aligned_alloc` and follow equivalent semantics.
 */
#define GEN_MEMORY_ALIGNED_ALLOC aligned_alloc
#endif
#ifndef GEN_MEMORY_REALLOC
/**
 * The function to use for reallocation.
 * Must match the signature of `realloc` and follow equivalent semantics.
 */
#define GEN_MEMORY_REALLOC realloc
#endif
#ifndef GEN_MEMORY_FREE
/**
 * The function to use for freeing allocations.
 * Must match the signature of `free` and follow equivalent semantics.
 */
#define GEN_MEMORY_FREE free
#endif

gen_error_t gen_allocate_zeroed(void* restrict* const restrict out_address, const size_t count, const size_t size) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_allocate_zeroed", (void*) gen_allocate_zeroed, GEN_FILENAME);
	if(error.type) return error;

	if(!out_address) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`out_address` was `NULL`");
	if(!size) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`size` was 0");
	if(!count) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`count` was 0");
	if(size == SIZE_MAX) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`size` was `SIZE_MAX`");
	if(count == SIZE_MAX)  return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`count` was `SIZE_MAX`");

	void* const allocated = GEN_MEMORY_CALLOC(count, size);
	if(!allocated) return gen_error_attach_backtrace(gen_error_from_errno(), "`" GEN_STRINGIFY(GEN_MEMORY_CALLOC) "` returned `NULL`");

	*out_address = allocated;

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_allocate_zeroed_aligned(void* restrict* const restrict out_address, const size_t count, const size_t size, const size_t alignment) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_allocate_zeroed_aligned", (void*) gen_allocate_zeroed_aligned, GEN_FILENAME);
	if(error.type) return error;

	if(!out_address) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`out_address` was `NULL`");
	if(!size) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`size` was 0");
	if(!count) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`count` was 0");
	if(size == SIZE_MAX) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`size` was `SIZE_MAX`");
	if(count == SIZE_MAX)  return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`count` was `SIZE_MAX`");
	if(!alignment) gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`alignment` was 0");
	if(alignment & (alignment - 1)) gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`alignment` was not a power of 2");

	// Size must be a multiple of alignment
	const size_t aligned_size = (((count * size) / alignment) + 1) * alignment;

	void* const allocated = GEN_MEMORY_ALIGNED_ALLOC(alignment, aligned_size);
	if(!allocated) return gen_error_attach_backtrace(gen_error_from_errno(), "`" GEN_STRINGIFY(GEN_MEMORY_ALIGNED_ALLOC) "` returned `NULL`");

	*out_address = allocated;

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_reallocate_zeroed(void* restrict* const restrict address, const size_t old_count, const size_t count, const size_t size) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_reallocate_zeroed", (void*) gen_reallocate_zeroed, GEN_FILENAME);
	if(error.type) return error;

	if(!address) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`address` was `NULL`");
	if(!size) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`size` was 0");
	if(!count) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`count` was 0");
	if(size == SIZE_MAX) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`size` was `SIZE_MAX`");
	if(count == SIZE_MAX)  return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`count` was `SIZE_MAX`");
	
	void* const allocated = GEN_MEMORY_REALLOC(*address, count * size);
	if(!allocated) return gen_error_attach_backtrace(gen_error_from_errno(), "`" GEN_STRINGIFY(GEN_MEMORY_REALLOC) "` returned `NULL`");
	if(count > old_count) {
		error = gen_memory_set(allocated + (size * old_count), size * (count - old_count), 0);
		if(error.type) return error;
	}

	*address = allocated;

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_free(void* restrict* const restrict address) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_free", (void*) gen_free, GEN_FILENAME);
	if(error.type) return error;

	if(!address) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`address` was `NULL`");

	GEN_MEMORY_FREE(*address);

	*address = NULL;

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_memory_set(void* const restrict address, const size_t length, const unsigned char value) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_memory_set", (void*) gen_memory_set, GEN_FILENAME);
	if(error.type) return error;

	if(!address) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`address` was `NULL`");

	memset(address, 0, value);

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_memory_copy(const void* const restrict from, const size_t from_size, void* const restrict to, const size_t to_size, const size_t limit) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_memory_copy", (void*) gen_memory_copy, GEN_FILENAME);
	if(error.type) return error;

	if(!from) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`from` was `NULL`");
	if(!to) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, "`to` was `NULL`");
	if(limit > from_size) return gen_error_attach_backtrace(GEN_OUT_OF_BOUNDS, "`limit` exceeded `from_size`");
	if(limit > to_size) return gen_error_attach_backtrace(GEN_OUT_OF_BOUNDS, "`limit` exceeded `to_size`");

	memcpy(to, from, limit);

	return (gen_error_t){GEN_OK, ""};
}
