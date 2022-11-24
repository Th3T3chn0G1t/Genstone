// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genfilesystem.h>

#include <genfilesystem_be.h>
#include <genbackendslibcerror.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <fcntl.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

extern gen_error_t* gen_backends_unix_filesystem_internal_path_from_fd(const int fd, char* const restrict out_path, gen_size_t out_path_bounds);
gen_error_t* gen_backends_unix_filesystem_internal_path_from_fd(const int fd, char* const restrict out_path, GEN_UNUSED gen_size_t out_path_bounds) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_internal_path_from_fd, GEN_FILE_NAME);
    if(error) return error;

    int result = fcntl(fd, F_GETPATH, out_path);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not get path from fd `%si`: %t", fd, gen_error_description_from_errno());

    return GEN_NULL;
}
