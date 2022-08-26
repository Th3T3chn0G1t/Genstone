// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genstring.h"

#include "include/genmemory.h"

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <string.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

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

	*out_equal = !strncmp(a, b, GEN_MAXIMUM(GEN_MINIMUM(a_length, b_bounds), GEN_MINIMUM(b_length, a_bounds)));

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

	strncpy(destination, source, GEN_MINIMUM(source_length, destination_bounds));

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

	strncat(destination, source, GEN_MINIMUM(source_length, destination_bounds - destination_length));

	return NULL;
}

gen_error_t* gen_string_length(const char* const restrict string, const size_t string_bounds, const size_t limit, size_t* const restrict out_length) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_string_length, GEN_FILE_NAME);
	if(error) return error;

	if(!string) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`string` was `NULL`");
	if(!out_length) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_length` was `NULL`");

	*out_length = strnlen(string, GEN_MINIMUM(limit, string_bounds));

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
	if(!out_length) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_length` was `NULL`");

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
	*out_length = string_length;

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
	for(size_t i = 0; i < string_length; ++i) {
		if(string[i] < '0' || string[i] > '9') return gen_error_attach_backtrace_formatted(GEN_ERROR_BAD_CONTENT, GEN_LINE_NUMBER, "Encountered non-numeric character `%c`", string[i]);
		accumulate += (size_t) (string[i] - '0');
	}
#endif

	*out_number = accumulate;

	return NULL;
}
