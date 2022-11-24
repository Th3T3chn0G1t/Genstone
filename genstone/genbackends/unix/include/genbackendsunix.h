// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_BACKENDS_UNIX_H
#define GEN_BACKENDS_UNIX_H

extern gen_error_t* gen_backends_unix_filesystem_internal_path_from_fd(const int fd, char* const restrict out_path, gen_size_t size);

#endif
