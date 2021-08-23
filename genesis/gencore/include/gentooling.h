// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @file gentooling.h
 * Provides utilities for tooling and profiling
 */

#include "gencommon.h"

#ifndef GEN_TOOLING_DEPTH
/**
 * The maximum depth of a tooled call stack
 */
#define GEN_TOOLING_DEPTH 64
#endif

/**
 * A tooled call stack
 */
typedef struct {
    /**
     * Offset of the next free space in the stack
     */
    size_t next;
    /**
     * A buffer function names
     */
    const char* functions[GEN_TOOLING_DEPTH];
    /**
     * A buffer function addresses
     */
    uintptr_t addresses[GEN_TOOLING_DEPTH]; 
    /**
     * A buffer source files for functions
     */
    const char* files[GEN_TOOLING_DEPTH];
} gen_tooling_stack_t;

/**
 * Handler for call stack push events
 */
typedef void (*gen_tooling_stack_push_handler_t) (void);
/**
 * Handler for call stack pop events
 */
typedef void (*gen_tooling_stack_pop_handler_t) (void);

/**
 * The global tooled call stack
 */
extern gen_tooling_stack_t gen_tooling_call_stack;

/**
 * The global pre-push handler for the tooled call stack
 */
extern gen_tooling_stack_push_handler_t gen_tooling_push_handler;

/**
 * The global pre-pop handler for the tooled call stack
 */
extern gen_tooling_stack_pop_handler_t gen_tooling_pop_handler;

/**
 * Pushes a new entry onto the tooled call stack
 * @param frame the name of the frame
 * @param address the address of the frame
 * @param file the source file containing the frame
 */
extern void gen_tooling_stack_push(const char* restrict frame, const uintptr_t address, const char* restrict file);
/**
 * Pops the last frame from the tooled call stack
 */
extern void gen_tooling_stack_pop(void);

/**
 * cleanup attribute wrapper for gen_tooling_stack_pop()
 * @see GEN_FRAME_BEGIN
 */
extern void gen_internal_tooling_frame_scope_end(const char* const restrict passthrough);

/**
 * Begins a tooled frame with an automatic lifetime
 * @param func the address of this frame
 * @note for non-function frames use gen_tooling_stack_push directly
 */
#define GEN_FRAME_BEGIN(func) \
    __attribute__((cleanup (gen_internal_tooling_frame_scope_end))) __unused const char __frame_scope_tmpvar; \
    gen_tooling_stack_push(__func__, (uintptr_t) func, __FILE__)

/**
 * Backtrace logging prefix
 */
#define GEN_LOGGER_TRACE_PREFIX GEN_ANSI_COLOR_LIGHT(GEN_ANSI_GRAY) GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Trace: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)

/**
 * Outputs backtrace information
 */
#define gtrace GEN_FOREACH(i, trace, gen_tooling_call_stack.next, gen_tooling_call_stack.functions) printf("%s%p %s() %s\n", GEN_LOGGER_TRACE_PREFIX, (void*) gen_tooling_call_stack.addresses[i], trace, gen_tooling_call_stack.files[i])
