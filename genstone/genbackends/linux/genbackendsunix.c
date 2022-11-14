// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genfilesystem.h>

#include <genfilesystem_be.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <unistd.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

static void gen_internal_backends_unix_filesystem_internal_path_from_fd_cleanup_path(char** path) {
    if(!*path) return;

    error = gen_memory_free((void**) path);
	if(error) gen_error_abort_with_error(error, "genfilesystem");
}

gen_error_t* gen_backends_unix_filesystem_internal_path_from_fd(const int fd, char* const restrict out_path) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_internal_path_from_fd, GEN_FILE_NAME);
    if(error) return error;

    char* path = GEN_NULL;
    gen_size_t path_bounds = sizeof("/proc/self/fd/") + 10 /* 2147483647 (`INT_MAX`) is 10 characters long */;
    error = gen_memory_allocate_zeroed((void**) &path, path_bounds, sizeof(char));
    if(error) return error;

    GEN_CLEANUP_FUNCTION(gen_internal_backends_unix_filesystem_internal_path_from_fd_cleanup_path) GEN_UNUSED char* path_scope_variable = GEN_NULL;

    error = gen_string_format(path_bounds, path, GEN_NULL, "/proc/self/fd/%si", sizeof("/proc/self/fd/%si") - 1, fd);
    if(error) return error;

    int result = readlink(path, out_path, (gen_size_t) (value + 1));
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not get path from fd `%si`: %t", fd, gen_error_description_from_errno());

    return GEN_NULL;
}
