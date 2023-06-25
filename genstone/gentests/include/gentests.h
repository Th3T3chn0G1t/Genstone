// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

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
    gen_bool_t present;
    const char* name;
    gen_main_t proc;
} gen_tests_unit_t;

extern gen_tests_unit_t gen_tests_list[GEN_TESTS_MAX];
extern const char* gen_tests_name;

#define GEN_TESTS_EXPECT_INTERNAL(a, b) \
        return gen_error_attach_backtrace( \
            GEN_ERROR_DOES_NOT_MATCH, GEN_LINE_STRING, "`" a "` != `" b "`")


#define GEN_TESTS_EXPECT(a, b) \
        if((a) != (b)) GEN_TESTS_EXPECT_INTERNAL(#a, #b)

#define GEN_TESTS_EXPECT_STRING(a, b) \
        if(({ \
            gen_size_t i = 0; \
            for(; (a)[i] && (b)[i] && (a)[i] == (b)[i]; ++i); \
            (a)[i] != (b)[i]; \
        })) GEN_TESTS_EXPECT_INTERNAL(#a, #b)

#ifndef GEN_TESTS_DISABLE
static gen_error_t* gen_main(void);

GEN_INITIALIZER static void gen_tests_internal_register_test(void) {
    gen_tests_name = GEN_TESTS_NAME;
    
    gen_size_t i = 0;
    for(; gen_tests_list[i].present; ++i);
    if(i >= GEN_TESTS_MAX) {
        gen_log(
                GEN_LOG_LEVEL_FATAL, GEN_TESTS_NAME,
                "Number of tests exceeded maximum of %uz",
                (gen_size_t) GEN_TESTS_MAX);
        gen_abort();
    }

    gen_tests_list[i] = (gen_tests_unit_t) {
                            gen_true, GEN_TESTS_UNIT, gen_main };
}
#endif

#endif
