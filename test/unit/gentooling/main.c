// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include <gencommon.h>
#include <gentooling.h>

int main() {
    glog(INFO, "Testing GEN_FRAME_BEGIN()...");
    GEN_FRAME_BEGIN(main);

    GEN_REQUIRE_EQUAL_STRING("main", gen_tooling_call_stack.functions[gen_tooling_call_stack.next - 1]);
    GEN_REQUIRE_EQUAL_STRING("test/unit/gentooling/main.c", gen_tooling_call_stack.files[gen_tooling_call_stack.next - 1]);
    GEN_REQUIRE_EQUAL((uintptr_t) main, gen_tooling_call_stack.addresses[gen_tooling_call_stack.next - 1]);

    glog(INFO, "Testing gtrace...");
    gtrace;

    glog(INFO, "Testing gen_tooling_stack_pop()...");
}
