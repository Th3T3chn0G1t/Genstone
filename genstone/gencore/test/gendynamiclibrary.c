// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_UNIT "gendynamiclibrary"
#include <gentests.h>
#include <gendynamiclibrary.h>

static gen_error_t* gen_main(void) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_main, GEN_FILE_NAME);
    if(error) return error;

    gen_dynamic_library_handle_t handle = {0};
    // We can expect that gencore is available here
    error = gen_dynamic_library_handle_open("gencore", sizeof("gencore") - 1, &handle);
    if(error) return error;

    void (*proc)(void);
    error = gen_dynamic_library_handle_get_symbol(&handle, "gen_dynamic_library_handle_open", sizeof("gen_dynamic_library_handle_open") - 1, (void**) &proc);
    if(error) return error;

    error = GEN_TESTS_EXPECT((void*) gen_dynamic_library_handle_open, (void*) proc);
    if(error) return error;

    error = gen_dynamic_library_handle_close(&handle);
    if(error) return error;

    return NULL;
}
