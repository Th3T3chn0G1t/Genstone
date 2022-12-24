// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"

#include "include/genmemory.h"

#include <genmemory_be.h>
#include <genbackends.h>

gen_error_t* gen_memory_allocate_zeroed(void* restrict* const restrict out_address, const gen_size_t count, const gen_size_t size) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_allocate_zeroed, GEN_FILE_NAME);
	if(error) return error;

	if(!out_address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_address` was `GEN_NULL`");
	if(!size) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`size` was 0");
	if(!count) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`count` was 0");
	if(size == GEN_SIZE_MAX) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`size` was `GEN_SIZE_MAX`");
	if(count == GEN_SIZE_MAX) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`count` was `GEN_SIZE_MAX`");

    error = GEN_BACKENDS_CALL(memory_allocate_zeroed)(out_address, count, size);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_memory_allocate_zeroed_aligned(void* restrict* const restrict out_address, const gen_size_t count, const gen_size_t size, const gen_size_t alignment) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_allocate_zeroed_aligned, GEN_FILE_NAME);
	if(error) return error;

	if(!out_address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_address` was `GEN_NULL`");
	if(!size) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`size` was 0");
	if(!count) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`count` was 0");
	if(size == GEN_SIZE_MAX) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`size` was `GEN_SIZE_MAX`");
	if(count == GEN_SIZE_MAX) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`count` was `GEN_SIZE_MAX`");
	if(!alignment) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`alignment` was 0");
	if(alignment & (alignment - 1)) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`alignment` was not a power of 2");

	// Size must be a multiple of alignment
	const gen_size_t aligned_size = ((size / alignment) + 1) * alignment;

    error = GEN_BACKENDS_CALL(memory_allocate_zeroed_aligned)(out_address, count, aligned_size, alignment);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_memory_reallocate_zeroed(void* restrict* const restrict address, const gen_size_t old_count, const gen_size_t count, const gen_size_t size) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_reallocate_zeroed, GEN_FILE_NAME);
	if(error) return error;

	if(!address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`address` was `GEN_NULL`");
	if(!size) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`size` was 0");
	if(!count) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`count` was 0");
	if(size == GEN_SIZE_MAX) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`size` was `GEN_SIZE_MAX`");
	if(count == GEN_SIZE_MAX) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`count` was `GEN_SIZE_MAX`");

    error = GEN_BACKENDS_CALL(memory_reallocate_zeroed)(address, old_count, count, size);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_memory_free(void* restrict* const restrict address) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_free, GEN_FILE_NAME);
	if(error) return error;

	if(!address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`address` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(memory_free)(address);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_memory_set(void* const restrict address, const gen_size_t bounds, const gen_size_t count, const unsigned char value) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_set, GEN_FILE_NAME);
	if(error) return error;

	if(!address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`address` was `GEN_NULL`");
    if(count > bounds) return gen_error_attach_backtrace(GEN_ERROR_OUT_OF_BOUNDS, GEN_LINE_NUMBER, "`count` > `bounds`");

    error = GEN_BACKENDS_CALL(memory_set)(address, bounds, count, value);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_memory_copy(void* const restrict to, const gen_size_t to_bounds, const void* const restrict from, const gen_size_t from_bounds, const gen_size_t count) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_copy, GEN_FILE_NAME);
	if(error) return error;

	if(!to) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`to` was `GEN_NULL`");
	if(!from) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`from` was `GEN_NULL`");
	if(count > to_bounds) return gen_error_attach_backtrace(GEN_ERROR_OUT_OF_BOUNDS, GEN_LINE_NUMBER, "`count` > `to_bounds`");
	if(count > from_bounds) return gen_error_attach_backtrace(GEN_ERROR_OUT_OF_BOUNDS, GEN_LINE_NUMBER, "`count` > `from_bounds`");

    error = GEN_BACKENDS_CALL(memory_copy)(to, to_bounds, from, from_bounds, count);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_memory_compare(const void* const restrict a, const gen_size_t a_bounds, const void* const restrict b, const gen_size_t b_bounds, const gen_size_t count, gen_bool_t* const restrict out_equal) {
 	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_memory_compare, GEN_FILE_NAME);
	if(error) return error;

	if(!a) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`a` was `GEN_NULL`");
	if(!b) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`b` was `GEN_NULL`");
	if(!out_equal) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_equal` was `GEN_NULL`");
	if(count > a_bounds) return gen_error_attach_backtrace(GEN_ERROR_OUT_OF_BOUNDS, GEN_LINE_NUMBER, "`count` > `a_bounds`");
	if(count > b_bounds) return gen_error_attach_backtrace(GEN_ERROR_OUT_OF_BOUNDS, GEN_LINE_NUMBER, "`count` > `b_bounds`");

    error = GEN_BACKENDS_CALL(memory_compare)(a, a_bounds, b, b_bounds, count, out_equal);
	if(error) return error;

	return GEN_NULL;
}
