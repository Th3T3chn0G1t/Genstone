// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_UNIT "tests-test!"
#include <gentests.h>

static gen_error_t* gen_main(void) {
    GEN_TESTS_EXPECT(1, 2);

    return GEN_NULL;
}
