// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genmemory.h"

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

gen_error_t* gen_memory_allocate_zeroed(void* restrict* const restrict out_address, const size_t count, const size_t size) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_allocate_zeroed, GEN_FILE_NAME);
	if(error) return error;

	if(!out_address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_address` was `NULL`");
	if(!size) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`size` was 0");
	if(!count) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`count` was 0");
	if(size == SIZE_MAX) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`size` was `SIZE_MAX`");
	if(count == SIZE_MAX) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`count` was `SIZE_MAX`");

	void* const allocated = GEN_MEMORY_CALLOC(count, size);
	if(!allocated) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "`" GEN_STRINGIFY(GEN_MEMORY_CALLOC) "` returned `NULL`: %t", gen_error_description_from_errno());

	*out_address = allocated;

	return NULL;
}

gen_error_t* gen_memory_allocate_zeroed_aligned(void* restrict* const restrict out_address, const size_t count, const size_t size, const size_t alignment) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_allocate_zeroed_aligned, GEN_FILE_NAME);
	if(error) return error;

	if(!out_address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_address` was `NULL`");
	if(!size) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`size` was 0");
	if(!count) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`count` was 0");
	if(size == SIZE_MAX) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`size` was `SIZE_MAX`");
	if(count == SIZE_MAX) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`count` was `SIZE_MAX`");
	if(!alignment) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`alignment` was 0");
	if(alignment & (alignment - 1)) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`alignment` was not a power of 2");

	// Size must be a multiple of alignment
	const size_t aligned_size = (((count * size) / alignment) + 1) * alignment;

	void* const allocated = GEN_MEMORY_ALIGNED_ALLOC(alignment, aligned_size);
	if(!allocated) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "`" GEN_STRINGIFY(GEN_MEMORY_ALIGNED_ALLOC) "` returned `NULL`: %t", gen_error_description_from_errno());

	*out_address = allocated;

	return NULL;
}

gen_error_t* gen_memory_reallocate_zeroed(void* restrict* const restrict address, const size_t old_count, const size_t count, const size_t size) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_reallocate_zeroed, GEN_FILE_NAME);
	if(error) return error;

	if(!address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`address` was `NULL`");
	if(!size) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`size` was 0");
	if(!count) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`count` was 0");
	if(size == SIZE_MAX) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`size` was `SIZE_MAX`");
	if(count == SIZE_MAX) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`count` was `SIZE_MAX`");

	void* allocated = GEN_MEMORY_REALLOC(*address, count * size);
	if(!allocated) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "`" GEN_STRINGIFY(GEN_MEMORY_REALLOC) "` returned `NULL`: %t", gen_error_description_from_errno());
	if(count > old_count) {
		error = gen_memory_set(allocated + (size * old_count), size * (count - old_count), 0);
		if(error) {
#ifdef __ANALYZER
			GEN_MEMORY_FREE(allocated);
#else
			gen_error_t* free_error = gen_memory_free(&allocated);
			if(free_error) return free_error;
#endif

			return error;
		}
	}

	*address = allocated;

	return NULL;
}

gen_error_t* gen_memory_free(void* restrict* const restrict address) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_free, GEN_FILE_NAME);
	if(error) return error;

	if(!address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`address` was `NULL`");

	GEN_MEMORY_FREE(*address);

	*address = NULL;

	return NULL;
}

gen_error_t* gen_memory_set(void* const restrict address, const size_t length, const unsigned char value) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_set, GEN_FILE_NAME);
	if(error) return error;

	if(!address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`address` was `NULL`");

	memset(address, value, length);

	return NULL;
}

gen_error_t* gen_memory_copy(void* const restrict to, const size_t to_size, const void* const restrict from, const size_t from_size, const size_t limit) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_copy, GEN_FILE_NAME);
	if(error) return error;

	if(!from) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`from` was `NULL`");
	if(!to) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`to` was `NULL`");
	if(limit > from_size) return gen_error_attach_backtrace(GEN_ERROR_OUT_OF_BOUNDS, GEN_LINE_NUMBER, "`limit` exceeded `from_size`");
	if(limit > to_size) return gen_error_attach_backtrace(GEN_ERROR_OUT_OF_BOUNDS, GEN_LINE_NUMBER, "`limit` exceeded `to_size`");

	memcpy(to, from, limit);

	return NULL;
}
