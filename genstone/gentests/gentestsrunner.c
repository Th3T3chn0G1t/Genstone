// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_DISABLE
#include "include/gentests.h"

gen_tests_unit_t gen_tests_list[GEN_TESTS_MAX] = {0};
const char* gen_tests_name = GEN_TESTS_NAME;

// We're quite lax about error reporting here because a test runner failure is
// definitely a Genstone bug and not a test failure
int main(void) {
    gen_tooling_push(GEN_FUNCTION_NAME, GEN_FILE_NAME);
    GEN_TOOLING_AUTO gen_error_t* error;

    gen_size_t len = 0;
    for(; gen_tests_list[len].present && len < GEN_TESTS_MAX; ++len);

    gen_bool_t had_failure = gen_false;
    for(gen_size_t i = 0; i < len; ++i) {
        gen_log(
                GEN_LOG_LEVEL_INFO, gen_tests_name,
                "Testing %t... [%uz/%uz]", gen_tests_list[i].name, i + 1, len);
        error = gen_tests_list[i].proc();
        if(error) {
            gen_log(GEN_LOG_LEVEL_ERROR, gen_tests_name, "%e", error);
            had_failure = gen_true;
            gen_log(
                    GEN_LOG_LEVEL_ERROR, gen_tests_name,
                    "Failed!", i + 1, len);
        }
        else {
            gen_log(GEN_LOG_LEVEL_INFO, gen_tests_name, "Passed!", i + 1, len);
        }
    }
    if(had_failure) {
        gen_log(GEN_LOG_LEVEL_FATAL, gen_tests_name, "Test suite failed");
        gen_error_abort();
    }
    else {
        gen_log(GEN_LOG_LEVEL_INFO, gen_tests_name, "All units passing");
    }
}
