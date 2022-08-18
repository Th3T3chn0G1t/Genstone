// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"

#ifndef GEN_TOOLING_DEPTH
/**
 * The maximum depth of a tooled call stack.
 */
#define GEN_TOOLING_DEPTH 64
#endif

/**
 * A tooled call stack.
 */
typedef struct {
	/**
     * Offset of the next free space in the stack.
     */
	size_t next;
	/**
     * A buffer of function names.
     */
	const char* functions[GEN_TOOLING_DEPTH];
	/**
     * A buffer of function addresses.
     */
	uintptr_t addresses[GEN_TOOLING_DEPTH];
	/**
     * A buffer of source files for functions.
     */
	const char* files[GEN_TOOLING_DEPTH];
} gen_tooling_stack_t;

thread_local gen_tooling_stack_t gen_tooling_call_stack = {0, {0}, {0}, {0}};

void gen_tooling_stack_push(const char* const restrict frame, const uintptr_t address, const char* const restrict file) {
	if(gen_tooling_call_stack.next >= GEN_TOOLING_DEPTH) GEN_FATAL_ERROR(GEN_OUT_OF_SPACE, "Tooling stack is full. Increase `GEN_TOOLING_DEPTH` if this was legitimately reached");
	if(!frame) GEN_FATAL_ERROR(GEN_INVALID_PARAMETER, "`frame` was NULL");
	if(!address) GEN_FATAL_ERROR(GEN_INVALID_PARAMETER, "`address` was NULL");
	if(!file) GEN_FATAL_ERROR(GEN_INVALID_PARAMETER, "`file` was NULL");

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

void gen_internal_tooling_frame_scope_end(__unused const char* const restrict passthrough) {
	//	if(*passthrough != '\0')

	gen_tooling_stack_pop();
}

void gen_tooling_freq_profile_ping(const char* const restrict name) {
	if(gen_tooling_freq_profile_next > GEN_FREQ_PROFILE_MAX) GEN_FATAL_ERROR(GEN_OUT_OF_SPACE, "Frequency profile buffer is full. Increase `GEN_FREQ_PROFILE_MAX` if this was legitimately reached");
	if(!name) GEN_FATAL_ERROR(GEN_INVALID_PARAMETER, "`name` was NULL");

	GEN_FOREACH_PTR(i, profile, gen_tooling_freq_profile_next, gen_tooling_freq_profiles) {
		if(profile->name == name) {
			struct timeval current_time;
			gettimeofday(&current_time, NULL);

			struct timeval delta;
			gen_timeval_sub(&current_time, &profile->last, &delta);
			gen_timeval_add(&profile->running, &delta, &profile->running);
			profile->last.tv_sec = current_time.tv_sec;
			profile->last.tv_usec = current_time.tv_usec;
			++profile->calls_length;

			return;
		}
	}

	if(gen_tooling_freq_profile_next >= GEN_FREQ_PROFILE_MAX) GEN_FATAL_ERROR(GEN_OUT_OF_SPACE, "Frequency profile buffer is full. Increase `GEN_FREQ_PROFILE_MAX` if this was legitimately reached");

	gen_tooling_freq_profile_t* const new_profile = &gen_tooling_freq_profiles[gen_tooling_freq_profile_next++];

	new_profile->name = name;

	struct timeval current_time;
	gettimeofday(&current_time, NULL);

	new_profile->first.tv_sec = current_time.tv_sec;
	new_profile->first.tv_usec = current_time.tv_usec;
	new_profile->last.tv_sec = current_time.tv_sec;
	new_profile->last.tv_usec = current_time.tv_usec;
	++new_profile->calls_length;
}

void gen_tooling_print_backtrace(void) {
	if(gen_tooling_call_stack.next > GEN_TOOLING_DEPTH) GEN_FATAL_ERROR(GEN_OUT_OF_SPACE, "Tooling stack is full. Increase `GEN_TOOLING_DEPTH` if this was legitimately reached");

	GEN_FOREACH(i, trace, gen_tooling_call_stack.next, gen_tooling_call_stack.functions) {
		(void) trace;
		glogf(TRACE, "frame #%zu: 0x%p %s() %s", i, (void*) gen_tooling_call_stack.addresses[gen_tooling_call_stack.next - (i + 1)], gen_tooling_call_stack.functions[gen_tooling_call_stack.next - (i + 1)], gen_tooling_call_stack.files[gen_tooling_call_stack.next - (i + 1)]);
	}
}
