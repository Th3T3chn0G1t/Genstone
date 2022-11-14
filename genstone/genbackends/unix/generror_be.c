// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

GEN_NORETURN extern void gen_backends_unix_error_abort(void);
GEN_NORETURN void gen_backends_unix_error_abort(void) {
    (void) kill(getpid(), SIGKILL);
    // This is just a fallback if we're on a weird system
    // Where `kill`ing ourself doesn't work.
    pthread_exit(NULL);
}
