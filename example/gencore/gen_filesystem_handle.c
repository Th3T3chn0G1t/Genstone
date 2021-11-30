// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genfs.h>

#define EXAMPLE_DATA "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

static void dir_handler(const char* const restrict file, __unused void* const restrict passthrough) {
    glogf(INFO, "%s is a file in a directory!", file);
}

int main(void) {
    gen_filesystem_handle_t file_handle;
	gen_error_t error = gen_handle_open(&file_handle, "./path_to_file");
	GEN_REQUIRE_NO_ERROR(error);

    error = gen_handle_write(&file_handle, sizeof(EXAMPLE_DATA), (const uint8_t*) EXAMPLE_DATA);
	GEN_REQUIRE_NO_ERROR(error);

    size_t file_size;
	error = gen_handle_size(&file_size, &file_handle);
	GEN_REQUIRE_NO_ERROR(error);

    uint8_t file_read_buffer[file_size + 1];
	error = gen_handle_read(file_read_buffer, &file_handle, 0, file_size);
	file_read_buffer[file_size] = '\0';
	GEN_REQUIRE_NO_ERROR(error);

    gen_filesystem_handle_t dir_handle;
	error = gen_handle_open(&dir_handle, "./path_to_directory");
	GEN_REQUIRE_NO_ERROR(error);

    error = gen_directory_list(&dir_handle, dir_handler, NULL);
	GEN_REQUIRE_NO_ERROR(error);

	error = gen_handle_close(&file_handle);
	GEN_REQUIRE_NO_ERROR(error);
}
