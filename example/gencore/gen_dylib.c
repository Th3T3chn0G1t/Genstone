// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <gendl.h>

int main(void) {
    gen_dylib_t handle;

    gen_error_t error = gen_dylib_load(&handle, "mysharedobject");
    GEN_REQUIRE_NO_ERROR(error);

    void* foo;
    error = gen_dylib_symbol(&foo, &handle, "foo");
    GEN_REQUIRE_NO_ERROR(error);

    ((void(*)(void)) foo)();

    error = gen_dylib_unload(&handle);
    GEN_REQUIRE_NO_ERROR(error);
}
