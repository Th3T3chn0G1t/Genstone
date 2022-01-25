// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <gentooling.h>

static void tooling_test(void) {
	glog(INFO, "Testing GEN_FRAME_BEGIN()...");
	GEN_FRAME_BEGIN(tooling_test);

	GEN_REQUIRE_EQUAL_STRING("tooling_test", gen_tooling_call_stack.functions[gen_tooling_call_stack.next - 1]);
	GEN_REQUIRE_EQUAL_STRING("test/unit/gentooling/main.c", gen_tooling_call_stack.files[gen_tooling_call_stack.next - 1]);
	GEN_REQUIRE_EQUAL((uintptr_t) /* This is safe for all currently supported architectures */ tooling_test, gen_tooling_call_stack.addresses[gen_tooling_call_stack.next - 1]);

	glog(INFO, "Testing gtrace...");
	gtrace;

	glog(INFO, "Testing gen_tooling_stack_pop()...");
}

static void freq_prof_test(void) {
	GEN_FRAME_BEGIN(freq_prof_test);

	GEN_FREQ_PROFILE;
}

int main(void) {
	GEN_FRAME_BEGIN(main);

	tooling_test();

	glog(INFO, "Testing GEN_FREQ_PROFILE...");
	for(size_t i = 0; i < 1000000; i++)
		freq_prof_test();

	const gen_tooling_freq_profile_t* const profile = &gen_tooling_freq_profiles[gen_tooling_freq_profile_next - 1];

	// const long double total = GEN_TIMEVAL_AS_SECONDS(profile->running) / (long double) GEN_MILLISECONDS_PER_SECOND;
	// const long double average = (total / profile->calls_length) / (long double) GEN_MILLISECONDS_PER_SECOND;
	// glogf(PERFORMANCE, "%s: av. %Lf ms tot. %Lf ms", profile->name, average, total);

	const double total = (double) GEN_TIMEVAL_AS_SECONDS(profile->running) / (double) GEN_MILLISECONDS_PER_SECOND;
	const double average = (double) (total / (double) profile->calls_length) / (double) GEN_MILLISECONDS_PER_SECOND;
	glogf(PERFORMANCE, "%s: av. %lf ms tot. %lf ms", profile->name, average, total);
}
