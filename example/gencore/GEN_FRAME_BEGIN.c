// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>

static void buzz(void) {
    GEN_FRAME_BEGIN(buzz);

    /*
     * Should give the following backtrace:
     *  - `main`
     *  - `foo`
     *  - `bar`
     *  - `fizz`
     *  - `buzz`
     */
    gtrace;
}

static void fizz(void) {
    GEN_FRAME_BEGIN(fizz);

    buzz();
}

static void bar(void) {
    GEN_FRAME_BEGIN(bar);

    fizz();
}

static void foo(void) {
    GEN_FRAME_BEGIN(foo);

    bar();
}

int main(void) {
    GEN_FRAME_BEGIN(main);

    foo();
}
