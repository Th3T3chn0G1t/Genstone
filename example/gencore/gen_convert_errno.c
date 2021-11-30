// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>

int main(void) {
    gen_error_t error = gen_convert_errno(ENOENT);
    GEN_REQUIRE_EQUAL(GEN_NO_SUCH_OBJECT, error); // ENOENT maps to GEN_NO_SUCH_OBJECT
}
