// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <genfs.h>

static void dir_handler(__unused const char* file, __unused void* passthrough) {}

// This is to make sure the test value is constant for require
#define file_data "foobarfizzbuzz123"

int main(void) {
	GEN_FRAME_BEGIN(main);

	gen_error_t error = GEN_OK;

	// We can't really check that this is correct in-language
	// We just need to trust the OS implementation of `realpath` or equiv.
	glog(INFO, "Testing gen_path_canonical()...");
	char canonical[GEN_PATH_MAX + 1] = {0};
	error = gen_path_canonical(canonical, ".");

	GEN_REQUIRE_NO_ERROR(error);

	glog(INFO, "Testing gen_path_validate()...");

	error = gen_path_validate("foo/bar.txt");
	GEN_REQUIRE_NO_ERROR(error);

	glog(INFO, "Testing gen_path_exists()...");
	bool exists = false;
	error = gen_path_exists(".", &exists);
	GEN_REQUIRE_NO_ERROR(error);
	GEN_REQUIRE_EQUAL(true, exists);

	glog(INFO, "Testing gen_path_create_file()...");
	error = gen_path_create_file("./testfile");

	GEN_REQUIRE_NO_ERROR(error);
	exists = false;
	error = gen_path_exists("./testfile", &exists);
	GEN_REQUIRE_NO_ERROR(error);
	GEN_REQUIRE_EQUAL(true, exists);

	glog(INFO, "Testing gen_handle_open() (file)...");
	gen_filesystem_handle_t file_handle;
	error = gen_handle_open(&file_handle, "./testfile");

	GEN_REQUIRE_NO_ERROR(error);
	GEN_REQUIRE_EQUAL(false, file_handle.is_directory);

	glog(INFO, "Testing gen_handle_write()...");
	error = gen_handle_write(&file_handle, sizeof(file_data), (const uint8_t*) file_data);

	GEN_REQUIRE_NO_ERROR(error);

	glog(INFO, "Testing gen_handle_size()...");
	size_t file_size;
	error = gen_handle_size(&file_size, &file_handle);

	GEN_REQUIRE_NO_ERROR(error);
	GEN_REQUIRE_EQUAL(sizeof(file_data), file_size);

	glog(INFO, "Testing gen_handle_read()...");
	uint8_t* file_read_buffer = NULL;
	error = gzalloc((void**) &file_read_buffer, file_size + 1, sizeof(uint8_t));
	GEN_REQUIRE_NO_ERROR(error);
	error = gen_handle_read(file_read_buffer, &file_handle, 0, file_size);
	file_read_buffer[file_size] = '\0';

	GEN_REQUIRE_NO_ERROR(error);
	GEN_REQUIRE_EQUAL_MEMREGION((void*) file_data, (void*) file_read_buffer, sizeof(file_data));

	glog(INFO, "Testing gen_path_create_dir()...");
	error = gen_path_create_dir("./testdir");

	GEN_REQUIRE_NO_ERROR(error);
	exists = false;
	error = gen_path_exists("./testdir", &exists);
	GEN_REQUIRE_NO_ERROR(error);
	GEN_REQUIRE_EQUAL(true, exists);

	glog(INFO, "Testing gen_handle_open() (directory)...");
	gen_filesystem_handle_t dir_handle;
	error = gen_handle_open(&dir_handle, "./testdir");

	GEN_REQUIRE_NO_ERROR(error);
	GEN_REQUIRE_EQUAL(true, dir_handle.is_directory);

	// We can't effectively test this because the test run directory will be different on some
	// platforms
	glog(INFO, "Testing gen_directory_list()...");
	error = gen_directory_list(&dir_handle, dir_handler, NULL);

	GEN_REQUIRE_NO_ERROR(error);

	glog(INFO, "Testing gen_handle_close() (file)...");

	error = gen_handle_close(&file_handle);
	GEN_REQUIRE_NO_ERROR(error);

	glog(INFO, "Testing gen_handle_close() (directory)...");

	error = gen_handle_close(&dir_handle);
	GEN_REQUIRE_NO_ERROR(error);

	glog(INFO, "Testing gen_path_delete() (file)...");
	error = gen_path_delete("./testfile");

	GEN_REQUIRE_NO_ERROR(error);
	exists = false;
	error = gen_path_exists("./testfile", &exists);
	GEN_REQUIRE_NO_ERROR(error);
	GEN_REQUIRE_EQUAL(false, exists);

	glog(INFO, "Testing gen_path_delete() (directory)...");
	error = gen_path_delete("./testdir");

	GEN_REQUIRE_NO_ERROR(error);
	exists = false;
	error = gen_path_exists("./testdir", &exists);
	GEN_REQUIRE_NO_ERROR(error);
	GEN_REQUIRE_EQUAL(false, exists);
}
