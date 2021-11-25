// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genproc.h
 * Utility for starting and managing subprocesses
 */

#ifndef GEN_PROC_H
#define GEN_PROC_H

#include "gencommon.h"

/**
 * Handle type for a process
 * @note Size and type may vary across systems
 */
typedef pid_t gen_process_t;

#ifndef GEN_PROC_READ_BUFFER_CHUNK
/**
 * The size of chunk to read at once while storing output in `gen_proc_get_output`
 * @note Increasing this if you have large amounts of output in subprocesses may improve performance
 */
#define GEN_PROC_READ_BUFFER_CHUNK 128
#endif

/**
 * Creates a new subprocess whose output is redirected to a `FILE*`
 * @param process_out pointer to storage for the `gen_process_t`
 * @param exec the command line to execute
 * @param redirect the stream to redirect output to
 * @return an error code
 */
GEN_ERRORABLE gen_proc_start_redirected(gen_process_t* const restrict process_out, const char* const restrict exec, FILE* const restrict redirect);
/**
 * Blocks the current thread until the specified process returns
 * Provides exit code after return
 * @param out_result pointer to storage for the exit code
 * @param process the process to wait on
 * @return an error code
 */
GEN_ERRORABLE gen_proc_wait(int* const restrict out_result, gen_process_t process);
/**
 * Blocks the current thread until the specified process returns
 * Provides exit code and console output after return
 * @param out_output pointer to storage for the output buffer heap pointer
 * @param out_result pointer to storage for the exit code
 * @param exec the command line to execute
 * @return an error code
 * @note `out_output` will be assigned a heap pointer which will need to be `gfree`'d
 */
GEN_ERRORABLE gen_proc_get_output(char** const restrict out_output, int* const restrict out_result, const char* const restrict exec);

#endif
