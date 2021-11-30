// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genlocale.h>

int main(void) {
    size_t size;
    gen_error_t error = gen_locale_convert_currency(NULL, 0, &size, 3, 40);
    GEN_REQUIRE_NO_ERROR(error);

    char string[size];
    error = gen_locale_convert_currency(string, size, NULL, 3, 40);
    GEN_REQUIRE_NO_ERROR(error);

    glogf(INFO, "3 units 40 cents -> %s", string);
}
