// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_DISABLE
#include "include/gentests.h"

#include <genstring.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <float.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

gen_error_t* gen_tests_expect_pointer(const void* const restrict a, const char* const restrict a_str, const void* const restrict b, const char* const restrict b_str) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_tests_expect_numeric, GEN_FILE_NAME);
    if(error) return error;

    return gen_tests_expect_numeric((uintmax_t) a, a_str, (uintmax_t) b, b_str);
}

gen_error_t* gen_tests_expect_numeric(const uintmax_t a, const char* const restrict a_str, const uintmax_t b, const char* const restrict b_str) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_tests_expect_numeric, GEN_FILE_NAME);
    if(error) return error;

    if(a != b) return gen_error_attach_backtrace_formatted(GEN_ERROR_DOES_NOT_MATCH, GEN_LINE_NUMBER, "`%t` (%uz) != `%t` (%uz)", a_str, a, b_str, b);

    return NULL;
}

gen_error_t* gen_tests_expect_float(const float a, const char* const restrict a_str, const float b, const char* const restrict b_str) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_tests_expect_float, GEN_FILE_NAME);
    if(error) return error;

    if(a - b > FLT_EPSILON) return gen_error_attach_backtrace_formatted(GEN_ERROR_DOES_NOT_MATCH, GEN_LINE_NUMBER, "`%t` (%fs) != `%t` (%fs)", a_str, a, b_str, b);

    return NULL;
}

gen_error_t* gen_tests_expect_double(const double a, const char* const restrict a_str, const double b, const char* const restrict b_str) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_tests_expect_double, GEN_FILE_NAME);
    if(error) return error;

    if(a - b > DBL_EPSILON) return gen_error_attach_backtrace_formatted(GEN_ERROR_DOES_NOT_MATCH, GEN_LINE_NUMBER, "`%t` (%fd) != `%t` (%fd)", a_str, a, b_str, b);

    return NULL;
}

gen_error_t* gen_tests_expect_long_double(const long double a, const char* const restrict a_str, const long double b, const char* const restrict b_str) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_tests_expect_long_double, GEN_FILE_NAME);
    if(error) return error;

    if(a - b > LDBL_EPSILON) return gen_error_attach_backtrace_formatted(GEN_ERROR_DOES_NOT_MATCH, GEN_LINE_NUMBER, "`%t` (%fe) != `%t` (%fe)", a_str, a, b_str, b);

    return NULL;
}

gen_error_t* gen_tests_expect_string(const char* const restrict a, const char* const restrict a_str, const char* const restrict b, const char* const restrict b_str) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_tests_expect_string, GEN_FILE_NAME);
    if(error) return error;

    bool equal = false;
    error = gen_string_compare(a, GEN_STRING_NO_BOUNDS, b, GEN_STRING_NO_BOUNDS, GEN_STRING_NO_BOUNDS, &equal);
    if(error) return error;

    if(!equal) return gen_error_attach_backtrace_formatted(GEN_ERROR_DOES_NOT_MATCH, GEN_LINE_NUMBER, "`%t` (%t) != `%t` (%t)", a_str, a, b_str, b);

    return NULL;
}

