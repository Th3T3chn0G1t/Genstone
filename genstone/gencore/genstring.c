// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genstring.h"

#include "include/gemory.h"

gen_error_t gen_string_compare(const char* const restrict a, const size_t a_bound, const char* const restrict b, const size_t b_bound, const size_t limit, bool* const restrict out_equal) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_compare, GEN_FILE_NAME);
	if(error.type) return error;

	if(!a) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`a` was `NULL`");
	if(!b) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`b` was `NULL`");
	if(!out_equal) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_equal` was `NULL`");

	*out_equal = true;

	if(!limit) return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};

	size_t a_length = 0;
	gen_error_t error = gen_string_length(a, a_bound, limit, &a_length);
	if(error.type) return error;

	size_t b_length = 0;
	error = gen_string_length(b, b_bound, limit, &b_length);
	if(error.type) return error;

	if(limit == GEN_STRING_NO_BOUND && a_length != b_length) {
		*out_equal = false;
		return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
	}

	size_t compare_length = a_length;
	if(limit < compare_length) compare_length = limit;
	if(compare_length > b_length) compare_length = b_length;

	GEN_STRING_FOREACH(c, compare_length, a) {
		if(*c != b[c - a]) {
			*out_equal = false;
			return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
		}
	}

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_string_copy(char* const restrict destination, const size_t destination_bound, const char* const restrict source, const size_t source_bound, const size_t limit) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_copy, GEN_FILE_NAME);
	if(error.type) return error;

	if(!destination) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`destination` was `NULL`");
	if(!source) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`source` was `NULL`");

	if(!limit) return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};

	GEN_STRING_FOREACH(c, limit < destination_bound ? limit : destination_bound, destination)* c = '\0';

	size_t destination_length = 0;
	gen_error_t error = gen_string_length(destination, destination_bound, limit, &destination_length);
	if(error.type) return error;

	size_t source_length = 0;
	error = gen_string_length(source, source_bound, limit, &source_length);
	if(error.type) return error;

	size_t copy_length = source_length;
	if(limit < copy_length) copy_length = limit;

	GEN_STRING_FOREACH(c, copy_length, destination) {
		*c = source[c - destination];
	}

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_string_append(char* const restrict destination, const size_t destination_bound, const char* const restrict source, const size_t source_bound, const size_t limit) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_append, GEN_FILE_NAME);
	if(error.type) return error;

	if(!destination) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`destination` was `NULL`");
	if(!source) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`source` was `NULL`");

	if(!limit) return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};

	size_t destination_length = 0;
	gen_error_t error = gen_string_length(destination, destination_bound, GEN_STRING_NO_BOUND, &destination_length);
	if(error.type) return error;

	size_t source_length = 0;
	error = gen_string_length(source, source_bound, limit, &source_length);
	if(error.type) return error;

	size_t append_length = source_length;
	if(limit < append_length) append_length = limit;

	if(destination_length + append_length + 1 > destination_bound) return gen_error_attach_backtrace(GEN_TOO_SHORT, GEN_LINE_NUMBER, "Length of data to append was greater than string bounds");

	error = gen_string_copy(destination + destination_length, destination_bound - destination_length, source, source_bound, append_length);
	if(error.type) return error;

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_string_length(const char* const restrict string, const size_t string_bound, const size_t limit, size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_length, GEN_FILE_NAME);
	if(error.type) return error;

	if(!string) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `NULL`");
	if(!out_length) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_length` was `NULL`");

	if(!limit) return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};

	*out_length = 0;

	GEN_STRING_FOREACH(c, string_bound, string) {
		if(!*c) return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
		++*out_length;
	}

	if(*out_length > limit) *out_length = limit;
	if(limit == GEN_STRING_NO_BOUND) return gen_error_attach_backtrace(GEN_TOO_SHORT, GEN_LINE_NUMBER, "String length exceeded string bounds");

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_string_duplicate(const char* const restrict string, const size_t string_bound, const size_t limit, char* restrict* const restrict out_duplicated) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_duplicate, GEN_FILE_NAME);
	if(error.type) return error;

	if(!string) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `NULL`");

	*out_duplicated = NULL;

	if(!limit) return gen_error_attach_backtrace(GEN_TOO_SHORT, GEN_LINE_NUMBER, "`limit` was 0");

	size_t string_length = 0;
	gen_error_t error = gen_string_length(string, string_bound, limit, &string_length);
	if(error.type) return error;

	size_t duplicate_length = string_length;
	if(limit < duplicate_length) duplicate_length = limit;

	error = gzalloc((void* restrict*) out_duplicated, duplicate_length + 1, sizeof(char));
	if(error.type) return error;
	error = gen_string_copy(*out_duplicated, duplicate_length + 1, string, string_bound, limit);
	if(error.type) return error;

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_string_character_first(const char* const restrict string, const size_t string_bound, const char character, const size_t limit, const char** const restrict out_found) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_character_first, GEN_FILE_NAME);
	if(error.type) return error;

	if(!string) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `NULL`");
	if(!out_found) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_found` was `NULL`");

	*out_found = NULL;

	if(!limit) return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};

	size_t string_length = 0;
	gen_error_t error = gen_string_length(string, string_bound, limit, &string_length);
	if(error.type) return error;

	size_t search_length = string_length;
	if(limit < search_length) search_length = limit;

	GEN_STRING_FOREACH(c, search_length, string) {
		if(*c == character) {
			*out_found = c;
			return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
		}
	}

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_string_character_last(const char* const restrict string, const size_t string_bound, const char character, const size_t limit, const char** const restrict out_found) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_character_last, GEN_FILE_NAME);
	if(error.type) return error;

	if(!string) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `NULL`");

	*out_found = NULL;
	if(!out_found) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_found` was `NULL`");

	if(!limit) return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};

	size_t string_length = 0;
	gen_error_t error = gen_string_length(string, string_bound, limit, &string_length);
	if(error.type) return error;

	size_t search_length = string_length;
	if(limit < search_length) search_length = limit;

	GEN_STRING_FOREACH(c, search_length, string) {
		const char* const current = ((string + (string_length - 1)) - (c - string));
		if(*current == character) {
			*out_found = current;
			return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
		}
	}

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}

gen_error_t gen_string_number(const char* const restrict string, const size_t string_bound, const size_t limit, size_t* const restrict out_number) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_number, GEN_FILE_NAME);
	if(error.type) return error;

	if(!string_bound) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string_bound` was `NULL`");
	if(!out_number) return gen_error_attach_backtrace(GEN_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_number` was `NULL`");

	char* copy = NULL;
	gen_error_t error = gen_string_duplicate(string, string_bound, limit, &copy);
	if(error.type) return error;
	*out_number = strtoul(copy, NULL, 10);
	gen_error_attach_backtrace_IF_ERRNO(strtoul, errno);
	error = gfree(copy);
	if(error.type) return error;

	return (gen_error_t){GEN_OK, GEN_LINE_NUMBER, ""};
}
