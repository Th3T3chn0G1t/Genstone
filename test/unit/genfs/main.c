// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include <genfs.h>

static void dir_handler(const char* file, void* passthrough) {
    assert(file);
}

int main() {
    gen_error_t error = GEN_OK;

    puts("Testing gen_path_canonical()...");
    char canonical[GEN_PATH_MAX];
    error = gen_path_canonical(canonical, ".");

    assert(error == GEN_OK);
    // We can't really check that this is correct in-language
    // We just need to trust the OS implementation
    // And check that the result is at least 1 char in length as a token
    assert(canonical[0]);


    puts("Testing gen_path_relative()...");
    char relative[GEN_PATH_MAX];
    error = gen_path_relative(relative, "/foo/bar/fizz", "/foo/bar/");

    assert(error == GEN_OK);
    assert(!strcmp(relative, "fizz"));


    puts("Testing gen_path_filename()...");
    char filename[GEN_PATH_MAX];
    error = gen_path_filename(filename, "foo/bar/fizz.txt");

    assert(error == GEN_OK);
    assert(!strcmp(filename, "fizz.txt"));


    puts("Testing gen_path_pathname()...");
    char pathname[GEN_PATH_MAX];
    error = gen_path_pathname(pathname, "foo/bar/fizz.txt");

    assert(error == GEN_OK);
    assert(!strcmp(pathname, "foo/bar"));


    puts("Testing gen_path_extension()...");
    char extension[GEN_PATH_MAX];
    error = gen_path_extension(extension, "foo/bar/fizz.txt");

    assert(error == GEN_OK);
    assert(!strcmp(extension, ".txt"));


    puts("Testing gen_path_validate()...");
    error = gen_path_validate("foo/bar.txt");
    assert(error == GEN_OK);

    error = gen_path_validate("");
    assert(error == GEN_TOO_SHORT);


    puts("Testing gen_path_exists()...");
    assert(gen_path_exists("."));


    puts("Testing gen_handle_open() (file)...");
    gen_filesystem_handle_t file_handle;
    error = gen_handle_open(&file_handle, "./testfile");

    assert(error == GEN_OK);
    assert(gen_path_exists("./testfile"));
    assert(!file_handle.dir);


    puts("Testing gen_file_write()...");
    const uint8_t file_data[] = "foobarfizzbuzz123";
    error = gen_file_write(&file_handle, sizeof(file_data), file_data);

    assert(error == GEN_OK);


    puts("Testing gen_handle_size()...");
    const size_t file_size = gen_handle_size(&file_handle);

    assert(file_size == sizeof(file_data));


    puts("Testing gen_file_read()...");
    uint8_t file_read_buffer[file_size + 1];
    error = gen_file_read(file_read_buffer, &file_handle, 0, file_size);
    file_read_buffer[file_size] = '\0';

    assert(error == GEN_OK);
    assert(!strcmp(file_read_buffer, file_data));


    puts("Testing gen_path_create_dir()...");
    error = gen_path_create_dir("./testdir");

    assert(error == GEN_OK);
    assert(gen_path_exists("./testdir"));


    puts("Testing gen_handle_open() (directory)...");
    gen_filesystem_handle_t dir_handle;
    error = gen_handle_open(&dir_handle, "./testdir");

    assert(error == GEN_OK);
    assert(dir_handle.dir);


    // We can't effectively test this because the test run directory will be different on some platforms
    puts("Testing gen_directory_list()...");
    error = gen_directory_list(&dir_handle, dir_handler, NULL);

    assert(error == GEN_OK);


    puts("Testing gen_handle_close() (file)...");
    error = gen_handle_close(&file_handle);

    assert(error == GEN_OK);


    puts("Testing gen_handle_close() (directory)...");
    error = gen_handle_close(&dir_handle);

    assert(error == GEN_OK);


    puts("Testing gen_path_delete() (file)...");
    error = gen_path_delete("./testfile");

    assert(error == GEN_OK);
    assert(!gen_path_exists("./testfile"));


    puts("Testing gen_path_delete() (directory)...");
    error = gen_path_delete("./testdir");

    assert(error == GEN_OK);
    assert(!gen_path_exists("./testdir"));
}
