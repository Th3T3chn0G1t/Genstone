// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genproc.h
 * Utility for starting and managing subprocesses.
 */

#ifndef GEN_PROC_H
#define GEN_PROC_H

#include "gencommon.h"
#include "genfs.h"

/**
 * Handle type for a process.
 * @note Size and type may vary across systems.
 */
typedef pid_t gen_process_t;

/**
 * @example{lineno} example/gencore/gen_proc.c
 * Example for how to use the `gen_proc*` family of functions.
 * The `gen_proc*` family of functions is used for managing subprocesses in a platform agnostic way.
 */


/**
 * Creates a pair of filesystem object handles which back eachother - a pipe.
 * This is intended to be used as a redirect for subprocess output.
 * Writing to the "write" handle will make data available for reading in the "read" handle etc.
 * These handles are *not* suitable for regular genfs IO and will cause errors.
 * @param[out] output_handle_read pointer to storage for the read handle.
 * @param[out] output_handle_write pointer to storage for the write handle.
 * @return an error code.
 * @see gen_proc_create_redirected_to
 */
GEN_ERRORABLE gen_handle_create_proc_redirect_target(gen_filesystem_handle_t* restrict output_handle_read, gen_filesystem_handle_t* restrict output_handle_write);

/**
 * Creates a subprocess and redirects output to a given redirect handle.
 * @param[out] out_process pointer to storage for the created process.
 * @param[in] exec the command line to start the process. This goes through the shell so caution should be taken regarding arbitrary shell execution.
 * @param[in] redirect the handle to redirect output to. If created with `gen_handle_create_proc_redirect_target` should be the "write" handle. Pass `gen_stdout_handle` to get regular output.
 * @return an error code. 
 * @see gen_handle_create_proc_redirect_target
 */
GEN_ERRORABLE gen_proc_create_redirected_to(gen_process_t* const restrict out_process, const char* const restrict exec, const gen_filesystem_handle_t* const restrict redirect);

/**
 * Blocks the current thread until the specified subprocess exits.
 * @param process the subprocess to wait for.
 * @param out_exitcode pointer to storage for the exit code of the subprocess.
 * @return an error code.
 */
GEN_ERRORABLE gen_proc_wait(const gen_process_t* const restrict process, int* const restrict out_exitcode);

/**
 * Attempts to kill a subprocess.
 * @param process the subprocess to kill.
 * @return an error code.
 */
GEN_ERRORABLE gen_proc_kill(const gen_process_t* const restrict process);

#endif
