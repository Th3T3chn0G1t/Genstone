// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_UNIT "genmemory"
#include <gentests.h>
#include <genmemory.h>

static gen_error_t* gen_main(void) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_main, GEN_FILE_NAME);
    if(error) return error;

    char* buff = NULL;
    error = gen_memory_allocate_zeroed((void**) &buff, 8, sizeof(char));
    if(error) return error;

    error = GEN_TESTS_EXPECT(false, buff == NULL);
    if(error) return error;

    error = gen_memory_reallocate_zeroed((void**) &buff, 8, 9, sizeof(char));
    if(error) return error;

    error = GEN_TESTS_EXPECT(false, buff == NULL);
    if(error) return error;

    size_t* buff_aligned = NULL;
    error = gen_memory_allocate_zeroed_aligned((void**) &buff_aligned, 1, sizeof(size_t), alignof(size_t));
    if(error) return error;

    error = GEN_TESTS_EXPECT(false, buff_aligned == NULL);
    if(error) return error;

    error = gen_memory_set(buff_aligned, sizeof(size_t), 65);
    if(error) return error;

    error = gen_memory_copy(buff, 9 * sizeof(char), buff_aligned, sizeof(size_t), sizeof(size_t));
    if(error) return error;

    error = GEN_TESTS_EXPECT("AAAAAAAA", buff);
    if(error) return error;

    bool equal = false;
    error = gen_memory_compare("AAAAAAAA", sizeof("AAAAAAAA"), buff, 9, 9, &equal);
    if(error) return error;

    error = GEN_TESTS_EXPECT(true, equal);
    if(error) return error;

    error = gen_memory_compare("BBAAAAAA", sizeof("BBAAAAAA"), buff, 9, 9, &equal);
    if(error) return error;

    error = GEN_TESTS_EXPECT(false, equal);
    if(error) return error;

    error = gen_memory_free((void**) &buff);
    if(error) return error;

    error = GEN_TESTS_EXPECT(NULL, buff);
    if(error) return error;

    error = gen_memory_free((void**) &buff_aligned);
    if(error) return error;

    error = GEN_TESTS_EXPECT(NULL, buff_aligned);
    if(error) return error;

    return NULL;
}
