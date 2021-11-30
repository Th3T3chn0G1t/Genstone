// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>

static void foo(void) {
	GEN_FREQ_PROFILE;
	/**
	 * or use GEN_NAMED_FREQ_PROFILE("whatever")
	 */
}

int main(void) {
    foo();

	glog(INFO, "Testing GEN_FREQ_PROFILE...");
	for(size_t i = 0; i < 1000000; i++)
		freq_prof_test();

	const gen_tooling_freq_profile_t* const profile = &gen_tooling_freq_profiles[gen_tooling_freq_profile_next - 1];

	const double total = (double) GEN_TIMEVAL_AS_SECONDS(profile->running) / (double) GEN_MILLISECONDS_PER_SECOND;
	const double average = (double) (total / (double) profile->n_calls) / (double) GEN_MILLISECONDS_PER_SECOND;

	glogf(PERFORMANCE, "%s: av. %lf ms tot. %lf ms", profile->name, average, total);
}
