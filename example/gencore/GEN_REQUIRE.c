// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>

int main(void) {
    GEN_REQUIRE_EQUAL(2, 2);
    
    void* a; gzalloc(a, 1, 1);
    void* b; gzalloc(b, 1, 1);
    GEN_REQUIRE_EQUAL_MEMREGION(a, b, 1);

    GEN_REQUIRE_EQUAL_STRING("foo", "foo");

    GEN_REQUIRE_NO_ERROR(GEN_OK);
    
    GEN_REQUIRE_NON_NULL("bar");

    GEN_REQUIRE_NO_REACH; // This will abort
}
