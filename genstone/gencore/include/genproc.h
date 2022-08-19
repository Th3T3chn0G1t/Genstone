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
 * Creates a subprocess and redirects output to a given redirect handle.
 * @param[out] out_process pointer to storage for the created process.
 * @param[in] exec the command line to start the process. This goes through the shell so caution should be taken regarding arbitrary shell execution.
 * @param[in] redirect the handle to redirect output to.
 * @return An error code. 
 */
extern gen_error_t gen_proc_create_redirected_to(gen_process_t* const restrict out_process, const char* const restrict exec, const gen_filesystem_handle_t* const restrict redirect);

/**
 * Blocks the current thread until the specified subprocess exits.
 * @param process the subprocess to wait for.
 * @param out_exitcode pointer to storage for the exit code of the subprocess.
 * @return An error code.
 */
extern gen_error_t gen_proc_wait(const gen_process_t* const restrict process, int* const restrict out_exitcode);

/**
 * Attempts to kill a subprocess.
 * @param process the subprocess to kill.
 * @return An error code.
 */
extern gen_error_t gen_proc_kill(const gen_process_t* const restrict process);

#endif
