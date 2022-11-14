// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_FILESYSTEM_BE_H
#define GEN_FILESYSTEM_BE_H

typedef struct gen_backends_filesystem_handle_t {
    void* directory;
    int file;
} gen_backends_filesystem_handle_t;

#endif
