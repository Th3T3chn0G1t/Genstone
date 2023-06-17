// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"

typedef struct {
	gen_size_t next;
	const char* functions[GEN_TOOLING_DEPTH];
	const void* addresses[GEN_TOOLING_DEPTH];
	const char* files[GEN_TOOLING_DEPTH];
} gen_tooling_stack_t;

static GEN_THREAD_LOCAL gen_tooling_stack_t call_stack = {0};

void gen_tooling_internal_auto_cleanup(
        GEN_UNUSED const void* const restrict p) {

	gen_tooling_pop();
}

void gen_tooling_push(
        const char* const restrict frame,const char* const restrict file) {

	if(call_stack.next >= GEN_TOOLING_DEPTH) gen_error_abort();

	call_stack.functions[call_stack.next] = frame;
	call_stack.addresses[call_stack.next] = __builtin_return_address(0);
	call_stack.files[call_stack.next] = file;
	++call_stack.next;
}

void gen_tooling_pop(void) {
	if(call_stack.next == 0)  gen_error_abort();

	--call_stack.next;
}

void gen_tooling_get_backtrace(
        gen_tooling_frame_t* const restrict out_backtrace,
        gen_size_t* const restrict out_length) {

	if(out_length) *out_length = call_stack.next;

	if(out_backtrace) {
		for(gen_size_t i = 0; i < call_stack.next; ++i) {
			out_backtrace[i] =
                    (gen_tooling_frame_t) {
                        call_stack.functions[i], call_stack.addresses[i],
                        call_stack.files[i]};
		}
	}
}
