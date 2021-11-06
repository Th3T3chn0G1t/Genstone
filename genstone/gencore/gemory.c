#include "include/gencommon.h"

gen_error_t galloc(void** const restrict out_address, const size_t size, const size_t count) {
	GEN_FRAME_BEGIN(galloc);

	if(!out_address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`out_address` was NULL");
	if(!size) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was 0");
	if(!count) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was 0");
	if(size == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was `SIZE_MAX`");
	if(count == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was `SIZE_MAX`");

	void* const allocated = mi_malloc(size * count);
	GEN_ERROR_OUT_IF_ERRNO(mi_malloc, errno);

	*out_address = allocated;

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gzalloc(void** const restrict out_address, const size_t size, const size_t count) {
	GEN_FRAME_BEGIN(gzalloc);

	if(!out_address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`out_address` was NULL");
	if(!size) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was 0");
	if(!count) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was 0");
	if(size == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was `SIZE_MAX`");
	if(count == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was `SIZE_MAX`");

	void* const allocated = mi_calloc(count, size);
	GEN_ERROR_OUT_IF_ERRNO(mi_calloc, errno);

	*out_address = allocated;

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t grealloc(void** const restrict address, const size_t size, const size_t count) {
	GEN_FRAME_BEGIN(grealloc);

	if(!address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`address` was NULL");
	if(!size) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was 0");
	if(!count) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was 0");
	if(size == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`size` was `SIZE_MAX`");
	if(count == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`count` was `SIZE_MAX`");

	void* const allocated = mi_realloc(*address, count * size);
	GEN_ERROR_OUT_IF_ERRNO(mi_realloc, errno);

	*address = allocated;

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gstrndup(char** const restrict out_address, const char* const restrict str, const size_t max) {
	GEN_FRAME_BEGIN(gstrndup);

	if(!out_address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`address` was NULL");
	if(!str) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`str` was NULL");
	if(!max) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`max` was 0");
	if(max == SIZE_MAX) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`max` was SIZE_MAX");

	char* const allocated = mi_strndup(str, max);
	GEN_ERROR_OUT_IF_ERRNO(mi_strndup, errno);

	*out_address = allocated;

	GEN_ERROR_OUT(GEN_OK, "");
}

gen_error_t gfree(void* const restrict address) {
	GEN_FRAME_BEGIN(gfree);

	if(!address) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`address` was NULL");

	mi_free(address);

	GEN_ERROR_OUT(GEN_OK, "");
}
