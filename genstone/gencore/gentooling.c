// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"

gen_tooling_stack_t gen_tooling_call_stack = {0, {0}, {0}, {0}};
gen_tooling_freq_profile_t gen_tooling_freq_profiles[GEN_FREQ_PROFILE_MAX] = {0};
size_t gen_tooling_freq_profile_next = 0;

gen_tooling_stack_push_handler_t gen_tooling_push_handler = NULL;
gen_tooling_stack_pop_handler_t gen_tooling_pop_handler = NULL;

void gen_tooling_stack_push(const char* const restrict frame, const uintptr_t address, const char* const restrict file) {
	if(gen_tooling_call_stack.next == GEN_TOOLING_DEPTH) GEN_FATAL_ERROR(GEN_OUT_OF_SPACE, "Tooling stack is full. Increase `GEN_TOOLING_DEPTH` if this was legitimately reached");
	
	if(gen_tooling_push_handler) gen_tooling_push_handler();
	gen_tooling_call_stack.functions[gen_tooling_call_stack.next] = frame;
	gen_tooling_call_stack.addresses[gen_tooling_call_stack.next] = address;
	gen_tooling_call_stack.files[gen_tooling_call_stack.next] = file;
	++gen_tooling_call_stack.next;
}

void gen_tooling_stack_pop(void) {
	if(gen_tooling_call_stack.next == 0) GEN_FATAL_ERROR(GEN_BAD_OPERATION, "Tooling stack is empty but tried to pop");

	if(gen_tooling_pop_handler) gen_tooling_pop_handler();
	--gen_tooling_call_stack.next;
}

void gen_internal_tooling_frame_scope_end(const char* const restrict passthrough) {
//	if(*passthrough != '\0')

	gen_tooling_stack_pop();
}

void gen_tooling_freq_profile_ping(const char* const restrict name) {
	GEN_FOREACH_PTR(i, profile, gen_tooling_freq_profile_next, gen_tooling_freq_profiles) {
		if(profile->name == name) {
			struct timeval current_time;
			gettimeofday(&current_time, NULL);

			struct timeval delta;
			gen_timeval_sub(&current_time, &profile->last, &delta);
			gen_timeval_add(&profile->running, &delta, &profile->running);
			profile->last.tv_sec = current_time.tv_sec;
			profile->last.tv_usec = current_time.tv_usec;
			++profile->n_calls;

			return;
		}
	}

	gen_tooling_freq_profile_t* const new_profile = &gen_tooling_freq_profiles[gen_tooling_freq_profile_next++];

	new_profile->name = name;

	struct timeval current_time;
	gettimeofday(&current_time, NULL);

	new_profile->first.tv_sec = current_time.tv_sec;
	new_profile->first.tv_usec = current_time.tv_usec;
	new_profile->last.tv_sec = current_time.tv_sec;
	new_profile->last.tv_usec = current_time.tv_usec;
	++new_profile->n_calls;
}

void gen_tooling_print_backtrace(void) {
	GEN_FOREACH(i, trace, gen_tooling_call_stack.next, gen_tooling_call_stack.functions)
		glogf(TRACE, "%zu. %p %s() %s", i, (void*) gen_tooling_call_stack.addresses[i], trace, gen_tooling_call_stack.files[i]);
}
