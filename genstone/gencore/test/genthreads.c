// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_UNIT "genthreads"
#include <gentests.h>
#include <genthreads.h>

static gen_error_t* gen_main(void) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_main, GEN_FILE_NAME);
    if(error) return error;

    // TODO: Create actual tests for this once genthreads is implemented

    gen_threads_mutex_t mutex = {0};
    error = gen_threads_mutex_create(&mutex);
    if(error) return error;
    error = gen_threads_mutex_lock(&mutex);
    if(error) return error;
    error = gen_threads_mutex_unlock(&mutex);
    if(error) return error;
    error = gen_threads_mutex_destroy(&mutex);
    if(error) return error;

    return GEN_NULL;
}
