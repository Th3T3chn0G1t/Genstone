// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

/**
 * @file genprocess.h
 * Utility for starting and managing processes.
 */

#ifndef GEN_PROC_H
#define GEN_PROC_H

#include "gencommon.h"
#include "genfilesystem.h"

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
/**
 * Handle type for a process.
 */
typedef int gen_process_t;
#endif

/**
 * Creates a process redirected to a filesystem handle.
 * @param[in] executable_path The path to the executable to create the process with. May be taken as a suffix to `PATH`.
 * @param[in] executable_path_length The length of the path to the executable to create the process with.
 * @param[in] arguments The arguments to pass to the created process.
 * @param[in] argument_lengths The length of each argument to pass to the created process.
 * @param[in] arguments_length The length of the list of arguments to pass to the created process.
 * @param[in] environment The list of environment variables to pass to the created process.
 * @param[in] environment_lengths The length of each environment variable to pass to the created process.
 * @param[in] environment_length The length of the list of environment variables to pass to the created process.
 * @param[in,out] filesystem_handle The filesystem handle to redirect output to.
 * @param[out] out_process A pointer to storage for a handle to the created process.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_process_create_with_redirect(const char* const restrict executable_path, const gen_size_t executable_path_length, const char* const* const restrict arguments, const gen_size_t* const restrict argument_lengths, const gen_size_t arguments_length, const char* const restrict * const restrict environment, const gen_size_t* const restrict environment_lengths, const gen_size_t environment_length, gen_filesystem_handle_t* const restrict filesystem_handle, gen_process_t* const restrict out_process);

/**
 * Blocks the current thread until the specified process exits.
 * @param[in] process The process to wait for.
 * @param[out] out_exitcode A pointer to storage for the exit code of the process.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_process_wait(const gen_process_t* const restrict process, int* const restrict out_exitcode);

/**
 * Attempts to kill a process.
 * @param[in] process The process to kill.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_process_kill(const gen_process_t* const restrict process);

/**
 * Checks if a process is alive.
 * @param[in] process The process to check.
 * @param[out] out_alive A pointer to storage for the result of whether the process is alive.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_process_check(const gen_process_t* const restrict process, gen_bool_t* const restrict out_alive);

/**
 * Gets a pointer to the current process' environment.
 * @param[out] out_environment A pointer to a pointer to a buffer of pointers to environment variables.
 * @param[out] out_length A pointer to storage for the length of the environment.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_process_get_environment(char*** const restrict out_environment, gen_size_t* const restrict out_length);

#endif
