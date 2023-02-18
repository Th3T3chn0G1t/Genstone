// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_UNIT "genprocess"
#include <gentests.h>
#include <genprocess.h>
#include <genfilesystem.h>
#include <genmemory.h>

static gen_error_t* gen_main(void) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_main, GEN_FILE_NAME);
    if(error) return error;

    {
        gen_filesystem_handle_t handle = {0};
        error = gen_filesystem_handle_open_anonymous(&handle);
        if(error) return error;

        gen_process_handle_t process = {0};
        error = gen_process_handle_create_with_redirect("echo", sizeof("echo"), sizeof("echo") - 1, (const char*[]) {"-n", "Hello, world!"}, (const gen_size_t[]) {sizeof("-n"), sizeof("Hello, world!")}, 2, GEN_NULL, GEN_NULL, 0, &handle, &process);
        if(error) return error;

        int exit_code = 0;
        error = gen_process_handle_wait(&process, &exit_code);
        if(error) return error;

        error = GEN_TESTS_EXPECT(0, (gen_size_t) exit_code);
        if(error) return error;

        gen_size_t length = 0;
        error = gen_filesystem_handle_file_size(&handle, &length);
        if(error) return error;

        error = GEN_TESTS_EXPECT(sizeof("Hello, world!") - 1, length);
        if(error) return error;
        
        char buffer[sizeof("Hello, world!")] = {0};
        error = gen_filesystem_handle_file_read(&handle, 0, length, (unsigned char*) buffer);
        if(error) return error;

        error = GEN_TESTS_EXPECT("Hello, world!", buffer);
        if(error) return error;
    }
    
    {
        gen_filesystem_handle_t handle = {0};
        error = gen_filesystem_handle_open_anonymous(&handle);
        if(error) return error;

        gen_process_handle_t process = {0};
        error = gen_process_handle_create_with_redirect("sleep", sizeof("sleep"), sizeof("sleep") - 1, (const char*[]) {"100"}, (const gen_size_t[]) {sizeof("100")}, 1, GEN_NULL, GEN_NULL, 0, &handle, &process);
        if(error) return error;

        error = gen_process_handle_kill(&process);
        if(error) return error;
    }

    gen_size_t length = 0;
    error = gen_process_get_environment(GEN_NULL, GEN_NULL, &length);
    if(error) return error;

    gen_size_t* lengths = GEN_NULL;
    error = gen_memory_allocate_zeroed((void**) &lengths, length, sizeof(gen_size_t));
    if(error) return error;

    error = gen_process_get_environment(GEN_NULL, lengths, GEN_NULL);
    if(error) return error;

    char** environment = GEN_NULL;
    error = gen_memory_allocate_zeroed((void**) &environment, length, sizeof(char*));
    if(error) return error;

    for(gen_size_t i = 0; i < length; ++i) {
        error = gen_memory_allocate_zeroed((void**) &environment[i], lengths[i], sizeof(char));
        if(error) return error;
    }

    error = gen_process_get_environment(environment, GEN_NULL, GEN_NULL);
    if(error) return error;

    return GEN_NULL;
}
