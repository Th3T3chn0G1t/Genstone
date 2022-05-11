// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file gencommon.h
 * Includes common helpers and utilities for across the codebase.
 */

#ifndef GEN_COMMON_H
#define GEN_COMMON_H

#include "gendiag.h"
#include "generrors.h"

GEN_DIAG_REGION_BEGIN
GEN_DIAG_IGNORE_ALL
#include <assert.h>
#include <complex.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <iso646.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <poll.h>
#include <setjmp.h>
#include <signal.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <tgmath.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>
GEN_DIAG_REGION_END

#include "gemory.h"
#include "gentooling.h"
#include "genutil.h"
#include "glog.h"

#endif
