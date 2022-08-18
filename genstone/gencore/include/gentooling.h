// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gentooling.h
 * Provides utilities for tooling and profiling.
 */
#ifndef GEN_TOOLING_H
#define GEN_TOOLING_H

#include "gencommon.h"

/**
 * Pushes a new entry onto the tooled call stack.
 * @param[in] frame The name of the frame.
 * @param[in] address The address of the frame.
 * @param[in] file The name of the source file containing the frame.
 */
extern gen_error_t gen_tooling_push(const char* const restrict frame, const void* const restrict address, const char* const restrict file);

/**
 * Pops the last frame from the tooled call stack.
 */
extern void gen_tooling_pop(void);

/**
 * Attaches the current tooling frame to a variable's lifetime.
 * When the prefixed variable goes out of scope, the call frame is destroyed.
 */
#define GEN_TOOLING_AUTO __attribute__((cleanup(gen_internal_tooling_auto_pop)))
extern void gen_internal_tooling_auto_pop(const void* const restrict p);

/**
 * Gets the length of a bactrace of the current tooled call stack.
 * @return The length of a bactrace of the current tooled call stack.
 */
extern size_t gen_tooling_get_bactrace_size(void);

/**
 * Gets a backtrace of the current tooled call stack.
 * @param[out] out_backtrace A pointer to storage for the backtrace.
 * @param[in] buffer_size The size of the `out_backtrace` buffer.
 */
extern void gen_tooling_get_backtrace(char* const restrict out_backtrace, const size_t buffer_size);

#endif
