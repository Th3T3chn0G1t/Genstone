// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_BACKENDS_LIBC_ERROR_H
#define GEN_BACKENDS_LIBC_ERROR_H

extern gen_error_type_t gen_error_type_from_errno(void);
extern const char* gen_error_description_from_errno(void);

#endif
