// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include <gencommon.h>
#include <gentooling.h>

static void tooling_test(void) {
    glog(INFO, "Testing GEN_FRAME_BEGIN()...");
    GEN_FRAME_BEGIN(tooling_test);

    GEN_REQUIRE_EQUAL_STRING("tooling_test", gen_tooling_call_stack.functions[gen_tooling_call_stack.next - 1]);
    GEN_REQUIRE_EQUAL_STRING("test/unit/gentooling/main.c", gen_tooling_call_stack.files[gen_tooling_call_stack.next - 1]);
    GEN_REQUIRE_EQUAL((uintptr_t) tooling_test, gen_tooling_call_stack.addresses[gen_tooling_call_stack.next - 1]);

    glog(INFO, "Testing gtrace...");
    gtrace;

    glog(INFO, "Testing gen_tooling_stack_pop()...");
}

static void freq_prof_test(void) {
    GEN_FREQ_PROFILE;
}

int main() {
    tooling_test();

    for(size_t i = 0; i < 1000000; i++)
        freq_prof_test();

    const gen_tooling_freq_profile_t* const profile = &gen_tooling_freq_profiles[gen_tooling_freq_profile_next - 1];

    const long double total = (long double) profile->running.tv_usec + ((long double) profile->running.tv_sec * (long double) GEN_INTERNAL_USECONDS_PER_SECOND);
    const long double average = total / profile->n_calls;
    glogf(PERFORMANCE, "%s: av. %Lf ms tot. %Lf ms", profile->name, average / 1000.0L, total / 1000.0L);
}
