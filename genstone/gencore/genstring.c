// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genstring.h"

#include "include/gemory.h"

gen_error_t gen_string_compare(const char* const restrict a, const size_t a_bound, const char* const restrict b, const size_t b_bound, const size_t limit, bool* const restrict out_equal) {
	GEN_FRAME_BEGIN(gen_string_compare);

	GEN_NULL_CHECK(a);
	GEN_NULL_CHECK(b);
	GEN_NULL_CHECK(out_equal);

	*out_equal = true;

	if(!limit) GEN_ALL_OK;

	size_t a_length = 0;
	gen_error_t error = gen_string_length(a, a_bound, limit, &a_length);
	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

	size_t b_length = 0;
	error = gen_string_length(b, b_bound, limit, &b_length);
	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

	size_t compare_length = a_length;
	if(limit < compare_length) compare_length = limit;
	if(compare_length > b_length) compare_length = b_length;

	GEN_STRING_FOREACH(c, compare_length, a) {
		if(*c != b[c - a]) {
			*out_equal = false;
			GEN_ALL_OK;
		}
	}

	GEN_ALL_OK;
}

gen_error_t gen_string_copy(char* const restrict destination, const size_t destination_bound, const char* const restrict source, const size_t source_bound, const size_t limit) {
	GEN_FRAME_BEGIN(gen_string_copy);

	GEN_NULL_CHECK(destination);
	GEN_NULL_CHECK(source);

	if(!limit) GEN_ALL_OK;

	GEN_STRING_FOREACH(c, limit < destination_bound ? limit : destination_bound, destination)* c = '\0';

	size_t destination_length = 0;
	gen_error_t error = gen_string_length(destination, destination_bound, limit, &destination_length);
	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

	size_t source_length = 0;
	error = gen_string_length(source, source_bound, limit, &source_length);
	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

	size_t copy_length = source_length;
	if(limit < copy_length) copy_length = limit;

	GEN_STRING_FOREACH(c, copy_length, destination) {
		*c = source[c - destination];
	}

	GEN_ALL_OK;
}

gen_error_t gen_string_append(char* const restrict destination, const size_t destination_bound, const char* const restrict source, const size_t source_bound, const size_t limit) {
	GEN_FRAME_BEGIN(gen_string_append);

	GEN_NULL_CHECK(destination);
	GEN_NULL_CHECK(source);

	if(!limit) GEN_ALL_OK;

	size_t destination_length = 0;
	gen_error_t error = gen_string_length(destination, destination_bound, GEN_STRING_NO_BOUND, &destination_length);
	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

	size_t source_length = 0;
	error = gen_string_length(source, source_bound, limit, &source_length);
	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

	size_t append_length = source_length;
	if(limit < append_length) append_length = limit;

	if(destination_length + append_length + 1 > destination_bound) GEN_ERROR_OUT(GEN_TOO_SHORT, "Length of data to append was greater than string bounds");

	error = gen_string_copy(destination + destination_length, destination_bound - destination_length, source, source_bound, append_length);
	GEN_ERROR_OUT_IF(error, "`gen_string_copy` failed");

	GEN_ALL_OK;
}

gen_error_t gen_string_length(const char* const restrict string, const size_t string_bound, const size_t limit, size_t* const restrict out_length) {
	GEN_FRAME_BEGIN(gen_string_length);

	GEN_NULL_CHECK(string);
	GEN_NULL_CHECK(out_length);

	if(!limit) GEN_ALL_OK;

	*out_length = 0;

	GEN_STRING_FOREACH(c, string_bound, string) {
		if(!*c) GEN_ALL_OK;
		++*out_length;
	}
	if(*out_length > limit) *out_length = limit;

	if(*out_length > limit) *out_length = limit;
	if(limit == GEN_STRING_NO_BOUND) GEN_ERROR_OUT(GEN_TOO_SHORT, "String length exceeded string bounds");

	GEN_ALL_OK;
}

gen_error_t gen_string_duplicate(const char* const restrict string, const size_t string_bound, const size_t limit, char* restrict* const restrict out_duplicated) {
	GEN_FRAME_BEGIN(gen_string_duplicate);

	GEN_NULL_CHECK(string);

	*out_duplicated = NULL;

	if(!limit) GEN_ERROR_OUT(GEN_TOO_SHORT, "`limit` was 0");

	size_t string_length = 0;
	gen_error_t error = gen_string_length(string, string_bound, limit, &string_length);
	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

	size_t duplicate_length = string_length;
	if(limit < duplicate_length) duplicate_length = limit;

	error = gzalloc((void* restrict*) out_duplicated, duplicate_length + 1, sizeof(char));
	GEN_ERROR_OUT_IF(error, "`gzalloc` failed");
	error = gen_string_copy(*out_duplicated, duplicate_length + 1, string, string_bound, limit);
	GEN_ERROR_OUT_IF(error, "`gen_string_copy` failed");

	GEN_ALL_OK;
}

gen_error_t gen_string_character_first(const char* const restrict string, const size_t string_bound, const char character, const size_t limit, const char* restrict* const restrict out_found) {
	GEN_FRAME_BEGIN(gen_string_character_first);

	GEN_NULL_CHECK(string);
	GEN_NULL_CHECK(out_found);

	*out_found = NULL;

	if(!limit) GEN_ALL_OK;

	size_t string_length = 0;
	gen_error_t error = gen_string_length(string, string_bound, limit, &string_length);
	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

	size_t search_length = string_length;
	if(limit < search_length) search_length = limit;

	GEN_STRING_FOREACH(c, search_length, string) {
		if(*c == character) {
			*out_found = c;
			GEN_ALL_OK;
		}
	}

	GEN_ALL_OK;
}

gen_error_t gen_string_character_last(const char* const restrict string, const size_t string_bound, const char character, const size_t limit, const char* restrict* const restrict out_found) {
	GEN_FRAME_BEGIN(gen_string_character_last);

	GEN_NULL_CHECK(string);

	*out_found = NULL;
	GEN_NULL_CHECK(out_found);

	if(!limit) GEN_ALL_OK;

	size_t string_length = 0;
	gen_error_t error = gen_string_length(string, string_bound, limit, &string_length);
	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

	size_t search_length = string_length;
	if(limit < search_length) search_length = limit;

	GEN_STRING_FOREACH(c, search_length, string) {
		const char* const current = ((string + (string_length - 1)) - (c - string));
		if(*current == character) {
			*out_found = current;
			GEN_ALL_OK;
		}
	}

	GEN_ALL_OK;
}

gen_error_t gen_string_number(const char* const restrict string, const size_t string_bound, const size_t limit, size_t* const restrict out_number) {
	GEN_FRAME_BEGIN(gen_string_number);

	GEN_NULL_CHECK(string_bound);
	GEN_NULL_CHECK(out_number);

	char* copy = NULL;
	gen_error_t error = gen_string_duplicate(string, string_bound, limit, &copy);
	GEN_ERROR_OUT_IF(error, "`gen_string_duplicate` failed");
	*out_number = strtoul(copy, NULL, 10);
	GEN_ERROR_OUT_IF_ERRNO(strtoul, errno);
	error = gfree(copy);
	GEN_ERROR_OUT_IF(error, "`gfree` failed");

	GEN_ALL_OK;
}
