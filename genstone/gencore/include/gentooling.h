// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gentooling.h
 * Provides utilities for tooling and profiling.
 */
#ifndef GEN_TOOLING_H
#define GEN_TOOLING_H

#include "genutil.h"

#ifndef GEN_TOOLING_DEPTH
/**
 * The maximum depth of a tooled call stack.
 */
#define GEN_TOOLING_DEPTH 64
#endif

#ifndef GEN_FREQ_PROFILE_MAX
/**
 * The maximum number of frequency profilers.
 */
#define GEN_FREQ_PROFILE_MAX 64
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

/**
 * A frequency profile.
 */
typedef struct {
	/**
     * The name of the profile.
     */
	const char* name;
	/**
     * The number of calls the profile has had.
     */
	uintmax_t n_calls;
	/**
     * The running total time for all calls.
     */
	struct timeval running;
	/**
     * The time of the first call.
     */
	struct timeval first;
	/**
     * The time of the last call.
     */
	struct timeval last;
} gen_tooling_freq_profile_t;

/**
 * Handler for call stack push events.
 */
typedef void (*gen_tooling_stack_push_handler_t)(void);
/**
 * Handler for call stack pop events.
 */
typedef void (*gen_tooling_stack_pop_handler_t)(void);

/**
 * The global tooled call stack.
 */
extern thread_local gen_tooling_stack_t gen_tooling_call_stack;

/**
 * The global frequency profile pool.
 */
extern thread_local gen_tooling_freq_profile_t gen_tooling_freq_profiles[GEN_FREQ_PROFILE_MAX];

/**
 * The offset of the next free space in the global frequency profile pool.
 * @see gen_tooling_freq_profiles
 */
extern thread_local size_t gen_tooling_freq_profile_next;

/**
 * The global pre-push handler for the tooled call stack.
 */
extern thread_local gen_tooling_stack_push_handler_t gen_tooling_push_handler;

/**
 * The global pre-pop handler for the tooled call stack.
 */
extern thread_local gen_tooling_stack_pop_handler_t gen_tooling_pop_handler;

/**
 * Pushes a new entry onto the tooled call stack.
 * @param[in] frame the name of the frame.
 * @param[in] address the address of the frame.
 * @param[in] file the source file containing the frame.
 */
extern void gen_tooling_stack_push(const char* const restrict frame, const uintptr_t address, const char* const restrict file);
/**
 * Pops the last frame from the tooled call stack.
 */
extern void gen_tooling_stack_pop(void);

/**
 * Cleanup attribute wrapper for `gen_tooling_stack_pop`.
 * @see GEN_FRAME_END
 */
extern void gen_internal_tooling_frame_scope_end(const char* const restrict passthrough);

/**
 * Updates or adds a frequency profile.
 */
extern void gen_tooling_freq_profile_ping(const char* const restrict name);

#pragma clang diagnostic ignored "-Wreserved-id-macro"
/**
 * Begins a tooled frame with an automatic lifetime.
 * @param[in] func the address of this frame.
 * @note For non-function frames use `gen_tooling_stack_push` directly.
 */
#define GEN_FRAME_BEGIN(func) \
	errno = EOK; /* This is *very* naughty but it really screws us up if we don't do it due to errno being taken as a condition basically everywhere */ \
	__attribute__((cleanup(gen_internal_tooling_frame_scope_end))) __unused const char gen_internal_frame_scope_tmpvar = '\0'; \
	GEN_REQUIRE_EQUAL_STRING(#func, __func__); \
	gen_tooling_stack_push(__func__, (uintptr_t) func, __FILE__)
/**
 * @example{lineno} example/gencore/GEN_FRAME_BEGIN.c
 * Example for how to use `GEN_FRAME_BEGIN`.
 * `GEN_FRAME_BEGIN` is used for tooling functions for nice backtraces and possibly performance introspection.
 * `GEN_FRAME_BEGIN` is intended as a Genstone internal utility primarily, but is part of the public API if a program decides to use it.
 */

/**
 * Frequency profiling for calls to the current function.
 */
#define GEN_FREQ_PROFILE gen_tooling_freq_profile_ping(__func__) /* `__func__` is fine to compare against numerically without being constant */
/**
 * @example{lineno} example/gencore/GEN_FREQ_PROFILE.c
 * Example for how to use `GEN_FREQ_PROFILE` and `GEN_NAMED_FREQ_PROFILE`.
 * `GEN_FREQ_PROFILE` and `GEN_NAMED_FREQ_PROFILE` are used for tooling functions for performance introspection.
 * `GEN_FREQ_PROFILE` and `GEN_NAMED_FREQ_PROFILE` are intended as a Genstone internal utility primarily, but is part of the public API if a program decides to use it.
 */

/**
 * Frequency profiling for the call site.
 */
#define GEN_NAMED_FREQ_PROFILE(name) \
	do { \
		if(!__builtin_constant_p(name)) glogf(WARNING, "Non-constant frequency profile name %s (%s)", #name, name); \
		gen_tooling_freq_profile_ping(__func__); \
	} while(0)

extern void gen_tooling_print_backtrace(void);

/**
 * Outputs backtrace information.
 */
#define gtrace gen_tooling_print_backtrace()
/**
 * @example{lineno} example/gencore/gtrace.c
 * Example for how to use `gtrace`.
 * `gtrace` is used for getting backtraces in error handling.
 */

#endif
