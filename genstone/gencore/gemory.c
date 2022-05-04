// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"

/**
 * This is to make the static analyzer happy
 */
#define GEN_INTERNAL_GEMORY_PARAM_CHECK \
	do { \
		GEN_NULL_CHECK(out_address); \
		if(!size) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was 0"); \
		if(!count) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was 0"); \
		if(size == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was `SIZE_MAX`"); \
		if(count == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was `SIZE_MAX`"); \
	} while(0)

gen_error_t gzalloc(void* restrict* const restrict out_address, const size_t count, const size_t size) {
	GEN_FRAME_BEGIN(gzalloc);

	GEN_INTERNAL_GEMORY_PARAM_CHECK;

	void* const allocated = calloc(count, size);
	if(!allocated) GEN_ERROR_OUT_ERRNO(mi_calloc, errno);

	*out_address = allocated;

	GEN_ALL_OK;
}

gen_error_t gzalloc_aligned(void* restrict* const restrict out_address, const size_t count, const size_t size, const size_t align) {
	GEN_FRAME_BEGIN(gzalloc_aligned);

	GEN_INTERNAL_GEMORY_PARAM_CHECK;

	if(!align) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`align` was 0");
	if(align & (align - 1)) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`align` was not a power of 2");

	void* const allocated = aligned_alloc(align, count * size);
	if(!allocated) GEN_ERROR_OUT_ERRNO(mi_calloc, errno);

	*out_address = allocated;

	GEN_ALL_OK;
}

gen_error_t grealloc(void* restrict* const restrict out_address, const size_t old_count, const size_t count, const size_t size) {
	GEN_FRAME_BEGIN(grealloc);

	GEN_INTERNAL_GEMORY_PARAM_CHECK;

	void* const allocated = realloc(*out_address, count * size);
	GEN_ERROR_OUT_IF_ERRNO(realloc, errno);
	if(count > old_count) memset((unsigned char*) allocated + (size * old_count), 0, size * (count - old_count));

	*out_address = allocated;

	GEN_ALL_OK;
}

gen_error_t gfree(void* const restrict address) {
	GEN_FRAME_BEGIN(gfree);

	GEN_NULL_CHECK(address);

	free(address);

	GEN_ALL_OK;
}

gen_error_t gen_memory_set(void* const restrict address, const size_t length, const unsigned char value) {
	GEN_FRAME_BEGIN(gen_memory_set);

	GEN_NULL_CHECK(address);

	for(unsigned char* d = address; d < (unsigned char*) address + length; ++d) {
		*d = value;
	}

	GEN_ALL_OK;
}
