#include "include/genlocale.h"

#define GEN_INTERNAL_CURRENCY_FORMAT "%s%" PRIu64 "%s%02" PRIu64

__attribute__((constructor)) static void gen_internal_initialize_locales(void) {
	const char* locale = getenv("LANG");
	if(locale)
		setlocale(LC_ALL, locale);
	else {
		locale = getenv("LC_ALL");
		if(locale) setlocale(LC_ALL, locale);
	}

	tzset();
}

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

gen_error_t gen_locale_convert_date_and_time(__unused char* const out_string, __unused size_t* const restrict out_size, __unused const uint64_t year, __unused const gen_month_t month, __unused const uint8_t day, __unused const uint8_t hours, __unused const uint8_t minutes, __unused const uint8_t seconds) {
	// year month day hours minutes seconds
	// struct tm time = {
	// 	.tm_sec = seconds,
	// 	.tm_min = minutes,
	// 	.tm_hour = hours,
	// 	.tm_mday = day,
	// 	.tm_mon =
	// 	.tm_year =
	// 	.tm_wday =
	// 	.tm_yday =
	// 	.tm_isdst =
	// 	.tm_gmtoff =
	// 	.tm_zone =
	// 	.tm_zone =
	// }

	// printf("UTC:       %s", asctime(gmtime(&t)));
	// printf("local:     %s", asctime(localtime(&t)));
	// // POSIX-specific
	// putenv("TZ=Asia/Singapore");
	// printf("Singapore: %s", asctime(localtime(&t)));

	GEN_ERROR_OUT(GEN_OK, "");
}
