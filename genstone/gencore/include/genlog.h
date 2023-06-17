// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_LOG_H
#define GEN_LOG_H

#include "gencommon.h"

typedef enum {
    GEN_LOG_LEVEL_TRACE,
    GEN_LOG_LEVEL_DEBUG,
    GEN_LOG_LEVEL_INFO,
    GEN_LOG_LEVEL_WARNING,
    GEN_LOG_LEVEL_ERROR,
    GEN_LOG_LEVEL_FATAL
} gen_log_level_t;

gen_error_t* gen_log(
        const gen_log_level_t level, const char* const restrict context,
        const char* const restrict format, ...);

#endif
