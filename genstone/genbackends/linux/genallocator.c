// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>

#include <genbackends.h>

GEN_BACKENDS_DEFER(get_system_allocator, gen_error_t*, linux, "libc", return)
