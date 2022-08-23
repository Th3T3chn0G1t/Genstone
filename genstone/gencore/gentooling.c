// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"
#include "include/genthreads.h"

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
	const void* addresses[GEN_TOOLING_DEPTH];

	/**
     * A buffer of source files for functions.
     */
	const char* files[GEN_TOOLING_DEPTH];
} gen_tooling_stack_t;

static GEN_THREAD_LOCAL gen_tooling_stack_t call_stack = {0};

void gen_tooling_internal_auto_cleanup(GEN_UNUSED const void* const restrict p) {
	gen_error_t error = gen_tooling_pop();
	if(error.type) {
		gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
		gen_error_abort();
	}
}

gen_error_t gen_tooling_push(const char* const restrict frame, const void* const restrict address, const char* const restrict file) {
	if(call_stack.next >= GEN_TOOLING_DEPTH) return gen_error_attach_backtrace(GEN_ERROR_OUT_OF_SPACE, GEN_LINE_NUMBER, "Tooling stack is full");
	if(!frame) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`frame` was NULL");
	if(!address) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`address` was NULL");
	if(!file) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`file` was NULL");

	call_stack.functions[call_stack.next] = frame;
	call_stack.addresses[call_stack.next] = address;
	call_stack.files[call_stack.next] = file;
	++call_stack.next;

	return (gen_error_t){GEN_OK};
}

gen_error_t gen_tooling_pop(void) {
	if(call_stack.next == 0) return gen_error_attach_backtrace(GEN_ERROR_BAD_OPERATION, GEN_LINE_NUMBER, "Tooling stack is empty but tried to pop");

	--call_stack.next;

	return (gen_error_t){GEN_OK};
}
