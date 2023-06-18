// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genlog.h"
#include "include/genformat.h"

#include <genbackends.h>

#ifndef GEN_LOG_MAXIMUM_FORMATTED_LENGTH
#define GEN_LOG_MAXIMUM_FORMATTED_LENGTH 8192
#endif

#ifndef GEN_LOG_MAXIMUM_CONTEXT_LENGTH
#define GEN_LOG_MAXIMUM_CONTEXT_LENGTH 32
#endif

GEN_BACKENDS_PROC(terminal_write, void)
gen_error_t* gen_log(
        const gen_log_level_t level, const char* const restrict context,
        const char* const restrict format, ...) {

    gen_tooling_push(GEN_FUNCTION_NAME, GEN_FILE_NAME);
    GEN_TOOLING_AUTO gen_error_t* error;

    if(!context) {
        return gen_error_attach_backtrace(
                GEN_ERROR_INVALID_PARAMETER, GEN_LINE_STRING,
                "`context` was `GEN_NULL`");
    }

    if(!format) {
        return gen_error_attach_backtrace(
                GEN_ERROR_INVALID_PARAMETER, GEN_LINE_STRING,
                "`format` was `GEN_NULL`");
    }

    GEN_VARIADIC_LIST_AUTO gen_variadic_list_t list;
    gen_variadic_list_start(list, format);

    char message[GEN_LOG_MAXIMUM_FORMATTED_LENGTH + 1] = {0};
    error = gen_format_variadic_list(
            message, GEN_NULL, GEN_LOG_MAXIMUM_FORMATTED_LENGTH, format, list);
    if(error) return error;

    const char* levels[] = {
        [GEN_LOG_LEVEL_TRACE]   = "trace  ",
        [GEN_LOG_LEVEL_DEBUG]   = "debug  ",
        [GEN_LOG_LEVEL_INFO]    = "info   ",
        [GEN_LOG_LEVEL_WARNING] = "warning",
        [GEN_LOG_LEVEL_ERROR]   = "error  ",
        [GEN_LOG_LEVEL_FATAL]   = "fatal  "
    };

    // [context         ][level  ] message
    // [ + GEN_LOG_MAXIMUM_CONTEXT_LENGTH + ][ + 7 ("warning") + ] + 1
    // GEN_LOG_MAXIMUM_CONTEXT_LENGTH + 12

    char buf[
            GEN_LOG_MAXIMUM_FORMATTED_LENGTH +
            (GEN_LOG_MAXIMUM_CONTEXT_LENGTH + 12) + 1] = {0};

    gen_size_t context_pad = GEN_LOG_MAXIMUM_CONTEXT_LENGTH;
    for(gen_size_t i = 0; context[i]; ++i) {
        context_pad--;
        if(!context_pad && context[i + 1]) {
            return gen_error_attach_backtrace(
                GEN_ERROR_TOO_LONG, GEN_LINE_STRING,
                "Context string `%t` length exceeded context maximum `%uz`",
                context, GEN_LOG_MAXIMUM_CONTEXT_LENGTH);
        }
    }

    error = gen_format(
                buf, GEN_NULL, sizeof(buf), "[%t%cz][%t] %t",
                context, ' ', context_pad, levels[level], message);
    if(error) return error;

    gen_backends_terminal_write(buf);

    return GEN_NULL;
}
