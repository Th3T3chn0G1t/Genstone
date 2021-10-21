#include "include/gencommon.h"

gen_error_t galloc(void** const restrict out_address, const size_t size, const size_t count) {
	GEN_FRAME_BEGIN(galloc);

	if(!out_address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`out_address` was NULL");
	if(!size) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was 0");
	if(!count) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was 0");

	void* const allocated = mi_malloc(size * count);

	if(!allocated) GEN_ERROR_OUT_ERRNO(mi_malloc, errno);

	*out_address = allocated;

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gzalloc(void** const restrict out_address, const size_t size, const size_t count) {
	GEN_FRAME_BEGIN(gzalloc);

	if(!out_address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`out_address` was NULL");
	if(!size) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was 0");
	if(!count) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was 0");

	void* const allocated = mi_calloc(count, size);

	if(!allocated) GEN_ERROR_OUT_ERRNO(mi_calloc, errno);

	*out_address = allocated;

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t grealloc(void** const restrict address, const size_t size, const size_t count) {
	GEN_FRAME_BEGIN(grealloc);

	if(!address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`address` was NULL");
	if(!size) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was 0");
	if(!count) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was 0");

	void* const allocated = mi_realloc(*address, count * size);

	if(!allocated) GEN_ERROR_OUT_ERRNO(mi_realloc, errno);

	*address = allocated;

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gfree(void* const restrict address) {
	GEN_FRAME_BEGIN(gfree);

	if(!address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`address` was NULL");

	mi_free(address);

	GEN_ERROR_OUT(GEN_OK, "");
}
