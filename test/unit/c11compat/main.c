// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include <gencommon.h>

static int ticks = 0;
static mtx_t ticks_mtx;

static bool child_started = false;

static int thread_func(void* pass) {
    child_started = true;

    for(size_t i = 0; i < 10; i++) {
        mtx_lock(&ticks_mtx);
        ticks++;
        mtx_unlock(&ticks_mtx);
    }

    return 0;
}

int main() {
    mtx_init(&ticks_mtx, mtx_plain);

    thrd_t thread;
    thrd_create(&thread, thread_func, NULL);

    while(!child_started)
        ;

    for(size_t i = 0; i < 10; i++) {
        mtx_lock(&ticks_mtx);
        ticks++;
        mtx_unlock(&ticks_mtx);
    }

    int result;
    thrd_join(thread, &result);

    GEN_REQUIRE_EQUAL(20, ticks);

    mtx_destroy(&ticks_mtx);
}
