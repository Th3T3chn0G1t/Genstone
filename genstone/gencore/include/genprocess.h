// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

/**
 * @file genprocess.h
 * Utility for starting and managing subprocesses.
 */

// TODO: Implement genprocess

#ifndef GEN_PROC_H
#define GEN_PROC_H

#include "gencommon.h"
#include "genfilesystem.h"

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <unistd.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

/**
 * Handle type for a process.
 */
typedef pid_t gen_process_t;

/**
 * Creates a subprocess.
 * @param[in] executable_path The path to the executable to create the subprocess with. 
 * @param[in] executable_path_length The length of the path to the executable to create the subprocess with. 
 * @param[in] arguments The arguments to pass to the created subprocess.
 * @param[in] argument_lengths The lengths of the arguments to pass to the created subprocess.
 * @param[in] arguments_length The length of the `arguments` buffer.
 * @param[out] out_process A pointer to storage for a handle to the created subprocess.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_process_create(const char* const restrict executable_path, const size_t executable_path_length, const char* const restrict* const restrict arguments, const size_t* const restrict argument_lengths, const size_t arguments_length, gen_process_t* const restrict out_process);

/**
 * Creates a subprocess redirected to a filesystem handle.
 * @param[in] executable_path The path to the executable to create the subprocess with. 
 * @param[in] executable_path_length The length of the path to the executable to create the subprocess with. 
 * @param[in] arguments The arguments to pass to the created subprocess.
 * @param[in] argument_lengths The lengths of the arguments to pass to the created subprocess.
 * @param[in] arguments_length The length of the `arguments` buffer.
 * @param[in,out] filesystem_handle The filesystem handle to redirect output to.
 * @param[out] out_process A pointer to storage for a handle to the created subprocess.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_process_create_with_redirect(const char* const restrict executable_path, const size_t executable_path_length, const char* const restrict* const restrict arguments, const size_t* const restrict argument_lengths, const size_t arguments_length, gen_filesystem_handle_t* const restrict filesystem_handle, gen_process_t* const restrict out_process);

/**
 * Blocks the current thread until the specified subprocess exits.
 * @param process The subprocess to wait for.
 * @param out_exitcode A pointer to storage for the exit code of the subprocess.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_process_wait(const gen_process_t* const restrict process, int* const restrict out_exitcode);

/**
 * Attempts to kill a subprocess.
 * @param process The subprocess to kill.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_process_kill(const gen_process_t* const restrict process);

#endif
