// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gentooling.h
 * Provides utilities for tooling and profiling.
 */

#ifndef GEN_TOOLING_H
#define GEN_TOOLING_H

#include "gentoolingframe.h"
#include "gencommon.h"

extern void gen_tooling_internal_auto_cleanup(GEN_UNUSED const void* const restrict p);

/**
 * Attaches the current tooling frame to a variable's lifetime.
 * When the prefixed variable goes out of scope, the call frame is destroyed.
 */
#define GEN_TOOLING_AUTO GEN_CLEANUP_FUNCTION(gen_tooling_internal_auto_cleanup)

/**
 * Pushes a new entry onto the tooled call stack.
 * @param[in] frame The name of the frame.
 * @param[in] address The address of the frame.
 * @param[in] file The name of the source file containing the frame.
 * @return An error code.
 */
extern gen_error_t gen_tooling_push(const char* const restrict frame, const void* const restrict address, const char* const restrict file);

/**
 * Pops the last frame from the tooled call stack.
 * @return An error code.
 */
extern gen_error_t gen_tooling_pop(void);

/**
 * Gets a backtrace of the current tooled call stack.
 * @param[out] out_backtrace A pointer to storage for the backtrace.
 * @param[out] out_length A pointer to storage for length of the backtrace.
 * @return An error code.
 */
extern gen_error_t gen_tooling_get_backtrace(gen_tooling_frame_t* const restrict out_backtrace, size_t* const restrict out_length);

#endif
