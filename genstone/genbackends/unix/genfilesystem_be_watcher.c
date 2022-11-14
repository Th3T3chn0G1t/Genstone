// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genfilesystem.h>
#include <genmemory.h>

#include <genfilesystem_be.h>

#include <genbackendslibcerror.h>

#include <genfilesystem_be_watcher.h>
#include "include/genfilesystem_be_watcher.h"

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <sys/stat.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

typedef struct gen_backends_unix_filesystem_watcher_handle_watched_t {
    gen_filesystem_handle_t* handle;
    struct stat stat_cached;
    gen_size_t directory_length_cached;
    gen_filesystem_watcher_id_t id;
    gen_bool_t present;
} gen_backends_unix_filesystem_watcher_handle_watched_t;

extern gen_error_t* gen_backends_unix_filesystem_watcher_create(GEN_UNUSED gen_filesystem_watcher_t* const restrict out_watcher);
gen_error_t* gen_backends_unix_filesystem_watcher_create(GEN_UNUSED gen_filesystem_watcher_t* const restrict out_watcher) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_watcher_create, GEN_FILE_NAME);
    if(error) return error;    

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_watcher_destroy(gen_filesystem_watcher_t* const restrict watcher);
gen_error_t* gen_backends_unix_filesystem_watcher_destroy(gen_filesystem_watcher_t* const restrict watcher) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_watcher_destroy, GEN_FILE_NAME);
    if(error) return error;

    if(watcher->native->watched) {
        error = gen_memory_free((void**) &watcher->native->watched);
        if(error) return error;
    }

    return GEN_NULL;
}

// TODO: Implement hashmaps for storing the watched list
extern gen_error_t* gen_backends_unix_filesystem_watcher_add(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_handle_t* const restrict handle, gen_filesystem_watcher_id_t* const restrict out_id);
gen_error_t* gen_backends_unix_filesystem_watcher_add(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_handle_t* const restrict handle, gen_filesystem_watcher_id_t* const restrict out_id) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_watcher_add, GEN_FILE_NAME);
    if(error) return error;

    error = gen_memory_reallocate_zeroed((void**) &watcher->native->watched, watcher->native->bounds, watcher->native->bounds + 1, sizeof(gen_backends_unix_filesystem_watcher_handle_watched_t));
    if(error) return error;

    gen_backends_unix_filesystem_watcher_handle_watched_t* watched = &watcher->native->watched[watcher->native->bounds];

    watched->handle = handle;

    int result = fstat(handle->native->file, &watched->stat_cached);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not add handle to watcher: %t", gen_error_description_from_errno());

    if(handle->type == GEN_FILESYSTEM_HANDLE_DIRECTORY) {
        error = gen_filesystem_handle_directory_list(handle, GEN_NULL, GEN_NULL, &watched->directory_length_cached);
        if(error) return error;
    }


    watched->id = watcher->native->next_id++;
    watched->present = gen_true;
    *out_id = watched->id;

    watcher->native->bounds++;

    return GEN_NULL;
}

extern gen_error_t* gen_backends_unix_filesystem_watcher_remove(gen_filesystem_watcher_t* const restrict watcher, const gen_filesystem_watcher_id_t id);
gen_error_t* gen_backends_unix_filesystem_watcher_remove(gen_filesystem_watcher_t* const restrict watcher, const gen_filesystem_watcher_id_t id) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_watcher_remove, GEN_FILE_NAME);
    if(error) return error;    

    for(gen_size_t i = 0; i < watcher->native->bounds; ++i) {
        if(watcher->native->watched[i].id == id) {
            watcher->native->watched[i].present = gen_false;
            return GEN_NULL;
        }
    }

    return gen_error_attach_backtrace_formatted(GEN_ERROR_NO_SUCH_OBJECT, GEN_LINE_NUMBER, "No handle with id `%uz` exists in watcher", id);
}

extern gen_error_t* gen_backends_unix_filesystem_watcher_poll(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_watcher_event_t* const restrict out_event, gen_filesystem_watcher_id_t* const restrict out_id);
gen_error_t* gen_backends_unix_filesystem_watcher_poll(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_watcher_event_t* const restrict out_event, gen_filesystem_watcher_id_t* const restrict out_id) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_backends_unix_filesystem_watcher_poll, GEN_FILE_NAME);
    if(error) return error;

    for(gen_size_t i = 0; i < watcher->native->bounds; ++i) {
        gen_backends_unix_filesystem_watcher_handle_watched_t* watched = &watcher->native->watched[i];

        struct stat poll_stat = {0};
        int result = fstat(watched->handle->native->file, &watched->stat_cached);
        if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not poll for updates: %t", gen_error_description_from_errno());
        
        // TODO: Check link counts for deletion
        if(poll_stat.st_mtime != watched->stat_cached.st_mtime || poll_stat.st_ctime != watched->stat_cached.st_ctime) {
            if(watched->handle->type == GEN_FILESYSTEM_HANDLE_DIRECTORY) {
                gen_size_t length = 0;
                error = gen_filesystem_handle_directory_list(watched->handle, GEN_NULL, GEN_NULL, &length);
                if(error) return error;

                if(length > watched->directory_length_cached) *out_event |= GEN_FILESYSTEM_WATCHER_EVENT_CREATED;
                else if(length < watched->directory_length_cached) *out_event |= GEN_FILESYSTEM_WATCHER_EVENT_DELETED;
                else *out_event |= GEN_FILESYSTEM_WATCHER_EVENT_MODIFIED;

                watched->directory_length_cached = length;
                *out_id = watched->id;
                return GEN_NULL;
            }
            else {
                *out_id = watched->id;
                *out_event |= GEN_FILESYSTEM_WATCHER_EVENT_MODIFIED;
                return GEN_NULL;
            }
        }
    }

    return GEN_NULL;
}
