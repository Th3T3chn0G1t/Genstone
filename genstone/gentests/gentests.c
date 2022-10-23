// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_DISABLE
#include "include/gentests.h"

gen_main_t gen_tests_list[GEN_TESTS_MAX] = {0};
const char* gen_tests_name = GEN_TESTS_NAME;

int main(void) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) main, GEN_FILE_NAME);
    if(error) {
        gen_error_print(gen_tests_name, error, GEN_ERROR_SEVERITY_FATAL);
        gen_error_abort();
    }

    size_t len = 0;
    for(; gen_tests_list[len] && len < GEN_TESTS_MAX; ++len);

    for(size_t i = 0; i < len; ++i) {
        gen_log_formatted(GEN_LOG_LEVEL_INFO, gen_tests_name, "Performing test [%uz/%uz]", i + 1, len);
        error = gen_tests_list[i]();
        if(error) {
            gen_error_print(gen_tests_name, error, GEN_ERROR_SEVERITY_FATAL);
            gen_error_abort();
        }
    }
}
