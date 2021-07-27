/**
 * @file gencommon.h
 * Includes common helpers and utilities for across the codebase
 * Includes the stdlib and helpers to provide missing headers on noncompliant platforms
 * Defines helper macros for pretty keywords and diagnostic info
 * Defines common data structures and operations
 */

#ifndef GEN_COMMON_H
#define GEN_COMMON_H

/**
 * Pretty keyword _Pragma
 * @param s string-literal argument for _Pragma
 */
#define pragma(s) _Pragma(s)
/**
 * Pretty keyword _Generic
 * @param s type-switch statement for _Generic
 */
#define generic(s) _Generic(s)

/**
 * Diagnostic helper
 * Begins a diagnostic region
 */
#define GEN_DIAG_REGION_BEGIN pragma("clang diagnostic push")
/**
 * Diagnostic helper
 * Ends a diagnostic region
 */
#define GEN_DIAG_REGION_END pragma("clang diagnostic pop")
/**
 * Diagnostic helper
 * Ignores all extra compiler warnings in a diagnostic region
 */
#define GEN_DIAG_IGNORE_ALL pragma("clang diagnostic ignored \"-Weverything\"")

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
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>
#if PLATFORM != WIN // clang tgmath.h is broken under windows
#include <tgmath.h>
#endif
#include <stdnoreturn.h>
#include <dirent.h> // Emulation provided on windows by https://github.com/tronkko/dirent
#include <unistd.h> // Apparently just works (kinda) on windows - go figure
#include <sys/types.h>
#include <sys/stat.h>
GEN_DIAG_REGION_END

#endif
