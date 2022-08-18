// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genstring.h"

#include "include/gemory.h"

gen_error_t gen_string_compare(const char* const restrict a, const size_t a_bound, const char* const restrict b, const size_t b_bound, const size_t limit, bool* const restrict out_equal) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_string_compare", (void*) gen_string_compare, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(a);
	GEN_NULL_CHECK(b);
	GEN_NULL_CHECK(out_equal);

	*out_equal = true;

	if(!limit) return (gen_error_t){GEN_OK, ""};

	size_t a_length = 0;
	gen_error_t error = gen_string_length(a, a_bound, limit, &a_length);
	gen_error_attach_backtrace_IF(error, "`gen_string_length` failed");

	size_t b_length = 0;
	error = gen_string_length(b, b_bound, limit, &b_length);
	gen_error_attach_backtrace_IF(error, "`gen_string_length` failed");

	if(limit == GEN_STRING_NO_BOUND && a_length != b_length) {
		*out_equal = false;
		return (gen_error_t){GEN_OK, ""};
	}

	size_t compare_length = a_length;
	if(limit < compare_length) compare_length = limit;
	if(compare_length > b_length) compare_length = b_length;

	GEN_STRING_FOREACH(c, compare_length, a) {
		if(*c != b[c - a]) {
			*out_equal = false;
			return (gen_error_t){GEN_OK, ""};
		}
	}

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_string_copy(char* const restrict destination, const size_t destination_bound, const char* const restrict source, const size_t source_bound, const size_t limit) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_string_copy", (void*) gen_string_copy, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(destination);
	GEN_NULL_CHECK(source);

	if(!limit) return (gen_error_t){GEN_OK, ""};

	GEN_STRING_FOREACH(c, limit < destination_bound ? limit : destination_bound, destination)* c = '\0';

	size_t destination_length = 0;
	gen_error_t error = gen_string_length(destination, destination_bound, limit, &destination_length);
	gen_error_attach_backtrace_IF(error, "`gen_string_length` failed");

	size_t source_length = 0;
	error = gen_string_length(source, source_bound, limit, &source_length);
	gen_error_attach_backtrace_IF(error, "`gen_string_length` failed");

	size_t copy_length = source_length;
	if(limit < copy_length) copy_length = limit;

	GEN_STRING_FOREACH(c, copy_length, destination) {
		*c = source[c - destination];
	}

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_string_append(char* const restrict destination, const size_t destination_bound, const char* const restrict source, const size_t source_bound, const size_t limit) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_string_append", (void*) gen_string_append, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(destination);
	GEN_NULL_CHECK(source);

	if(!limit) return (gen_error_t){GEN_OK, ""};

	size_t destination_length = 0;
	gen_error_t error = gen_string_length(destination, destination_bound, GEN_STRING_NO_BOUND, &destination_length);
	gen_error_attach_backtrace_IF(error, "`gen_string_length` failed");

	size_t source_length = 0;
	error = gen_string_length(source, source_bound, limit, &source_length);
	gen_error_attach_backtrace_IF(error, "`gen_string_length` failed");

	size_t append_length = source_length;
	if(limit < append_length) append_length = limit;

	if(destination_length + append_length + 1 > destination_bound) gen_error_attach_backtrace(GEN_TOO_SHORT, "Length of data to append was greater than string bounds");

	error = gen_string_copy(destination + destination_length, destination_bound - destination_length, source, source_bound, append_length);
	gen_error_attach_backtrace_IF(error, "`gen_string_copy` failed");

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_string_length(const char* const restrict string, const size_t string_bound, const size_t limit, size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_string_length", (void*) gen_string_length, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(string);
	GEN_NULL_CHECK(out_length);

	if(!limit) return (gen_error_t){GEN_OK, ""};

	*out_length = 0;

	GEN_STRING_FOREACH(c, string_bound, string) {
		if(!*c) return (gen_error_t){GEN_OK, ""};
		++*out_length;
	}

	if(*out_length > limit) *out_length = limit;
	if(limit == GEN_STRING_NO_BOUND) gen_error_attach_backtrace(GEN_TOO_SHORT, "String length exceeded string bounds");

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_string_duplicate(const char* const restrict string, const size_t string_bound, const size_t limit, char* restrict* const restrict out_duplicated) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_string_duplicate", (void*) gen_string_duplicate, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(string);

	*out_duplicated = NULL;

	if(!limit) gen_error_attach_backtrace(GEN_TOO_SHORT, "`limit` was 0");

	size_t string_length = 0;
	gen_error_t error = gen_string_length(string, string_bound, limit, &string_length);
	gen_error_attach_backtrace_IF(error, "`gen_string_length` failed");

	size_t duplicate_length = string_length;
	if(limit < duplicate_length) duplicate_length = limit;

	error = gzalloc((void* restrict*) out_duplicated, duplicate_length + 1, sizeof(char));
	gen_error_attach_backtrace_IF(error, "`gzalloc` failed");
	error = gen_string_copy(*out_duplicated, duplicate_length + 1, string, string_bound, limit);
	gen_error_attach_backtrace_IF(error, "`gen_string_copy` failed");

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_string_character_first(const char* const restrict string, const size_t string_bound, const char character, const size_t limit, const char** const restrict out_found) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_string_character_first", (void*) gen_string_character_first, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(string);
	GEN_NULL_CHECK(out_found);

	*out_found = NULL;

	if(!limit) return (gen_error_t){GEN_OK, ""};

	size_t string_length = 0;
	gen_error_t error = gen_string_length(string, string_bound, limit, &string_length);
	gen_error_attach_backtrace_IF(error, "`gen_string_length` failed");

	size_t search_length = string_length;
	if(limit < search_length) search_length = limit;

	GEN_STRING_FOREACH(c, search_length, string) {
		if(*c == character) {
			*out_found = c;
			return (gen_error_t){GEN_OK, ""};
		}
	}

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_string_character_last(const char* const restrict string, const size_t string_bound, const char character, const size_t limit, const char** const restrict out_found) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_string_character_last", (void*) gen_string_character_last, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(string);

	*out_found = NULL;
	GEN_NULL_CHECK(out_found);

	if(!limit) return (gen_error_t){GEN_OK, ""};

	size_t string_length = 0;
	gen_error_t error = gen_string_length(string, string_bound, limit, &string_length);
	gen_error_attach_backtrace_IF(error, "`gen_string_length` failed");

	size_t search_length = string_length;
	if(limit < search_length) search_length = limit;

	GEN_STRING_FOREACH(c, search_length, string) {
		const char* const current = ((string + (string_length - 1)) - (c - string));
		if(*current == character) {
			*out_found = current;
			return (gen_error_t){GEN_OK, ""};
		}
	}

	return (gen_error_t){GEN_OK, ""};
}

gen_error_t gen_string_number(const char* const restrict string, const size_t string_bound, const size_t limit, size_t* const restrict out_number) {
	GEN_TOOLING_AUTO gen_error_t error = gen_tooling_push("gen_string_number", (void*) gen_string_number, GEN_FILENAME);
	if(error.type) return error;

	GEN_NULL_CHECK(string_bound);
	GEN_NULL_CHECK(out_number);

	char* copy = NULL;
	gen_error_t error = gen_string_duplicate(string, string_bound, limit, &copy);
	gen_error_attach_backtrace_IF(error, "`gen_string_duplicate` failed");
	*out_number = strtoul(copy, NULL, 10);
	gen_error_attach_backtrace_IF_ERRNO(strtoul, errno);
	error = gfree(copy);
	gen_error_attach_backtrace_IF(error, "`gfree` failed");

	return (gen_error_t){GEN_OK, ""};
}
