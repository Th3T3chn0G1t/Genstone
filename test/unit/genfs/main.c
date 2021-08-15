// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include <genfs.h>

static void dir_handler(const char* file, void* passthrough) {}

// This is to make sure the test value is constant for require
#define file_data "foobarfizzbuzz123"

int main() {
    gen_error_t error = GEN_OK;

    // We can't really check that this is correct in-language
    // We just need to trust the OS implementation of `realpath` or equiv.
    puts("Testing gen_path_canonical()...");
    char canonical[GEN_PATH_MAX];
    error = gen_path_canonical(canonical, ".");

    GEN_REQUIRE_EQUAL(GEN_OK, error);

    puts("Testing gen_path_relative()...");
    char relative[GEN_PATH_MAX];
    error = gen_path_relative(relative, "/foo/bar/fizz", "/foo/bar/");

    GEN_REQUIRE_EQUAL(GEN_OK, error);
    GEN_REQUIRE_EQUAL_STRING("fizz", relative);

    puts("Testing gen_path_filename()...");
    char filename[GEN_PATH_MAX];
    error = gen_path_filename(filename, "foo/bar/fizz.txt");

    GEN_REQUIRE_EQUAL(GEN_OK, error);
    GEN_REQUIRE_EQUAL_STRING("fizz.txt", filename);

    puts("Testing gen_path_pathname()...");
    char pathname[GEN_PATH_MAX];
    error = gen_path_pathname(pathname, "foo/bar/fizz.txt");

    GEN_REQUIRE_EQUAL(GEN_OK, error);
    GEN_REQUIRE_EQUAL_STRING("foo/bar", pathname);

    puts("Testing gen_path_extension()...");
    char extension[GEN_PATH_MAX];
    error = gen_path_extension(extension, "foo/bar/fizz.txt");

    GEN_REQUIRE_EQUAL(GEN_OK, error);
    GEN_REQUIRE_EQUAL_STRING(".txt", extension);

    puts("Testing gen_path_validate()...");

    error = gen_path_validate("foo/bar.txt");
    GEN_REQUIRE_EQUAL(GEN_OK, error);

    error = gen_path_validate("");
    GEN_REQUIRE_EQUAL(GEN_TOO_SHORT, error);

    puts("Testing gen_path_exists()...");
    GEN_REQUIRE_EQUAL(true, gen_path_exists("."));

    puts("Testing gen_handle_open() (file)...");
    gen_filesystem_handle_t file_handle;
    error = gen_handle_open(&file_handle, "./testfile");

    GEN_REQUIRE_EQUAL(GEN_OK, error);
    GEN_REQUIRE_EQUAL(true, gen_path_exists("./testfile"));
    GEN_REQUIRE_EQUAL(false, file_handle.dir);

    puts("Testing gen_file_write()...");
    error = gen_file_write(&file_handle, sizeof(file_data), (const uint8_t*) file_data);

    GEN_REQUIRE_EQUAL(GEN_OK, error);

    puts("Testing gen_handle_size()...");
    const size_t file_size = gen_handle_size(&file_handle);

    GEN_REQUIRE_EQUAL(sizeof(file_data), file_size);

    puts("Testing gen_file_read()...");
    uint8_t file_read_buffer[file_size + 1];
    error = gen_file_read(file_read_buffer, &file_handle, 0, file_size);
    file_read_buffer[file_size] = '\0';

    GEN_REQUIRE_EQUAL(GEN_OK, error);
    GEN_REQUIRE_EQUAL_MEMREGION(file_data, file_read_buffer, sizeof(file_data));

    puts("Testing gen_path_create_dir()...");
    error = gen_path_create_dir("./testdir");

    GEN_REQUIRE_EQUAL(GEN_OK, error);
    GEN_REQUIRE_EQUAL(true, gen_path_exists("./testdir"));

    puts("Testing gen_handle_open() (directory)...");
    gen_filesystem_handle_t dir_handle;
    error = gen_handle_open(&dir_handle, "./testdir");

    GEN_REQUIRE_EQUAL(GEN_OK, error);
    GEN_REQUIRE_EQUAL(true, dir_handle.dir);

    // We can't effectively test this because the test run directory will be different on some
    // platforms
    puts("Testing gen_directory_list()...");
    error = gen_directory_list(&dir_handle, dir_handler, NULL);

    GEN_REQUIRE_EQUAL(GEN_OK, error);

    puts("Testing gen_handle_close() (file)...");
    error = gen_handle_close(&file_handle);

    GEN_REQUIRE_EQUAL(GEN_OK, error);

    puts("Testing gen_handle_close() (directory)...");
    error = gen_handle_close(&dir_handle);

    GEN_REQUIRE_EQUAL(GEN_OK, error);

    puts("Testing gen_path_delete() (file)...");
    error = gen_path_delete("./testfile");

    GEN_REQUIRE_EQUAL(GEN_OK, error);
    GEN_REQUIRE_EQUAL(false, gen_path_exists("./testfile"));

    puts("Testing gen_path_delete() (directory)...");
    error = gen_path_delete("./testdir");

    GEN_REQUIRE_EQUAL(GEN_OK, error);
    GEN_REQUIRE_EQUAL(false, gen_path_exists("./testdir"));
}
