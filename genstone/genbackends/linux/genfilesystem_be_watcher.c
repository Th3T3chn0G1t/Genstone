// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genfilesystem.h>



#include <genfilesystem_be.h>

#include <genfilesystem_be_watcher.h>
#include "include/genfilesystem_be_watcher.h"


gen_error_t* gen_backends_linux_filesystem_watcher_create(gen_filesystem_watcher_t* const restrict out_watcher) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_linux_filesystem_watcher_create, GEN_FILE_NAME);
    if(error) return error;

    return GEN_NULL;
}


gen_error_t* gen_backends_linux_filesystem_watcher_destroy(gen_filesystem_watcher_t* const restrict watcher) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_linux_filesystem_watcher_destroy, GEN_FILE_NAME);
    if(error) return error;

    return GEN_NULL;
}


gen_error_t* gen_backends_linux_filesystem_watcher_add(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_handle_t* const restrict handle) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_linux_filesystem_watcher_add, GEN_FILE_NAME);
    if(error) return error;

    return GEN_NULL;
}


gen_error_t* gen_backends_linux_filesystem_watcher_remove(gen_filesystem_watcher_t* const restrict watcher, const gen_filesystem_watcher_id_t id) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_linux_filesystem_watcher_remove, GEN_FILE_NAME);
    if(error) return error;

    return GEN_NULL;
}


gen_error_t* gen_backends_linux_filesystem_watcher_poll(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_watcher_event_t* const restrict out_event, gen_filesystem_watcher_id_t* const restrict out_id) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_linux_filesystem_watcher_poll, GEN_FILE_NAME);
    if(error) return error;

    return GEN_NULL;
}
