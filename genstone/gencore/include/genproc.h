// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genproc.h
 * Utility for starting and managing subprocesses
 */

#ifndef GEN_PROC_H
#define GEN_PROC_H

#include "gencommon.h"

#if PLATFORM == WIN
typedef HANDLE gen_process_t;
#else
typedef pid_t gen_process_t;
#endif

extern GEN_ERRORABLE_RETURN gen_proc_start_redirected(gen_process_t* const restrict process_out, const char* const restrict exec, FILE* const restrict redirect);
extern GEN_ERRORABLE_RETURN gen_proc_wait(int* const restrict out_result, gen_process_t process);
extern GEN_ERRORABLE_RETURN gen_proc_get_output(char** const restrict out_buffer_pointer, int* const restrict out_result, const char* const restrict exec);

#endif
