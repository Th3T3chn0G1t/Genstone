// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gencommon.h
 * Includes common helpers and utilities for across the codebase.
 * Includes the stdlib and helpers to provide missing headers on noncompliant platforms.
 * Defines helper macros for pretty keywords and diagnostic info.
 */

#ifndef GEN_COMMON_H
#define GEN_COMMON_H

#include "gendiag.h"
#include "gendbg.h"
#include "generrors.h"

GEN_DIAG_REGION_BEGIN
GEN_DIAG_IGNORE_ALL
#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <ctype.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <iso646.h>
#include <limits.h>
#if PLATFORM == LNX
#include <linux/limits.h> // Why is PATH_MAX here of all places?
#endif
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdalign.h>
#include <stdatomic.h>
#include <time.h>
#include <threads.h>
#include <wchar.h>
#include <wctype.h>
#include <tgmath.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef GEN_USE_MIMALLOC
/**
 * Whether to use mimalloc as the allocator.
 */
#define GEN_USE_MIMALLOC ENABLED
#endif

#if GEN_USE_MIMALLOC == ENABLED
#include <mimalloc.h> // \[0]/
#endif

#include <safe_types.h>
#include <safe_lib.h>
#include <safe_lib_errno.h>
#include <safe_mem_lib.h>
#include <safe_str_lib.h>

#include <stdnoreturn.h>
GEN_DIAG_REGION_END

#include "glog.h"
#include "genutil.h"
#include "gentooling.h"
#include "gemory.h"

#endif
