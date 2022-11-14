// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genstring.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <string.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

extern gen_error_t* gen_backends_libc_string_compare(const char* const restrict a, const gen_size_t a_bounds, const gen_size_t a_length, const char* const restrict b, const gen_size_t b_bounds, const gen_size_t b_length, GEN_UNUSED const gen_size_t count, gen_bool_t* const restrict out_equal);
gen_error_t* gen_backends_libc_string_compare(const char* const restrict a, const gen_size_t a_bounds, const gen_size_t a_length, const char* const restrict b, const gen_size_t b_bounds, const gen_size_t b_length, GEN_UNUSED const gen_size_t count, gen_bool_t* const restrict out_equal) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_libc_string_compare, GEN_FILE_NAME);
    if(error) return error;    

	*out_equal = !strncmp(a, b, GEN_MAXIMUM(GEN_MINIMUM(a_length, b_bounds), GEN_MINIMUM(b_length, a_bounds)));

    return GEN_NULL;
}

extern gen_error_t* gen_backends_libc_string_copy(char* const restrict destination, const gen_size_t destination_bounds, const char* const restrict source, GEN_UNUSED const gen_size_t source_bounds, const gen_size_t source_length, GEN_UNUSED const gen_size_t count);
gen_error_t* gen_backends_libc_string_copy(char* const restrict destination, const gen_size_t destination_bounds, const char* const restrict source, GEN_UNUSED const gen_size_t source_bounds, const gen_size_t source_length, GEN_UNUSED const gen_size_t count) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_libc_string_copy, GEN_FILE_NAME);
    if(error) return error;    

	strncpy(destination, source, GEN_MINIMUM(source_length, destination_bounds));

    return GEN_NULL;
}

extern gen_error_t* gen_backends_libc_string_append(char* const restrict destination, const gen_size_t destination_bounds, const gen_size_t destination_length, const char* const restrict source, GEN_UNUSED const gen_size_t source_bounds, const gen_size_t source_length, const gen_size_t count);
gen_error_t* gen_backends_libc_string_append(char* const restrict destination, const gen_size_t destination_bounds, const gen_size_t destination_length, const char* const restrict source, GEN_UNUSED const gen_size_t source_bounds, const gen_size_t source_length, const gen_size_t count) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_libc_string_append, GEN_FILE_NAME);
    if(error) return error;

	strncat(destination, source, GEN_MINIMUM(GEN_MINIMUM(source_length, count), destination_bounds - destination_length));

    return GEN_NULL;
}

extern gen_error_t* gen_backends_libc_string_length(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t limit, gen_size_t* const restrict out_length);
gen_error_t* gen_backends_libc_string_length(const char* const restrict string, const gen_size_t string_bounds, const gen_size_t limit, gen_size_t* const restrict out_length) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_libc_string_length, GEN_FILE_NAME);
    if(error) return error;

	*out_length = strnlen(string, GEN_MINIMUM(limit, string_bounds));

    return GEN_NULL;
}
