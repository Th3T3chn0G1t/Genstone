// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genfs.h>

int main(void) {
    char canonical[GEN_PATH_MAX];
    gen_error_t error = gen_path_canonical(canonical, ".");
    GEN_REQUIRE_NO_ERROR(error); // Return value depends on environment

    char filename[GEN_PATH_MAX];
    error = gen_path_filename(filename, "/usr/foo.d/path/filename.s.d");
    GEN_REQUIRE_NO_ERROR(error); // Should be filename.s.d

    char pathname[GEN_PATH_MAX];
    error = gen_path_pathname(pathname, "/usr/foo.d/path/filename.s.d");
    GEN_REQUIRE_NO_ERROR(error); // Should be /usr/foo.d/path/

    char extension[GEN_PATH_MAX];
    error = gen_path_extension(extension, "/usr/foo.d/path/filename.s.d");
    GEN_REQUIRE_NO_ERROR(error); // Should be .s.d

    error = gen_path_exists(".");
    GEN_REQUIRE_NO_ERROR(error); // Should be true

    error = gen_path_validate("/usr/foo.d/path/filename.s.d");
    GEN_REQUIRE_NO_ERROR(error); // Should be valid

    error = gen_path_create_file("./testfile");
    GEN_REQUIRE_NO_ERROR(error);

    error = gen_path_create_dir("./testdir");
    GEN_REQUIRE_NO_ERROR(error);

    error = gen_path_delete("./deleteme.txt");
    GEN_REQUIRE_NO_ERROR(error);
}
