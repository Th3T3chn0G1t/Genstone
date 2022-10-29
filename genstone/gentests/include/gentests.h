// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_TESTS_H
#define GEN_TESTS_H

#include <gencommon.h>
#include <genlog.h>

typedef gen_error_t* (*gen_main_t)(void);

#ifndef GEN_TESTS_NAME
#define GEN_TESTS_NAME "unnamed-test"
#endif

#ifndef GEN_TESTS_UNIT
#define GEN_TESTS_UNIT "unnamed-unit"
#endif

#define GEN_TESTS_MAX 512

typedef struct {
    bool present;
    const char* name;
    gen_main_t proc;
} gen_tests_unit_t;

extern gen_tests_unit_t gen_tests_list[GEN_TESTS_MAX];
extern const char* gen_tests_name;

extern gen_error_t* gen_tests_expect_pointer(const void* const restrict a, const char* const restrict a_str, const void* const restrict b, const char* const restrict b_str, const char* const restrict file, const size_t line);
extern gen_error_t* gen_tests_expect_numeric(const uintmax_t a, const char* const restrict a_str, const uintmax_t b, const char* const restrict b_str, const char* const restrict file, const size_t line);
extern gen_error_t* gen_tests_expect_float(const float a, const char* const restrict a_str, const float b, const char* const restrict b_str, const char* const restrict file, const size_t line);
extern gen_error_t* gen_tests_expect_double(const double a, const char* const restrict a_str, const double b, const char* const restrict b_str, const char* const restrict file, const size_t line);
extern gen_error_t* gen_tests_expect_long_double(const long double a, const char* const restrict a_str, const long double b, const char* const restrict b_str, const char* const restrict file, const size_t line);
extern gen_error_t* gen_tests_expect_string(const char* const restrict a, const char* const restrict a_str, const char* const restrict b, const char* const restrict b_str, const char* const restrict file, const size_t line);

#define GEN_TESTS_EXPECT(a, b) \
    GEN_GENERIC(a, \
        const char* : gen_tests_expect_string, \
        char* : gen_tests_expect_string, \
        void* : gen_tests_expect_pointer, \
        float : gen_tests_expect_float, \
        double : gen_tests_expect_double, \
        long double : gen_tests_expect_long_double, \
        default : gen_tests_expect_numeric \
    ) (a, #a, b, #b, GEN_FILE_NAME, GEN_LINE_NUMBER)

#ifndef GEN_TESTS_DISABLE
static gen_error_t* gen_main(void);

GEN_INITIALIZER static void gen_tests_internal_register_test(void) {
    gen_tests_name = GEN_TESTS_NAME;
    
    size_t i = 0;
    for(; gen_tests_list[i].present; ++i);
    if(i >= GEN_TESTS_MAX) {
        gen_log_formatted(GEN_LOG_LEVEL_FATAL, GEN_TESTS_NAME, "Number of tests exceeded maximum of %uz", GEN_TESTS_MAX);
        gen_error_abort();
    }

    gen_tests_list[i] = (gen_tests_unit_t) {true, GEN_TESTS_UNIT, gen_main};
}
#endif

#endif
