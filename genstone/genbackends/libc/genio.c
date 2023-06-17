// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include <gencommon.h>

#include <genbackends.h>

#include <stdio.h>

GEN_BACKENDS_DEFER_NOGEN(terminal_write, void, libc, "puts", )
