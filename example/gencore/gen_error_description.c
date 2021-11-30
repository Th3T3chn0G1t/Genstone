// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>

int main(void) {
    const char* const description = gen_error_description(GEN_NO_SUCH_OBJECT);
    GEN_REQUIRE_EQUAL_STRING("A nonexistent object was referenced", description); // GEN_NO_SUCH_OBJECT maps to "A nonexistent object was referenced"
}
