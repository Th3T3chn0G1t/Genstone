#include "include/genlocale.h"

__attribute__((constructor)) static void gen_internal_initialize_locales(void) {
	const char *locale = getenv("LANG");
	if(!locale) {
		locale = getenv("LC_ALL");
		if(!locale) {
			setlocale(LC_ALL, "C");
			tzset();
			return;
		}
		setlocale(LC_ALL, locale);
		tzset();
		return;
	}
	setlocale(LC_ALL, locale);
	tzset();
}

#define GEN_INTERNAL_CURRENCY_FORMAT "%s%" PRIu64 "%s%02" PRIu64

gen_error_t gen_locale_convert_currency(char *const restrict out_string, size_t *const restrict out_size, const uint64_t units, const uint64_t cents) {
	const struct lconv *const restrict locale_info = localeconv();

	const int size_r = snprintf(NULL, 0, GEN_INTERNAL_CURRENCY_FORMAT, locale_info->currency_symbol, units, locale_info->decimal_point, cents);
	GEN_ERROR_OUT_IF_ERRNO(snprintf_s, errno);
	const size_t size = (size_t) size_r + 1;
	if(out_size) *out_size = size;
	if(out_string) {
		snprintf_s(out_string, size, GEN_INTERNAL_CURRENCY_FORMAT, locale_info->currency_symbol, units, locale_info->decimal_point, cents);
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
