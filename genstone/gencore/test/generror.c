// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_UNIT "generror"
#include <gentests.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <stdlib.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

extern int main(void);

static gen_error_t* gen_main(void) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_main, GEN_FILE_NAME);
    if(error) return error;

    gen_error_t* test_error = gen_error_attach_backtrace_formatted(GEN_ERROR_UNKNOWN, GEN_LINE_NUMBER, "Test error %uz", 42);
    
    {
        error = GEN_TESTS_EXPECT((uintmax_t) GEN_ERROR_UNKNOWN, test_error->type);
        if(error) return error;
        error = GEN_TESTS_EXPECT(18, test_error->line);
        if(error) return error;
        error = GEN_TESTS_EXPECT("Test error 42", test_error->context);
        if(error) return error;
    }

    {
        error = GEN_TESTS_EXPECT(2, test_error->backtrace_length);
        if(error) return error;

        error = GEN_TESTS_EXPECT("main", test_error->backtrace[0].function);
        if(error) return error;
        error = GEN_TESTS_EXPECT((void*) main, (const void*) test_error->backtrace[0].address);
        if(error) return error;
        // We can't reliably check file since the path
        // will change depending on whether Genstone was
        // Built as a subproject or not

        error = GEN_TESTS_EXPECT("gen_main", test_error->backtrace[1].function);
        if(error) return error;
        error = GEN_TESTS_EXPECT((void*) gen_main, (const void*) test_error->backtrace[1].address);
        if(error) return error;
    }
    

    return NULL;
}
