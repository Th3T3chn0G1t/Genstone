// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genstring.h"

#include "include/genmemory.h"

#include <genstring_be.h>
#include <genbackends.h>

// TODO: We do not error out with bad bounding and instead just
//       Silently use the minimum safe operation.
//       We really should be reporting bounding issues to
//       The user. Ideally this would remove all uses of
//       `GEN_MINIMUM` and `GEN_MAXIMUM`. This goes for the
//       `libc` `genbackends` implementation aswell.

gen_error_t* gen_string_compare(const char* const restrict a, const gen_size_t a_bounds, const gen_size_t a_length, const char* const restrict b, const gen_size_t b_bounds, const gen_size_t b_length, const gen_size_t count, gen_bool_t* const restrict out_equal) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_compare, GEN_FILE_NAME);
	if(error) return error;

	if(!a) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`a` was `GEN_NULL`");
	if(!b) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`b` was `GEN_NULL`");
	if(!out_equal) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_equal` was `GEN_NULL`");

	gen_size_t a_length_checked = 0;
	error = gen_string_length(a, a_bounds, GEN_MINIMUM(a_length, count), &a_length_checked);
	if(error) return error;

	gen_size_t b_length_checked = 0;
	error = gen_string_length(b, b_bounds, GEN_MINIMUM(b_length, count), &b_length_checked);
	if(error) return error;

	if((count == GEN_STRING_NO_BOUNDS || (a_length_checked < count || b_length_checked < count)) && a_length_checked != b_length_checked) {
		*out_equal = gen_false;
		return GEN_NULL;
	}

    error = GEN_BACKENDS_CALL(string_compare)(a, a_bounds, a_length_checked, b, b_bounds, b_length_checked, count, out_equal);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_string_copy(char* const restrict destination, const gen_size_t destination_bounds, const char* const restrict source, const gen_size_t source_bounds, const gen_size_t source_length, const gen_size_t count) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_copy, GEN_FILE_NAME);
	if(error) return error;

	if(!destination) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`destination` was `GEN_NULL`");
	if(!source) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`source` was `GEN_NULL`");

	gen_size_t source_length_checked = 0;
	error = gen_string_length(source, source_bounds, GEN_MINIMUM(source_length, count), &source_length_checked);
	if(error) return error;

    error = GEN_BACKENDS_CALL(string_copy)(destination, destination_bounds, source, source_bounds, source_length_checked, count);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_string_append(char* const restrict destination, const gen_size_t destination_bounds, const gen_size_t destination_length, const char* const restrict source, const gen_size_t source_bounds, const gen_size_t source_length, const gen_size_t count) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_append, GEN_FILE_NAME);
	if(error) return error;

	if(!destination) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`destination` was `GEN_NULL`");
	if(!source) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`source` was `GEN_NULL`");

	gen_size_t destination_length_checked = 0;
	error = gen_string_length(destination, destination_bounds, destination_length, &destination_length_checked);
	if(error) return error;

	gen_size_t source_length_checked = 0;
	error = gen_string_length(source, source_bounds, GEN_MINIMUM(source_length, count), &source_length_checked);
	if(error) return error;

    error = GEN_BACKENDS_CALL(string_append)(destination, destination_bounds, destination_length_checked, source, source_bounds, source_length_checked, count);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_string_length(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t limit, gen_size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_length, GEN_FILE_NAME);
	if(error) return error;

	if(!string) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `GEN_NULL`");
	if(!out_length) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_length` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(string_length)(string, string_bounds, limit, out_length);
	if(error) return error;

	return GEN_NULL;
}

static void gen_string_internal_duplicate_cleanup_duplicated(char** duplicated) {
	if(*duplicated) return;

	gen_error_t* error = gen_memory_free((void**) duplicated);
	if(error) gen_error_abort_with_error(error, "genstring");
}

gen_error_t* gen_string_duplicate(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t string_length, const gen_size_t count, char* restrict* const restrict out_duplicated, gen_size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_duplicate, GEN_FILE_NAME);
	if(error) return error;

	if(!string) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `GEN_NULL`");
	if(!out_duplicated) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_duplicated` was `GEN_NULL`");

	gen_size_t string_length_checked = 0;
	error = gen_string_length(string, string_bounds, GEN_MINIMUM(string_length, count), &string_length_checked);
	if(error) return error;

	char* duplicated = GEN_NULL;
	GEN_CLEANUP_FUNCTION(gen_string_internal_duplicate_cleanup_duplicated) char* duplicated_scope_variable = duplicated;
	error = gen_memory_allocate_zeroed((void**) &duplicated, string_length_checked + 1, sizeof(char));
	if(error) return error;

	error = gen_string_copy(duplicated, string_length_checked + 1, string, string_bounds, string_length_checked, string_length_checked);
	if(error) return error;

	*out_duplicated = duplicated;
	if(out_length) *out_length = string_length;

	duplicated_scope_variable = GEN_NULL;

	return GEN_NULL;
}

gen_error_t* gen_string_character_first(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t string_length, const char character, const gen_size_t count, gen_size_t* const restrict out_found) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_character_first, GEN_FILE_NAME);
	if(error) return error;

	if(!string) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `GEN_NULL`");
	if(!out_found) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_found` was `GEN_NULL`");

	gen_size_t string_length_checked = 0;
	error = gen_string_length(string, string_bounds, GEN_MINIMUM(string_length, count), &string_length_checked);
	if(error) return error;

	for(gen_size_t i = 0; i < string_length_checked; ++i) {
		if(string[i] == character) {
			*out_found = i;

			return GEN_NULL;
		}
	}

	*out_found = GEN_STRING_NO_BOUNDS;

	return GEN_NULL;
}

gen_error_t* gen_string_character_last(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t string_length, const char character, const gen_size_t count, gen_size_t* const restrict out_found) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_character_last, GEN_FILE_NAME);
	if(error) return error;

	if(!string) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `GEN_NULL`");
	if(!out_found) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_found` was `GEN_NULL`");

	gen_size_t string_length_checked = 0;
	error = gen_string_length(string, string_bounds, GEN_MINIMUM(string_length, count), &string_length_checked);
	if(error) return error;

	for(gen_size_t i = string_length_checked - 1; i != GEN_SIZE_MAX; --i) {
		if(string[i] == character) {
			*out_found = i;

			return GEN_NULL;
		}
	}

	*out_found = GEN_SIZE_MAX;

	return GEN_NULL;
}

gen_error_t* gen_string_number(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t string_length, const gen_size_t count, gen_size_t* const restrict out_number) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_number, GEN_FILE_NAME);
	if(error) return error;

	if(!out_number) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_number` was `GEN_NULL`");

	gen_size_t string_length_checked = 0;
	error = gen_string_length(string, string_bounds, GEN_MINIMUM(string_length, count), &string_length_checked);
	if(error) return error;

	gen_size_t accumulate = 0;
    gen_size_t pow = 1;
	for(gen_size_t i = string_length_checked - 1; i != GEN_SIZE_MAX; --i) {
		if(string[i] < '0' || string[i] > '9') return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Encountered non-numeric character `%c`", string[i]);
		accumulate += (gen_size_t) (string[i] - '0') * pow;
        pow *= 10;
	}

	*out_number = accumulate;

	return GEN_NULL;
}

gen_error_t* gen_string_format(const gen_size_t limit, char* const restrict out_buffer, gen_size_t* out_length, const char* const restrict format, const gen_size_t format_bounds, const gen_size_t format_length, ...) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_format, GEN_FILE_NAME);
	if(error) return error;

	gen_variadic_list_t list;
	gen_variadic_list_start(list, format_length);

	return gen_string_formatv(limit, out_buffer, out_length, format, format_bounds, format_length, list);
}

static void gen_string_internal_format_number_base10_unsigned(const gen_size_t len, char* const restrict out_buffer, gen_size_t* const restrict offset, const gen_size_t value) {
	gen_size_t accumulate = value;
	char out[20] = {0}; // "18446744073709551615" (i.e. `GEN_SIZE_MAX`) is 20 characters
	gen_size_t idx = 0;
	do {
		out[idx++] = '0' + (char) (accumulate % 10);
		accumulate /= 10;
	} while(accumulate);

	gen_size_t copied = GEN_MINIMUM(len - *offset, idx);
	if(out_buffer) {
		for(gen_size_t i = copied - 1; i != GEN_SIZE_MAX; --i) {
			out_buffer[*offset + i] = out[copied - (i + 1)];
		}
	}
	*offset += copied;
}

static gen_error_t* gen_string_internal_format_number_base16_unsigned(const gen_size_t len, char* const restrict out_buffer, gen_size_t* const restrict offset, const gen_size_t value) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_internal_format_number_base16_unsigned, GEN_FILE_NAME);
    if(error) return error;

	static const char hex_table[] = "0123456789ABCDEF";

	gen_size_t accumulate = value;
	char out[16]; // "FFFFFFFFFFFFFFFF" (i.e. `GEN_SIZE_MAX` in hex) is 16 characters

    error = gen_memory_set(out, sizeof(out), sizeof(out), '0');
    if(error) return error;

	gen_size_t idx = 0;
	do {
		out[idx++] = hex_table[accumulate % 16];
		accumulate /= 16;
	} while(accumulate);

	gen_size_t copied = GEN_MINIMUM(len - *offset, 16);
	if(out_buffer) {
		for(gen_size_t i = copied - 1; i != GEN_SIZE_MAX; --i) {
			out_buffer[*offset + i] = out[copied - (i + 1)];
		}
	}
	*offset += copied;

    return GEN_NULL;
}

static void gen_string_internal_format_number_base10_signed(const gen_size_t len, char* const restrict out_buffer, gen_size_t* const restrict offset, const gen_ssize_t value) {
	gen_ssize_t accumulate = value;
	char out[20] = {0}; // "-9223372036854775808" (i.e. `LONG_MIN`) is 20 characters
	gen_size_t idx = 0;
	if(value < 0) {
		accumulate = -value;
		out[idx++] = '-';
	}
	do {
		out[idx++] = '0' + (char) (accumulate % 10);
		accumulate /= 10;
	} while(accumulate);

	gen_size_t copied = GEN_MINIMUM(len - *offset, idx);
	if(out_buffer) {
		for(gen_size_t i = copied - 1; i != GEN_SIZE_MAX; --i) {
			out_buffer[*offset + i] = out[copied - (i + 1)];
		}
	}
	*offset += copied;
}

gen_error_t* gen_string_formatv(const gen_size_t limit, char* const restrict out_buffer, gen_size_t* const restrict out_length, const char* const restrict format, const gen_size_t format_bounds, const gen_size_t format_length, gen_variadic_list_t list) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_formatv, GEN_FILE_NAME);
	if(error) return error;

	if(!format) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`format` was `GEN_NULL`");

    // TODO: I don't trust that this applies GEN_NULL termination properly - should it?

    gen_size_t format_length_checked = 0;
    error = gen_string_length(format, format_bounds, format_length, &format_length_checked);
	if(error) return error;

	gen_size_t out_pos = 0;
	for(gen_size_t i = 0; i < format_length_checked; ++i) {
        if(out_pos == limit) break;

		if(format[i] != '%') {
			if(out_buffer) out_buffer[out_pos] = format[i];
			out_pos++;
			continue;
		}

		if(i + 1 == format_length_checked) return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%t`", i, format);

		switch(format[++i]) {
			case '%': {
				if(out_buffer) out_buffer[out_pos] = '%';
				out_pos++;
				break;
			}

			case 'p': {
				if(out_buffer) {
					error = gen_string_append(out_buffer, GEN_STRING_NO_BOUNDS, limit, "0x", 3, 2, 2);
					if(error) return error;
				}
				out_pos += 2;

				error = gen_string_internal_format_number_base16_unsigned(limit, out_buffer, &out_pos, (gen_size_t) gen_variadic_list_argument(list, void*));
                if(error) return error;

				break;
			}

			// TODO: Implement missing specifiers
			case 'f': {
				if(i + 1 == format_length_checked) return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%t`", i, format);
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
						return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%t`", i, format);
					}
				}
				// break;
			}
			case 'u': {
				if(i + 1 == format_length_checked) return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%t`", i, format);
				switch(format[++i]) {
					case 'z': {
						gen_string_internal_format_number_base10_unsigned(limit, out_buffer, &out_pos, gen_variadic_list_argument(list, gen_size_t));
						break;
					}
					case 'l': {
						gen_string_internal_format_number_base10_unsigned(limit, out_buffer, &out_pos, gen_variadic_list_argument(list, unsigned long));
						break;
					}
					case 'i': {
						gen_string_internal_format_number_base10_unsigned(limit, out_buffer, &out_pos, gen_variadic_list_argument(list, unsigned int));
						break;
					}
					case 's': {
						gen_string_internal_format_number_base10_unsigned(limit, out_buffer, &out_pos, gen_variadic_list_argument(list, unsigned int /* `unsigned short` - promotion */));
						break;
					}
					case 'c': {
						gen_string_internal_format_number_base10_unsigned(limit, out_buffer, &out_pos, gen_variadic_list_argument(list, unsigned int /* `unsigned char` - promotion */));
						break;
					}
					default: {
						return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%t`", i - 2, format);
					}
				}
				break;
			}
			case 's': {
				if(i + 1 == format_length_checked) return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%t`", i, format);
				switch(format[++i]) {
					case 'z': {
						gen_string_internal_format_number_base10_signed(limit, out_buffer, &out_pos, gen_variadic_list_argument(list, gen_ssize_t));
						break;
					}
					case 'l': {
						gen_string_internal_format_number_base10_signed(limit, out_buffer, &out_pos, gen_variadic_list_argument(list, long));
						break;
					}
					case 'i': {
						gen_string_internal_format_number_base10_signed(limit, out_buffer, &out_pos, gen_variadic_list_argument(list, int));
						break;
					}
					case 's': {
						gen_string_internal_format_number_base10_signed(limit, out_buffer, &out_pos, gen_variadic_list_argument(list, int /* `short` - promotion */));
						break;
					}
					case 'c': {
						gen_string_internal_format_number_base10_signed(limit, out_buffer, &out_pos, gen_variadic_list_argument(list, int /* `char` - promotion */));
						break;
					}
					default: {
						return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%t`", i - 2, format);
					}
				}
				break;
			}
			case 't': {
				if(i + 1 == format_length_checked) {
                    const char* string = gen_variadic_list_argument(list, const char*);
                    gen_size_t string_length = 0;
                    error = gen_string_length(string, GEN_STRING_NO_BOUNDS, limit - out_pos, &string_length);
                    if(error) return error;
                    if(out_buffer) {
                        error = gen_string_append(out_buffer, GEN_STRING_NO_BOUNDS, limit, string, string_length + 1, string_length, string_length);
                        if(error) return error;
                    }
                    out_pos += string_length;
					break;
				}
				switch(format[++i]) {
					case 'z': {
						const char* string = gen_variadic_list_argument(list, const char*);
						gen_size_t string_bounds = gen_variadic_list_argument(list, gen_size_t);
						gen_size_t string_length = 0;
						error = gen_string_length(string, string_bounds, limit - out_pos, &string_length);
						if(error) return error;
						if(out_buffer) {
							error = gen_string_append(out_buffer, GEN_STRING_NO_BOUNDS, limit, string, string_length + 1, string_length, string_length);
							if(error) return error;
						}
						out_pos += string_length;
						break;
					}
					default: {
						--i;
						const char* string = gen_variadic_list_argument(list, const char*);
						gen_size_t string_length = 0;
						error = gen_string_length(string, GEN_STRING_NO_BOUNDS, limit - out_pos, &string_length);
						if(error) return error;
						if(out_buffer) {
							error = gen_string_append(out_buffer, GEN_STRING_NO_BOUNDS, limit, string, string_length + 1, string_length, string_length);
							if(error) return error;
						}
						out_pos += string_length;
    				}
				}
				break;
			}
			case 'c': {
				if(i + 1 == format_length_checked) {
					char c = (char) gen_variadic_list_argument(list, int /* `char` - promotion */);
					if(out_buffer) out_buffer[out_pos] = c;
					out_pos++;
					break;
				}
				switch(format[++i]) {
					case 'z': {
						char c = (char) gen_variadic_list_argument(list, int /* `char` - promotion */);
						gen_size_t repetitions = gen_variadic_list_argument(list, gen_size_t);

						for(gen_size_t j = 0; j < GEN_MINIMUM(repetitions, limit - out_pos); ++j) {
							if(out_buffer) out_buffer[out_pos] = c;
							out_pos++;
						}
						break;
					}
					default: {
						--i;
						char c = (char) gen_variadic_list_argument(list, int /* `char` - promotion */);
						if(out_buffer) out_buffer[out_pos] = c;
						out_pos++;
					}
				}

				break;
			}
			default: {
				return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Invalid format specifier at position %uz in string `%t`", i - 1, format);
			}
		}
	}

	if(out_pos > limit) return gen_error_attach_backtrace(GEN_ERROR_UNKNOWN, GEN_LINE_NUMBER, "Something went wrong while formatting string");

	if(out_length) *out_length = out_pos;

	return GEN_NULL;
}

gen_error_t* gen_string_contains(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t string_length, const char* const restrict target, const gen_size_t target_bounds, const gen_size_t target_length, const gen_size_t count, gen_size_t* const restrict out_offset) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_contains, GEN_FILE_NAME);
	if(error) return error;

    if(!string) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `GEN_NULL`");
    if(!target) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`target` was `GEN_NULL`");
    if(!out_offset) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_offset` was `GEN_NULL`");

    gen_size_t string_length_checked = 0;
    error = gen_string_length(string, string_bounds, GEN_MINIMUM(count, string_length), &string_length_checked);
	if(error) return error;

    gen_size_t target_length_checked = 0;
    error = gen_string_length(target, target_bounds, target_length, &target_length_checked);
	if(error) return error;

    for(gen_size_t i = 0; i < string_length_checked - (target_length_checked - 1); ++i) {
        gen_bool_t equal = gen_false;
        error = gen_string_compare(&string[i], string_bounds - i, string_bounds - (i + 1), target, target_length_checked + 1, GEN_MINIMUM(target_length_checked, string_length_checked - i), GEN_MINIMUM(target_length_checked, string_length_checked - i), &equal);
        if(error) return error;

        if(equal) {
            *out_offset = i;
            return GEN_NULL;
        }
    }

    *out_offset = GEN_SIZE_MAX;

    return GEN_NULL;
}
