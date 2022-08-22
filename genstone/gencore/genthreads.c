// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genthreads.h"

void gen_threads_internal_mutex_scoped_lock_cleanup(gen_threads_mutex_t* const restrict * const restrict * const restrict mutex) {
    if(!*mutex) return;

    gen_error_t error = gen_threads_mutex_unlock(**mutex);
    if(error.type) {
        gen_error_print(&error, GEN_ERROR_SEVERITY_FATAL);
        gen_error_abort();
    }
}
