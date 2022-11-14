// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genmath.h"

#include <genbackends.h>

gen_error_t* gen_math_compare_fuzzy_float(const float a, const float b, gen_bool_t* const restrict out_equal) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_math_compare_fuzzy_float, GEN_FILE_NAME);
    if(error) return error;

    error = GEN_BACKENDS_CALL(math_compare_fuzzy_float)(a, b, out_equal);
    if(error) return error;

    return GEN_NULL;
}
gen_error_t* gen_math_compare_fuzzy_double(const double a, const double b, gen_bool_t* const restrict out_equal) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_math_compare_fuzzy_double, GEN_FILE_NAME);
    if(error) return error;

    error = GEN_BACKENDS_CALL(math_compare_fuzzy_double)(a, b, out_equal);
    if(error) return error;

    return GEN_NULL;
}
gen_error_t* gen_math_compare_fuzzy_long_double(const long double a, const long double b, gen_bool_t* const restrict out_equal) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_math_compare_fuzzy_long_double, GEN_FILE_NAME);
    if(error) return error;

    error = GEN_BACKENDS_CALL(math_compare_fuzzy_long_double)(a, b, out_equal);
    if(error) return error;

    return GEN_NULL;
}
