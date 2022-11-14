// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_THREADS_BE_H
#define GEN_THREADS_BE_H

typedef struct gen_backends_threads_mutex_t {
    void* mutex;
} gen_backends_threads_mutex_t;

typedef struct gen_backends_threads_handle_t {
    void* thread;
} gen_backends_threads_handle_t;

#endif
