// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_DISABLE
#include "include/gentests.h"

#include <genstring.h>
#include <genmath.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <float.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

gen_error_t* gen_tests_expect_pointer(const void* const restrict a, const char* const restrict a_str, const void* const restrict b, const char* const restrict b_str, const char* const restrict file, const gen_size_t line) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_tests_expect_numeric, GEN_FILE_NAME);
    if(error) return error;

    if(a != b) return gen_error_attach_backtrace_formatted(GEN_ERROR_DOES_NOT_MATCH, GEN_LINE_NUMBER, "`%t` (%p) != `%t` (%p) at %t:%uz", a_str, a, b_str, b, file, line);

    return GEN_NULL;
}

gen_error_t* gen_tests_expect_numeric(const gen_size_t a, const char* const restrict a_str, const gen_size_t b, const char* const restrict b_str, const char* const restrict file, const gen_size_t line) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_tests_expect_numeric, GEN_FILE_NAME);
    if(error) return error;

    if(a != b) return gen_error_attach_backtrace_formatted(GEN_ERROR_DOES_NOT_MATCH, GEN_LINE_NUMBER, "`%t` (%uz) != `%t` (%uz) at %t:%uz", a_str, a, b_str, b, file, line);

    return GEN_NULL;
}

gen_error_t* gen_tests_expect_float(const float a, const char* const restrict a_str, const float b, const char* const restrict b_str, const char* const restrict file, const gen_size_t line) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_tests_expect_float, GEN_FILE_NAME);
    if(error) return error;

    gen_bool_t equal = gen_false;
    error = gen_math_compare_fuzzy_float(a, b, &equal);
    if(error) return error;
    
    if(equal) return gen_error_attach_backtrace_formatted(GEN_ERROR_DOES_NOT_MATCH, GEN_LINE_NUMBER, "`%t` (%fs) != `%t` (%fs) at %t:%uz", a_str, (double) a, b_str, (double) b, file, line);

    return GEN_NULL;
}

gen_error_t* gen_tests_expect_double(const double a, const char* const restrict a_str, const double b, const char* const restrict b_str, const char* const restrict file, const gen_size_t line) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_tests_expect_double, GEN_FILE_NAME);
    if(error) return error;

    gen_bool_t equal = gen_false;
    error = gen_math_compare_fuzzy_double(a, b, &equal);
    if(error) return error;

    if(equal) return gen_error_attach_backtrace_formatted(GEN_ERROR_DOES_NOT_MATCH, GEN_LINE_NUMBER, "`%t` (%fd) != `%t` (%fd) at %t:%uz", a_str, a, b_str, b, file, line);

    return GEN_NULL;
}

gen_error_t* gen_tests_expect_long_double(const long double a, const char* const restrict a_str, const long double b, const char* const restrict b_str, const char* const restrict file, const gen_size_t line) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_tests_expect_long_double, GEN_FILE_NAME);
    if(error) return error;

    gen_bool_t equal = gen_false;
    error = gen_math_compare_fuzzy_long_double(a, b, &equal);
    if(error) return error;

    if(equal) return gen_error_attach_backtrace_formatted(GEN_ERROR_DOES_NOT_MATCH, GEN_LINE_NUMBER, "`%t` (%fe) != `%t` (%fe) at %t:%uz", a_str, a, b_str, b, file, line);

    return GEN_NULL;
}

gen_error_t* gen_tests_expect_string(const char* const restrict a, const char* const restrict a_str, const char* const restrict b, const char* const restrict b_str, const char* const restrict file, const gen_size_t line) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_tests_expect_string, GEN_FILE_NAME);
    if(error) return error;

    gen_bool_t equal = gen_false;
    error = gen_string_compare(a, GEN_STRING_NO_BOUNDS, b, GEN_STRING_NO_BOUNDS, GEN_STRING_NO_BOUNDS, &equal);
    if(error) return error;

    if(!equal) return gen_error_attach_backtrace_formatted(GEN_ERROR_DOES_NOT_MATCH, GEN_LINE_NUMBER, "`%t` (%t) != `%t` (%t) at %t:%uz", a_str, a, b_str, b, file, line);

    return GEN_NULL;
}

