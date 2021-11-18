#include "include/gencommon.h"

gen_error_t gzalloc(void* restrict* const restrict out_address, const size_t size, const size_t count) {
	GEN_FRAME_BEGIN(gzalloc);

	if(!out_address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`address` was NULL");
	if(!size) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was 0");
	if(!count) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was 0");
	if(size == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was `SIZE_MAX`");
	if(count == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was `SIZE_MAX`");

#if GEN_USE_MIMALLOC == ENABLED
	void* const allocated = mi_calloc(count, size);
#else
	void* const allocated = calloc(count, size);
#endif
	if(!allocated) GEN_ERROR_OUT_ERRNO(mi_calloc, errno);

	*out_address = allocated;

	errno = 0; // mimalloc does weirdness with NUMA

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gzalloc_aligned(void* restrict* const restrict out_address, const size_t size, const size_t count, const size_t align) {
	GEN_FRAME_BEGIN(gzalloc);

	if(!out_address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`address` was NULL");
	if(!size) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was 0");
	if(!count) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was 0");
  	if(!align) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`align` was 0");
	if(align & (align - 1)) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`align` was not a power of 2");
	if(size == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was `SIZE_MAX`");
	if(count == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was `SIZE_MAX`");

#if GEN_USE_MIMALLOC == ENABLED
	void* const allocated = mi_calloc_aligned(count, size, align);
#else
	void* const allocated = aligned_alloc(align, count * size);
#endif
	if(!allocated) GEN_ERROR_OUT_ERRNO(mi_calloc, errno);

	*out_address = allocated;

	errno = 0; // mimalloc does weirdness with NUMA

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t grealloc(void* restrict* const restrict address, const size_t size, const size_t count) {
	GEN_FRAME_BEGIN(grealloc);

	if(!address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`address` was NULL");
	if(!size) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was 0");
	if(!count) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was 0");
	if(size == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was `SIZE_MAX`");
	if(count == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was `SIZE_MAX`");

#if GEN_USE_MIMALLOC == ENABLED
	void* const allocated = mi_recalloc(*address, count, size);
#else
	void* const allocated = realloc(*address, count * size);
#endif
	if(!allocated) GEN_ERROR_OUT_ERRNO(mi_recalloc, errno);

	*address = allocated;

	errno = 0; // mimalloc does weirdness with NUMA

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gstrndup(char* restrict* const restrict out_address, const char* const restrict str, const size_t max) {
	GEN_FRAME_BEGIN(gstrndup);

	if(!out_address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`address` was NULL");
	if(!str) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`str` was NULL");
	if(!max) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`max` was 0");
	if(max == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`max` was SIZE_MAX");

#if GEN_USE_MIMALLOC == ENABLED
	char* const allocated = mi_strndup(str, max);
#else
	char* const allocated = strndup(str, max);
#endif
	if(!allocated) GEN_ERROR_OUT_ERRNO(mi_strndup, errno);

	*out_address = allocated;

	errno = 0; // mimalloc does weirdness with NUMA

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gfree(void* const restrict address) {
	GEN_FRAME_BEGIN(gfree);

	if(!address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`address` was NULL");

#if GEN_USE_MIMALLOC == ENABLED
	mi_free(address);
#else
	free(address);
#endif

	errno = 0; // mimalloc does weirdness with NUMA

	GEN_ERROR_OUT(GEN_OK, "");
}
