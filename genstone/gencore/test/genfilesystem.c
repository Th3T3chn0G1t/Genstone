// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_UNIT "genfilesystem"
#include <gentests.h>
#include <genfilesystem.h>
#include <genmemory.h>
#include <genstring.h>

static gen_error_t* gen_filesystem_test_file_handle(gen_filesystem_handle_t* const restrict handle) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_filesystem_test_file_handle, GEN_FILE_NAME);
    if(error) return error;

    size_t size = 0;
    error = gen_filesystem_handle_file_size(handle, &size);
    if(error) return error;

    error = GEN_TESTS_EXPECT(0, size);
    if(error) return error;

    error = gen_filesystem_handle_file_write(handle, (unsigned char*) "Test data!", 0, sizeof("Test data!") - 1);
    if(error) return error;

    error = gen_filesystem_handle_file_size(handle, &size);
    if(error) return error;

    error = GEN_TESTS_EXPECT(sizeof("Test data!") - 1, size);
    if(error) return error;

    char buffer[sizeof("Test data!")] = {0};
    error = gen_filesystem_handle_file_read(handle, 0, size, (unsigned char*) buffer);
    if(error) return error;

    error = GEN_TESTS_EXPECT("Test data!", buffer);
    if(error) return error;

    // TODO: Test handle locking once `genthreads` is working

    return NULL;
}

static gen_error_t* gen_main(void) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_main, GEN_FILE_NAME);
    if(error) return error;

    {
#if GEN_PLATFORM == GEN_LINUX
        const char path[] = "/./usr/../etc";
        const char expected[] = "/etc";
#elif GEN_PLATFORM == GEN_OSX
        const char path[] = "/./usr/../etc";
        const char expected[] = "/private/etc";
#endif
        size_t canonical_length = 0;
        error = gen_filesystem_path_canonicalize(path, sizeof(path) - 1, NULL, &canonical_length);
        if(error) return error;

        char* canonical = NULL;
        error = gen_memory_allocate_zeroed((void**) &canonical, canonical_length + 1, sizeof(char));
        if(error) return error;

        error = gen_filesystem_path_canonicalize(path, sizeof(path) - 1, canonical, NULL);
        if(error) return error;

        error = GEN_TESTS_EXPECT(expected, canonical);
        if(error) return error;
    }

    {
        bool exists = false;
        error = gen_filesystem_path_exists(".", sizeof(".") - 1, &exists);
        if(error) return error;

        error = GEN_TESTS_EXPECT(true, exists);
        if(error) return error;

        error = gen_filesystem_path_exists("foo/./lets_really_really_hope_that_this_path_doesnt_exist", sizeof("foo/./lets_really_really_hope_that_this_path_doesnt_exist") - 1, &exists);
        if(error) return error;

        error = GEN_TESTS_EXPECT(false, exists);
        if(error) return error;
    }

    {
        error = gen_filesystem_path_validate("test/foo/fizz", sizeof("test/foo/fizz") - 1);
        if(error) return error;
    }

    {
        const char path[] = "__genstone_test_file.txt";

        error = gen_filesystem_path_create_file(path, sizeof(path) - 1);
        if(error) return error;

        bool exists = false;
        error = gen_filesystem_path_exists(path, sizeof(path) - 1, &exists);
        if(error) return error;

        error = GEN_TESTS_EXPECT(true, exists);
        if(error) return error;

        {
            gen_filesystem_handle_t handle = {0};
            error = gen_filesystem_handle_open(path, sizeof(path) - 1, &handle);
            if(error) return error;

            error = GEN_TESTS_EXPECT((uintmax_t) GEN_FILESYSTEM_HANDLE_FILE, handle.type);
            if(error) return error;

            error = gen_filesystem_test_file_handle(&handle);
            if(error) return error;

            error = gen_filesystem_handle_close(&handle);
            if(error) return error;
        }

        error = gen_filesystem_path_delete(path, sizeof(path) - 1);
        if(error) return error;
    }

    {
        const char path[] = "__genstone_test_directory";

        error = gen_filesystem_path_create_directory(path, sizeof(path) - 1);
        if(error) return error;

        bool exists = false;
        error = gen_filesystem_path_exists(path, sizeof(path) - 1, &exists);
        if(error) return error;

        error = GEN_TESTS_EXPECT(true, exists);
        if(error) return error;

        {
            gen_filesystem_handle_t handle = {0};
            error = gen_filesystem_handle_open(path, sizeof(path) - 1, &handle);
            if(error) return error;

            error = GEN_TESTS_EXPECT((uintmax_t) GEN_FILESYSTEM_HANDLE_DIRECTORY, handle.type);
            if(error) return error;

            size_t length = 0;
            // This is tested better below
            error = gen_filesystem_handle_directory_list(&handle, NULL, &length);
            if(error) return error;

            error = GEN_TESTS_EXPECT(0, length);
            if(error) return error;

            error = gen_filesystem_handle_close(&handle);
            if(error) return error;
        }

        error = gen_filesystem_path_delete(path, sizeof(path) - 1);
        if(error) return error;
    }

    {
        gen_filesystem_handle_t handle = {0};
        error = gen_filesystem_handle_open_anonymous(&handle);
        if(error) return error;

        error = GEN_TESTS_EXPECT((uintmax_t) GEN_FILESYSTEM_HANDLE_FILE, handle.type);
        if(error) return error;

        error = gen_filesystem_test_file_handle(&handle);
        if(error) return error;

        error = gen_filesystem_handle_close(&handle);
        if(error) return error;
    }

    {
        gen_filesystem_handle_t handle = {0};
        // This feels a little jank but it's a decent way to get a known directory without creating it ourselves
        error = gen_filesystem_handle_open(GENSTONE_DIR "/build", sizeof(GENSTONE_DIR "/build") - 1, &handle);
        if(error) return error;

        error = GEN_TESTS_EXPECT((uintmax_t) GEN_FILESYSTEM_HANDLE_DIRECTORY, handle.type);
        if(error) return error;

        size_t length = 0;
        error = gen_filesystem_handle_directory_list(&handle, NULL, &length);
        if(error) return error;

        error = GEN_TESTS_EXPECT(4, length);
        if(error) return error;

        char entries[GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX * 4];
        char* list[] = {
            &entries[GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX * 0],
            &entries[GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX * 1],
            &entries[GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX * 2],
            &entries[GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX * 3],
        };
        error = gen_filesystem_handle_directory_list(&handle, list, NULL);
        if(error) return error;

        for(size_t i = 0; i < 4; ++i) {
            bool equal = false;

            error = gen_string_compare("common.mk", sizeof("common.mk") - 1, list[i], GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX, GEN_STRING_NO_BOUNDS, &equal);
            if(error) return error;
            if(equal) continue;

            error = gen_string_compare("config.mk", sizeof("config.mk") - 1, list[i], GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX, GEN_STRING_NO_BOUNDS, &equal);
            if(error) return error;
            if(equal) continue;

            error = gen_string_compare("documentation", sizeof("documentation") - 1, list[i], GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX, GEN_STRING_NO_BOUNDS, &equal);
            if(error) return error;
            if(equal) continue;

            error = gen_string_compare("platform.mk", sizeof("platform.mk") - 1, list[i], GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX, GEN_STRING_NO_BOUNDS, &equal);
            if(error) return error;
            if(equal) continue;

            return gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_CONTROL, GEN_LINE_NUMBER, "Found invalid directory entry `%t` in listing", list[i]);
        }

        error = gen_filesystem_handle_close(&handle);
        if(error) return error;
    }

    {
        const char path[] = "__genstone_test_file.txt";

        error = gen_filesystem_path_create_file(path, sizeof(path) - 1);
        if(error) return error;

        gen_filesystem_handle_t handle = {0};
        error = gen_filesystem_handle_open(path, sizeof(path) - 1, &handle);
        if(error) return error;

        gen_filesystem_watcher_t watcher = {0};
        error = gen_filesystem_watcher_create(&handle, &watcher);
        if(error) return error;

        gen_filesystem_watcher_event_t event = GEN_FILESYSTEM_WATCHER_EVENT_NONE;
        error = gen_filesystem_watcher_poll(&watcher, &event);
        if(error) return error;

        error = GEN_TESTS_EXPECT(true, !!(event & GEN_FILESYSTEM_WATCHER_EVENT_NONE));
        if(error) return error;

        error = gen_filesystem_handle_file_write(&handle, (unsigned char*) "whatever", 0, sizeof("whatever") - 1);
        if(error) return error;

        error = gen_filesystem_watcher_poll(&watcher, &event);
        if(error) return error;

        error = GEN_TESTS_EXPECT(true, !!(event & GEN_FILESYSTEM_WATCHER_EVENT_MODIFIED));
        if(error) return error;

        error = gen_filesystem_path_delete(path, sizeof(path) - 1);
        if(error) return error;

// TODO: Fix `delete-self` check on other platforms
#if GEN_PLATFORM == GEN_LINUX && GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY == GEN_ENABLED && GEN_FILESYSTEM_FORCE_UNIX == GEN_DISABLED
        error = gen_filesystem_watcher_poll(&watcher, &event);
        if(error) return error;

        error = GEN_TESTS_EXPECT(true, !!(event & GEN_FILESYSTEM_WATCHER_EVENT_DELETED));
        if(error) return error;
#endif

        error = gen_filesystem_watcher_destroy(&watcher);
        if(error) return error;

        error = gen_filesystem_handle_close(&handle);
        if(error) return error;
    }

    {
        const char path[] = "__genstone_test_directory";

        error = gen_filesystem_path_create_directory(path, sizeof(path) - 1);
        if(error) return error;

        gen_filesystem_handle_t handle = {0};
        error = gen_filesystem_handle_open(path, sizeof(path) - 1, &handle);
        if(error) return error;

        gen_filesystem_watcher_t watcher = {0};
        error = gen_filesystem_watcher_create(&handle, &watcher);
        if(error) return error;

        error = GEN_TESTS_EXPECT(true, !!watcher.directory_handle);
        if(error) return error;

        gen_filesystem_watcher_event_t event = GEN_FILESYSTEM_WATCHER_EVENT_NONE;
        error = gen_filesystem_watcher_poll(&watcher, &event);
        if(error) return error;

        error = GEN_TESTS_EXPECT(true, !!(event & GEN_FILESYSTEM_WATCHER_EVENT_NONE));
        if(error) return error;

        const char subpath[] = "__genstone_test_directory/__genstone_test_file.txt";

        error = gen_filesystem_path_create_file(subpath, sizeof(subpath) - 1);
        if(error) return error;

        error = gen_filesystem_watcher_poll(&watcher, &event);
        if(error) return error;

        error = GEN_TESTS_EXPECT(true, !!(event & GEN_FILESYSTEM_WATCHER_EVENT_CREATED));
        if(error) return error;

        error = gen_filesystem_path_delete(subpath, sizeof(subpath) - 1);
        if(error) return error;

        error = gen_filesystem_watcher_poll(&watcher, &event);
        if(error) return error;

        error = GEN_TESTS_EXPECT(true, !!(event & GEN_FILESYSTEM_WATCHER_EVENT_DELETED));
        if(error) return error;

        error = gen_filesystem_path_delete(path, sizeof(path) - 1);
        if(error) return error;

        error = gen_filesystem_watcher_destroy(&watcher);
        if(error) return error;

        error = gen_filesystem_handle_close(&handle);
        if(error) return error;
    }

    return NULL;
}
