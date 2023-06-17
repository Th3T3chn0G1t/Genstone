// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"
#include "include/genformat.h"

#include <genbackends.h>

static GEN_THREAD_LOCAL gen_error_t error_buffer = {0};

gen_error_t* gen_error_attach_backtrace(
        const gen_error_type_t type, const char* const restrict line,
        const char* const restrict format, ...) {

    error_buffer = (gen_error_t) {0};
    gen_error_t* retval = &error_buffer;

    retval->type = type;
    retval->line = line;

    gen_tooling_get_backtrace(retval->backtrace, &retval->backtrace_length);

	gen_variadic_list_t list;
	gen_variadic_list_start(list, format);

    if(gen_format_variadic_list(
            retval->context, GEN_NULL, GEN_ERROR_MAXIMUM_CONTEXT_LENGTH,
            format, list)) gen_error_abort();

	return retval;
}

GEN_BACKENDS_PROC(error_abort, GEN_NORETURN void)
void gen_error_abort(void) {
    gen_backends_error_abort();
}
