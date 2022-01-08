// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"

/**
 * This is to make the static analyzer happy
 */
#define GEN_INTERNAL_GEMORY_PARAM_CHECK \
	do { \
		GEN_INTERNAL_BASIC_PARAM_CHECK(out_address); \
		if(!size) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was 0"); \
		if(!count) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was 0"); \
		if(size == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was `SIZE_MAX`"); \
		if(count == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was `SIZE_MAX`"); \
	} while(0)

gen_error_t gzalloc(void* restrict* const restrict out_address, const size_t size, const size_t count) {
	GEN_FRAME_BEGIN(gzalloc);

	GEN_INTERNAL_GEMORY_PARAM_CHECK;

#if GEN_USE_MIMALLOC == ENABLED
	void* const allocated = mi_calloc(count, size);
#else
	void* const allocated = calloc(count, size);
#endif
	if(!allocated) GEN_ERROR_OUT_ERRNO(mi_calloc, errno);

	*out_address = allocated;

	errno = 0; // mimalloc does weirdness with NUMA

	GEN_ALL_OK;
}

gen_error_t gzalloc_aligned(void* restrict* const restrict out_address, const size_t size, const size_t count, const size_t align) {
	GEN_FRAME_BEGIN(gzalloc);

	GEN_INTERNAL_GEMORY_PARAM_CHECK;

	if(!align) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`align` was 0");
	if(align & (align - 1)) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`align` was not a power of 2");

#if GEN_USE_MIMALLOC == ENABLED
	void* const allocated = mi_calloc_aligned(count, size, align);
#else
	void* const allocated = aligned_alloc(align, count * size);
#endif
	if(!allocated) GEN_ERROR_OUT_ERRNO(mi_calloc, errno);

	*out_address = allocated;

	errno = 0; // mimalloc does weirdness with NUMA

	GEN_ALL_OK;
}

gen_error_t grealloc(void* restrict* const restrict out_address, const size_t size, const size_t count) {
	GEN_FRAME_BEGIN(grealloc);

	GEN_INTERNAL_GEMORY_PARAM_CHECK;

#if GEN_USE_MIMALLOC == ENABLED
	void* const allocated = mi_recalloc(*out_address, count, size);
#else
	void* const allocated = realloc(*out_address, count * size);
#endif
	if(!allocated) GEN_ERROR_OUT_ERRNO(mi_recalloc, errno);

	*out_address = allocated;

	errno = 0; // mimalloc does weirdness with NUMA

	GEN_ALL_OK;
}

gen_error_t gfree(void* const restrict address) {
	GEN_FRAME_BEGIN(gfree);

	GEN_INTERNAL_BASIC_PARAM_CHECK(address);

#if GEN_USE_MIMALLOC == ENABLED
	mi_free(address);
#else
	free(address);
#endif

	errno = 0; // mimalloc does weirdness with NUMA

	GEN_ALL_OK;
}
