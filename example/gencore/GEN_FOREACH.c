// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>

int main(void) {
    char data[] = "yay";

    GEN_FOREACH(i, memb_a, 3, data) {
        glogf(INFO, "data[%zu] = %c", i, memb_a);
    }

    GEN_FOREACH_PTR(i, memb_b, 3, data) {
        glogf(INFO, "data[%zu] = %c (%p)", i, *memb_b, memb_b);
    }

    GEN_FOREACH_DIRECT_PTR(i, memb_c, 3, (char*) data) {
        glogf(INFO, "data[%zu] = %c (%p)", i, *memb_c, memb_c);
    }
}
