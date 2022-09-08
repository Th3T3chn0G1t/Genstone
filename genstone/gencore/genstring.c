// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genstring.h"

#include "include/genmemory.h"

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_PLATFORM == GEN_WINDOWS
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <string.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)
#endif

#ifndef GEN_STRING_STRNLEN
/**
 * The function to use for measuring string length.
 * Must match the signature of `strnlen` and follow equivalent semantics.
 */
#define GEN_STRING_STRNLEN strnlen
#endif

#ifndef GEN_STRING_STRNCMP
/**
 * The function to use for comparing strings.
 * Must match the signature of `strncmp` and follow equivalent semantics.
 */
#define GEN_STRING_STRNCMP strncmp
#endif

#ifndef GEN_STRING_STRNCPY
/**
 * The function to use for copying strings.
 * Must match the signature of `strncpy` and follow equivalent semantics.
 */
#define GEN_STRING_STRNCPY strncpy
#endif

#ifndef GEN_STRING_STRNCAT
/**
 * The function to use for concatonating strings.
 * Must match the signature of `strncat` and follow equivalent semantics.
 */
#define GEN_STRING_STRNCAT strncat
#endif

// TODO: We do not error out with bad bounding and instead just
//       Silently use the minimum safe operation.
//       We really should be reporting bounding issues to
//       The user.

gen_error_t* gen_string_compare(const char* const restrict a, const size_t a_bounds, const char* const restrict b, const size_t b_bounds, const size_t limit, bool* const restrict out_equal) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_compare, GEN_FILE_NAME);
	if(error) return error;

	if(!a) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`a` was `NULL`");
	if(!b) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`b` was `NULL`");
	if(!out_equal) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_equal` was `NULL`");

	if(!limit) {
		*out_equal = true;
		return NULL;
	}

	size_t a_length = 0;
	error = gen_string_length(a, a_bounds, limit, &a_length);
	if(error) return error;

	size_t b_length = 0;
	error = gen_string_length(b, b_bounds, limit, &b_length);
	if(error) return error;

	if(limit == GEN_STRING_NO_BOUNDS && a_length != b_length) {
		*out_equal = false;
		return NULL;
	}

	*out_equal = !GEN_STRING_STRNCMP(a, b, GEN_MAXIMUM(GEN_MINIMUM(a_length, b_bounds), GEN_MINIMUM(b_length, a_bounds)));

	return NULL;
}

gen_error_t* gen_string_copy(char* const restrict destination, const size_t destination_bounds, const char* const restrict source, const size_t source_bounds, const size_t limit) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_copy, GEN_FILE_NAME);
	if(error) return error;

	if(!destination) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`destination` was `NULL`");
	if(!source) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`source` was `NULL`");

	if(!limit) return NULL;

	error = gen_memory_set(destination, GEN_MINIMUM(limit, destination_bounds), '\0');
	if(error) return error;

	size_t source_length = 0;
	error = gen_string_length(source, source_bounds, limit, &source_length);
	if(error) return error;

	GEN_STRING_STRNCPY(destination, source, GEN_MINIMUM(source_length, destination_bounds));

	return NULL;
}

gen_error_t* gen_string_append(char* const restrict destination, const size_t destination_bounds, const char* const restrict source, const size_t source_bounds, const size_t limit) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_append, GEN_FILE_NAME);
	if(error) return error;

	if(!destination) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`destination` was `NULL`");
	if(!source) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`source` was `NULL`");

	if(!limit) return NULL;

	size_t destination_length = 0;
	error = gen_string_length(destination, destination_bounds, limit, &destination_length);
	if(error) return error;

	size_t source_length = 0;
	error = gen_string_length(source, source_bounds, limit, &source_length);
	if(error) return error;

	GEN_STRING_STRNCAT(destination, source, GEN_MINIMUM(source_length, destination_bounds - destination_length));

	return NULL;
}

gen_error_t* gen_string_length(const char* const restrict string, const size_t string_bounds, const size_t limit, size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_length, GEN_FILE_NAME);
	if(error) return error;

	if(!string) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `NULL`");
	if(!out_length) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_length` was `NULL`");

	*out_length = GEN_STRING_STRNLEN(string, GEN_MINIMUM(limit, string_bounds));

	return NULL;
}

static void gen_string_duplicate_duplicated_cleanup(char** duplicated) {
	if(*duplicated) return;

	gen_error_t* error = gen_memory_free((void**) duplicated);
	if(error) {
		gen_error_print("genstring", error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
}

gen_error_t* gen_string_duplicate(const char* const restrict string, const size_t string_bounds, const size_t limit, char* restrict* const restrict out_duplicated, size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_duplicate, GEN_FILE_NAME);
	if(error) return error;

	if(!string) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `NULL`");
	if(!limit) return gen_error_attach_backtrace(GEN_ERROR_TOO_SHORT, GEN_LINE_NUMBER, "`limit` was 0");
	if(!out_duplicated) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_duplicated` was `NULL`");

	size_t string_length = 0;
	error = gen_string_length(string, string_bounds, limit, &string_length);
	if(error) return error;

	char* duplicated = NULL;
	GEN_CLEANUP_FUNCTION(gen_string_duplicate_duplicated_cleanup)
	char* duplicated_scope_variable = duplicated;
	error = gen_memory_allocate_zeroed((void**) &duplicated, string_length + 1, sizeof(char));
	if(error) return error;

	error = gen_string_copy(duplicated, string_length + 1, string, string_bounds, string_length);
	if(error) return error;

	duplicated_scope_variable = NULL;

	*out_duplicated = duplicated;
	if(out_length) *out_length = string_length;

	return NULL;
}

gen_error_t* gen_string_character_first(const char* const restrict string, const size_t string_bounds, const char character, const size_t limit, size_t* const restrict out_found) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_character_first, GEN_FILE_NAME);
	if(error) return error;

	if(!string) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `NULL`");
	if(!out_found) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_found` was `NULL`");

	if(!limit) {
		*out_found = SIZE_MAX;
		return NULL;
	}

	size_t string_length = 0;
	error = gen_string_length(string, string_bounds, limit, &string_length);
	if(error) return error;

#ifdef __ANALYZER
#else
	for(size_t i = 0; i < string_length; ++i) {
		if(string[i] == character) {
			*out_found = i;
			return NULL;
		}
	}
#endif

	*out_found = SIZE_MAX;
	return NULL;
}

gen_error_t* gen_string_character_last(const char* const restrict string, const size_t string_bounds, const char character, const size_t limit, size_t* const restrict out_found) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_character_last, GEN_FILE_NAME);
	if(error) return error;

	if(!string) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `NULL`");
	if(!out_found) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_found` was `NULL`");

	if(!limit) {
		*out_found = SIZE_MAX;
		return NULL;
	}

	size_t string_length = 0;
	error = gen_string_length(string, string_bounds, limit, &string_length);
	if(error) return error;

	for(size_t i = string_length - 1; i != SIZE_MAX; --i) {
		if(string[i] == character) {
			*out_found = i;
			return NULL;
		}
	}

	*out_found = SIZE_MAX;
	return NULL;
}

gen_error_t* gen_string_number(const char* const restrict string, const size_t string_bounds, const size_t limit, size_t* const restrict out_number) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_number, GEN_FILE_NAME);
	if(error) return error;

	if(!out_number) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_number` was `NULL`");
	if(!string_bounds) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string_bounds` was 0");

	size_t string_length = 0;
	error = gen_string_length(string, string_bounds, limit, &string_length);
	if(error) return error;

	size_t accumulate = 0;
#ifdef __ANALYZER
#else
    size_t pow = 1;
	for(size_t i = string_length - 1; i != SIZE_MAX; --i) {
		if(string[i] < '0' || string[i] > '9') return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Encountered non-numeric character `%c`", string[i]);
		accumulate += (size_t) (string[i] - '0') * pow;
        pow *= 10;
	}
#endif

	*out_number = accumulate;

	return NULL;
}

gen_error_t* gen_string_format(const size_t limit, char* const restrict out_buffer, size_t* out_length, const char* const restrict format, const size_t format_length, ...) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_format, GEN_FILE_NAME);
	if(error) return error;

	va_list list;
	va_start(list, format_length);

	return gen_string_formatv(limit, out_buffer, out_length, format, format_length, list);
}

static void gen_string_internal_format_number_base10_unsigned(const size_t len, char* const restrict out_buffer, size_t* const restrict offset, const size_t value) {
	size_t accumulate = value;
	char out[20] = {0}; // "18446744073709551615" (i.e. `SIZE_MAX`) is 20 characters
	size_t idx = 0;
	do {
		out[idx++] = '0' + (accumulate % 10);
		accumulate /= 10;
	} while(accumulate);

	size_t copied = GEN_MINIMUM(len - *offset, idx);
	if(out_buffer) {
		for(size_t i = copied - 1; i != SIZE_MAX; --i) {
			out_buffer[*offset + i] = out[copied - (i + 1)];
		}
	}
	*offset += copied;
}

static gen_error_t* gen_string_internal_format_number_base16_unsigned(const size_t len, char* const restrict out_buffer, size_t* const restrict offset, const size_t value) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_internal_format_number_base16_unsigned, GEN_FILE_NAME);
    if(error) return error;

	static const char hex_table[] = "0123456789ABCDEF";

	size_t accumulate = value;
	char out[16]; // "FFFFFFFFFFFFFFFF" (i.e. `SIZE_MAX` in hex) is 16 characters

    error = gen_memory_set(out, sizeof(out), '0');
    if(error) return error;

	size_t idx = 0;
	do {
		out[idx++] = hex_table[accumulate % 16];
		accumulate /= 16;
	} while(accumulate);

	size_t copied = GEN_MINIMUM(len - *offset, 16);
	if(out_buffer) {
		for(size_t i = copied - 1; i != SIZE_MAX; --i) {
			out_buffer[*offset + i] = out[copied - (i + 1)];
		}
	}
	*offset += copied;

    return NULL;
}

static void gen_string_internal_format_number_base10_signed(const size_t len, char* const restrict out_buffer, size_t* const restrict offset, const ssize_t value) {
	ssize_t accumulate = value;
	char out[20] = {0}; // "-9223372036854775808" (i.e. `LONG_MIN`) is 20 characters
	size_t idx = 0;
	if(value < 0) {
		accumulate = -value;
		out[idx++] = '-';
	}
	do {
		out[idx++] = '0' + (accumulate % 10);
		accumulate /= 10;
	} while(accumulate);

	size_t copied = GEN_MINIMUM(len - *offset, idx);
	if(out_buffer) {
		for(size_t i = copied - 1; i != SIZE_MAX; --i) {
			out_buffer[*offset + i] = out[copied - (i + 1)];
		}
	}
	*offset += copied;
}

gen_error_t* gen_string_formatv(const size_t limit, char* const restrict out_buffer, size_t* const restrict out_length, const char* const restrict format, const size_t format_length, va_list list) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_formatv, GEN_FILE_NAME);
	if(error) return error;

	if(!format) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`format` was `NULL`");

    // TODO: I don't trust that this applies NULL termination properly - should it?

	size_t out_pos = 0;
	for(size_t i = 0; i < format_length; ++i) {
        if(out_pos == limit) break;

		if(format[i] != '%') {
			if(out_buffer) out_buffer[out_pos] = format[i];
			out_pos++;
			continue;
		}

		if(i + 1 == format_length) return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%s`", i, format);

		switch(format[++i]) {
			case '%': {
				if(out_buffer) out_buffer[out_pos] = '%';
				out_pos++;
				break;
			}

			case 'p': {
				if(out_buffer) {
					error = gen_string_append(out_buffer, limit, "0x", 3, 2);
					if(error) return error;
				}
				out_pos += 2;

				error = gen_string_internal_format_number_base16_unsigned(limit, out_buffer, &out_pos, (size_t) va_arg(list, void*));
                if(error) return error;

				break;
			}

			// TODO: Implement missing specifiers
			case 'f': {
				if(i + 1 == format_length) return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%s`", i, format);
				switch(format[++i]) {
					case 's': {
						return gen_error_attach_backtrace(GEN_ERROR_NOT_IMPLEMENTED, GEN_LINE_NUMBER, "Format specifier `%fs` not implemented");
					}
					case 'd': {
						return gen_error_attach_backtrace(GEN_ERROR_NOT_IMPLEMENTED, GEN_LINE_NUMBER, "Format specifier `%fd` not implemented");
					}
					case 'e': {
						return gen_error_attach_backtrace(GEN_ERROR_NOT_IMPLEMENTED, GEN_LINE_NUMBER, "Format specifier `%fe` not implemented");
					}
					default: {
						return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%s`", i, format);
					}
				}
				// break;
			}
			case 'u': {
				if(i + 1 == format_length) return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%s`", i, format);
				switch(format[++i]) {
					case 'z': {
						gen_string_internal_format_number_base10_unsigned(limit, out_buffer, &out_pos, va_arg(list, size_t));
						break;
					}
					case 'l': {
						gen_string_internal_format_number_base10_unsigned(limit, out_buffer, &out_pos, va_arg(list, unsigned long));
						break;
					}
					case 'i': {
						gen_string_internal_format_number_base10_unsigned(limit, out_buffer, &out_pos, va_arg(list, unsigned int));
						break;
					}
					case 's': {
						gen_string_internal_format_number_base10_unsigned(limit, out_buffer, &out_pos, va_arg(list, unsigned int /* `unsigned short` - promotion */));
						break;
					}
					case 'c': {
						gen_string_internal_format_number_base10_unsigned(limit, out_buffer, &out_pos, va_arg(list, unsigned int /* `unsigned char` - promotion */));
						break;
					}
					default: {
						return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%s`", i - 2, format);
					}
				}
				break;
			}
			case 's': {
				if(i + 1 == format_length) return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%s`", i, format);
				switch(format[++i]) {
					case 'z': {
						gen_string_internal_format_number_base10_signed(limit, out_buffer, &out_pos, va_arg(list, ssize_t));
						break;
					}
					case 'l': {
						gen_string_internal_format_number_base10_signed(limit, out_buffer, &out_pos, va_arg(list, long));
						break;
					}
					case 'i': {
						gen_string_internal_format_number_base10_signed(limit, out_buffer, &out_pos, va_arg(list, int));
						break;
					}
					case 's': {
						gen_string_internal_format_number_base10_signed(limit, out_buffer, &out_pos, va_arg(list, int /* `short` - promotion */));
						break;
					}
					case 'c': {
						gen_string_internal_format_number_base10_signed(limit, out_buffer, &out_pos, va_arg(list, int /* `char` - promotion */));
						break;
					}
					default: {
						return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%s`", i - 2, format);
					}
				}
				break;
			}
			case 't': {
				if(i + 1 == format_length) {
                    const char* string = va_arg(list, const char*);
                    size_t string_length = 0;
                    error = gen_string_length(string, GEN_STRING_NO_BOUNDS, limit - out_pos, &string_length);
                    if(error) return error;
                    if(out_buffer) {
                        error = gen_string_append(out_buffer, limit, string, string_length + 1, string_length);
                        if(error) return error;
                    }
                    out_pos += string_length;
					break;
				}
				switch(format[++i]) {
					case 'z': {
						const char* string = va_arg(list, const char*);
						size_t string_bounds = va_arg(list, size_t);
						size_t string_length = 0;
						error = gen_string_length(string, string_bounds, limit - out_pos, &string_length);
						if(error) return error;
						if(out_buffer) {
							error = gen_string_append(out_buffer, limit, string, string_length + 1, string_length);
							if(error) return error;
						}
						out_pos += string_length;
						break;
					}
					default: {
						--i;
						const char* string = va_arg(list, const char*);
						size_t string_length = 0;
						error = gen_string_length(string, GEN_STRING_NO_BOUNDS, limit - out_pos, &string_length);
						if(error) return error;
						if(out_buffer) {
							error = gen_string_append(out_buffer, limit, string, string_length + 1, string_length);
							if(error) return error;
						}
						out_pos += string_length;
    				}
				}
				break;
			}
			case 'c': {
				if(i + 1 == format_length) {
					char c = (char) va_arg(list, int /* `char` - promotion */);
					if(out_buffer) out_buffer[out_pos] = c;
					out_pos++;
					break;
				}
				switch(format[++i]) {
					case 'z': {
						char c = (char) va_arg(list, int /* `char` - promotion */);
						size_t repetitions = va_arg(list, size_t);

						for(size_t j = 0; j < GEN_MINIMUM(repetitions, limit - out_pos); ++j) {
							if(out_buffer) out_buffer[out_pos] = c;
							out_pos++;
						}
						break;
					}
					default: {
						--i;
						char c = (char) va_arg(list, int /* `char` - promotion */);
						if(out_buffer) out_buffer[out_pos] = c;
						out_pos++;
					}
				}

				break;
			}
			default: {
				return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%s`", i - 1, format);
			}
		}
	}

	if(out_pos > limit) return gen_error_attach_backtrace(GEN_ERROR_UNKNOWN, GEN_LINE_NUMBER, "Something went wrong while formatting string");

	if(out_length) *out_length = out_pos;

	return NULL;
}
