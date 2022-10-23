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

#define GEN_TESTS_MAX 512

extern gen_main_t gen_tests_list[GEN_TESTS_MAX];
extern const char* gen_tests_name;

#ifndef GEN_TESTS_DISABLE
static gen_error_t* gen_main(void);

GEN_INITIALIZER static void gen_tests_internal_register_test(void) {
    gen_tests_name = GEN_TESTS_NAME;
    
    size_t i = 0;
    for(; gen_tests_list[i]; ++i);
    if(i >= 512) {
        gen_log_formatted(GEN_LOG_LEVEL_FATAL, GEN_TESTS_NAME, "Number of tests exceeded maximum of %uz", GEN_TESTS_MAX);
        gen_error_abort();
    }
    gen_tests_list[i] = gen_main;
}
#endif

#endif
