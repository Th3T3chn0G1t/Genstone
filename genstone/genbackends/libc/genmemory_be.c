// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genmemory.h>

#include <genbackendslibcerror.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <stdlib.h>
#include <string.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

extern gen_error_t* gen_backends_libc_memory_allocate_zeroed(void* restrict* const restrict out_address, const gen_size_t count, const gen_size_t size);
gen_error_t* gen_backends_libc_memory_allocate_zeroed(void* restrict* const restrict out_address, const gen_size_t count, const gen_size_t size) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_libc_memory_allocate_zeroed, GEN_FILE_NAME);
    if(error) return error;

	void* const allocated = calloc(count, size);
	if(!allocated) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "`calloc` returned `GEN_NULL`: %t", gen_error_description_from_errno());

	*out_address = allocated;

    return GEN_NULL;
}

extern gen_error_t* gen_backends_libc_memory_allocate_zeroed_aligned(void* restrict* const restrict out_address, const gen_size_t count, const gen_size_t size, const gen_size_t alignment);
gen_error_t* gen_backends_libc_memory_allocate_zeroed_aligned(void* restrict* const restrict out_address, const gen_size_t count, const gen_size_t size, const gen_size_t alignment) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_libc_memory_allocate_zeroed_aligned, GEN_FILE_NAME);
    if(error) return error;

    

 	void* const allocated = aligned_alloc(alignment, count * size);
	if(!allocated) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "`aligned_alloc` returned `GEN_NULL`: %t", gen_error_description_from_errno());

    error = gen_memory_set(allocated, count * size, count * size, 0);
    if(error) return error;

	*out_address = allocated;

    return GEN_NULL;
}

static void gen_backends_memory_internal_reallocate_zeroed_cleanup_allocated(void** allocated) {
    if(!*allocated) return;

    gen_error_t* error = gen_memory_free(allocated);
    if(error) gen_error_abort_with_error(error, "genmemory");
}

extern gen_error_t* gen_backends_libc_memory_reallocate_zeroed(void* restrict* const restrict address, const gen_size_t old_count, const gen_size_t count, const gen_size_t size);
gen_error_t* gen_backends_libc_memory_reallocate_zeroed(void* restrict* const restrict address, const gen_size_t old_count, const gen_size_t count, const gen_size_t size) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_libc_memory_reallocate_zeroed, GEN_FILE_NAME);
    if(error) return error;

	void* allocated = realloc(*address, count * size);
	if(!allocated) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "`realloc` returned `GEN_NULL`: %t", gen_error_description_from_errno());

    GEN_CLEANUP_FUNCTION(gen_backends_memory_internal_reallocate_zeroed_cleanup_allocated) void* allocated_scope_variable = allocated;

	if(count > old_count) {
		error = gen_memory_set(allocated + (size * old_count), (size * count) - (size * old_count), size * (count - old_count), 0);
		if(error) return error;
	}

	*address = allocated;

    allocated_scope_variable = GEN_NULL;

    return GEN_NULL;
}

extern gen_error_t* gen_backends_libc_memory_free(void* restrict* const restrict address);
gen_error_t* gen_backends_libc_memory_free(void* restrict* const restrict address) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_libc_memory_free, GEN_FILE_NAME);
    if(error) return error;

	free(*address);

	*address = GEN_NULL;

    return GEN_NULL;
}

extern gen_error_t* gen_backends_libc_memory_set(void* const restrict address, GEN_UNUSED const gen_size_t bounds, const gen_size_t count, const unsigned char value);
gen_error_t* gen_backends_libc_memory_set(void* const restrict address, GEN_UNUSED const gen_size_t bounds, const gen_size_t count, const unsigned char value) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_libc_memory_set, GEN_FILE_NAME);
    if(error) return error;

	memset(address, value, count);

    return GEN_NULL;
}

extern gen_error_t* gen_backends_libc_memory_copy(void* const restrict to, GEN_UNUSED const gen_size_t to_bounds, const void* const restrict from, GEN_UNUSED const gen_size_t from_bounds, const gen_size_t count);
gen_error_t* gen_backends_libc_memory_copy(void* const restrict to, GEN_UNUSED const gen_size_t to_bounds, const void* const restrict from, GEN_UNUSED const gen_size_t from_bounds, const gen_size_t count) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_libc_memory_copy, GEN_FILE_NAME);
    if(error) return error;

	memcpy(to, from, count);

    return GEN_NULL;
}


extern gen_error_t* gen_backends_libc_memory_compare(const void* const restrict a, GEN_UNUSED const gen_size_t a_bounds, const void* const restrict b, GEN_UNUSED const gen_size_t b_bounds, const gen_size_t count, gen_bool_t* const restrict out_equal);
gen_error_t* gen_backends_libc_memory_compare(const void* const restrict a, GEN_UNUSED const gen_size_t a_bounds, const void* const restrict b, GEN_UNUSED const gen_size_t b_bounds, const gen_size_t count, gen_bool_t* const restrict out_equal) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_libc_memory_compare, GEN_FILE_NAME);
    if(error) return error;

    *out_equal = !memcmp(a, b, count);

    return GEN_NULL;
}
