// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>

int main(void) {
    const char* const description = gen_error_name(GEN_NO_SUCH_OBJECT);
    GEN_REQUIRE_EQUAL_STRING("GEN_NO_SUCH_OBJECT", GEN_NO_SUCH_OBJECT);
}
