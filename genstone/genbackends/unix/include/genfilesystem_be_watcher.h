// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_FILESYSTEM_FILEWATCH_BE_H
#define GEN_FILESYSTEM_FILEWATCH_BE_H

typedef struct gen_backends_unix_filesystem_watcher_handle_watched_t gen_backends_unix_filesystem_watcher_handle_watched_t;

typedef struct gen_backends_filesystem_watcher_handle_t {
    gen_backends_unix_filesystem_watcher_handle_watched_t* watched;
    gen_size_t bounds;
    gen_filesystem_watcher_id_t next_id;
} gen_backends_filesystem_watcher_handle_t;

#endif
