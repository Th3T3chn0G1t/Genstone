#include "include/gendbg.h"
#include "include/gendiag.h"
#include "include/generrors.h"
#include "include/gentooling.h"
#include "include/genutil.h"

GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wreserved-identifier"
extern int _getenv_s_chk(unsigned long *restrict len, char *restrict dest, unsigned long dmax, const char *restrict name, const unsigned long destbos);
extern int _snprintf_s_chk(char *restrict dest, unsigned long dmax, const unsigned long destbos, const char *restrict fmt, ...) __attribute_format__(printf, 4, 5);
GEN_DIAG_IGNORE_ALL
#include <inttypes.h>
#include <locale.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
extern size_t strerrorlen_s(int errnum);
extern int _strerror_s_chk(char *dest, unsigned long dmax, int errnum, const unsigned long destbos);
#define strerror_s(dest, dmax, errnum) _strerror_s_chk(dest, dmax, errnum, __builtin_object_size(dest, 1))
extern int _strcpy_s_chk(char *restrict dest, rsize_t dmax, const char *restrict src, const size_t destbos);
#define strcpy_s(dest, dmax, src) _strcpy_s_chk(dest, dmax, src, __builtin_object_size(dest, 1))
extern int fprintf_s(FILE *restrict stream, const char *restrict fmt, ...) __attribute_format__(printf, 2, 3);
GEN_DIAG_REGION_END

#include "include/glog.h"

__attribute__((constructor)) static void gen_internal_initialize_locales(void) {
	size_t locale_len = 0;

	_getenv_s_chk(&locale_len, NULL, 0, "LANG", __builtin_object_size(NULL, 1));
	if(!locale_len) {
		_getenv_s_chk(&locale_len, NULL, 0, "LC_ALL", __builtin_object_size(NULL, 1));
		if(!locale_len) {
			setlocale(LC_ALL, "C");
			return;
		}
		char locale[locale_len];
		_getenv_s_chk(&locale_len, locale, locale_len, "LC_ALL", __builtin_object_size(&locale, 1));
		setlocale(LC_ALL, locale);
	}
	char locale[locale_len];
	_getenv_s_chk(&locale_len, locale, locale_len, "LANG", __builtin_object_size(&locale, 1));
	setlocale(LC_ALL, locale);

	tzset();
}

#define GEN_INTERNAL_CURRENCY_FORMAT "%s%" PRIu64 "%s%02" PRIu64

extern gen_error_t gen_locale_convert_currency(char *const restrict out_string, size_t *const restrict out_size, const uint64_t units, const uint64_t cents);
gen_error_t gen_locale_convert_currency(char *const restrict out_string, size_t *const restrict out_size, const uint64_t units, const uint64_t cents) {
	const struct lconv *const restrict locale_info = localeconv();

	const int size_r = _snprintf_s_chk(NULL, 0, __builtin_object_size(NULL, 1), GEN_INTERNAL_CURRENCY_FORMAT, locale_info->currency_symbol, units, locale_info->decimal_point, cents);
	GEN_ERROR_OUT_IF_ERRNO(snprintf_s, errno);
	const size_t size = (size_t) size_r + 1;
	if(out_size) *out_size = size;
	if(out_string) {
		_snprintf_s_chk(out_string, size, __builtin_object_size(out_string, 1), GEN_INTERNAL_CURRENCY_FORMAT, locale_info->currency_symbol, units, locale_info->decimal_point, cents);
		GEN_ERROR_OUT_IF_ERRNO(snprintf_s, errno);
	}

	GEN_ALL_OK;
}

// const static uint64_t month_lens = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// gen_error_t gen_locale_convert_date_and_time(char* const out_string, size_t* const restrict out_size, const uint64_t year, const gen_month_t month, const uint8_t day, const uint8_t hours, const uint8_t minutes, const uint8_t seconds) {
// 	if(seconds > 59) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`seconds` exceeded 59");
// 	if(minutes > 59) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`minutes` exceeded 59");
// 	if(hours > 23) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`hours` exceeded 23");

// 	if(day > month_lens[month]) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`day` exceeded the number of days in `month`");

// 	// year month day hours minutes seconds
// 	const struct tm time = {
// 		.tm_sec = seconds,
// 		.tm_min = minutes,
// 		.tm_hour = hours,
// 		.tm_mday = day,
// 		.tm_mon = month,
// 		.tm_year = year - 1900
// 	};

// 	if(out_string) {
// 		*out_size = strnlen_s()
// 	}
// 	if(out_string) {
// 		asctime_s(out_string);
// 		GEN_ERROR_OUT_IF_ERRNO(asctime_s, errno);
// 	}

// 	GEN_ALL_OK;
// }
