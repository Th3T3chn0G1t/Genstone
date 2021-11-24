#include "include/genlocale.h"

#define GEN_INTERNAL_CURRENCY_FORMAT "%s%" PRIu64 "%s%02" PRIu64

gen_error_t gen_locale_convert_currency(char* const restrict out_string, size_t* const restrict out_size, const uint64_t units, const uint64_t cents) {
	const struct lconv* const restrict locale_info = localeconv();

	const int size_r = snprintf(NULL, 0, GEN_INTERNAL_CURRENCY_FORMAT, locale_info->currency_symbol, units, locale_info->decimal_point, cents);
	GEN_ERROR_OUT_IF_ERRNO(snprintf_s, errno);
	const size_t size = (size_t) size_r + 1;
	if(out_size) *out_size = size;
	if(out_string) {
		snprintf_s(out_string, size, GEN_INTERNAL_CURRENCY_FORMAT, locale_info->currency_symbol, units, locale_info->decimal_point, cents);
		GEN_ERROR_OUT_IF_ERRNO(snprintf_s, errno);
	}

	GEN_ERROR_OUT(GEN_OK, "");
}
