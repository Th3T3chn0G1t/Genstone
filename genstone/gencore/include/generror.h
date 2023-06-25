// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#ifndef GEN_ERROR_H
#define GEN_ERROR_H

#include "gentoolingframe.h"

typedef enum {
    GEN_ERROR_UNKNOWN,
    GEN_ERROR_PERMISSION,
    GEN_ERROR_INVALID_PARAMETER,
    GEN_ERROR_IO,
    GEN_ERROR_TOO_LONG,
    GEN_ERROR_TOO_SHORT,
    GEN_ERROR_NO_SUCH_OBJECT,
    GEN_ERROR_OUT_OF_MEMORY,
    GEN_ERROR_WRONG_OBJECT_TYPE,
    GEN_ERROR_ALREADY_EXISTS,
    GEN_ERROR_OUT_OF_SPACE,
    GEN_ERROR_OUT_OF_HANDLES,
    GEN_ERROR_BAD_CONTENT,
    GEN_ERROR_BAD_OPERATION,
    GEN_ERROR_IN_USE,
    GEN_ERROR_NOT_IMPLEMENTED,
    GEN_ERROR_OUT_OF_BOUNDS,
    GEN_ERROR_INVALID_CONTROL,
    GEN_ERROR_BAD_ALIGNMENT,
    GEN_ERROR_OPERATION_FAILED,
    GEN_ERROR_BAD_TIMING,
    GEN_ERROR_TIMEOUT,
    GEN_ERROR_DOES_NOT_MATCH
} gen_error_type_t;

typedef struct {
    gen_jump_point_t point;
    gen_error_type_t type;
} gen_failpoint_t;

#define GEN_FAILPOINT \
    do { \
        gen_failpoint_t _gen_failpoint; \
        if(_gen_failpoint.type = gen_set_jump_point(_gen_failpoint.point)) { \
            return gen_error_attach_backtrace( \
                        _gen_failpoint.type, GEN_LINE_STRING, \
                        "Nested call failure"); \
        } \
    } while(0)

#define GEN_CLEANUP_PROC_FAILPOINT(t) \
    struct { \
        gen_failpoint_t* failpoint; \
        t variable; \
    }* data = *ptr;

#define GEN_CLEANUP_FAILPOINT(func, v) \
    GEN_CLEANUP_FUNCTION(func) \
    struct { \
        gen_failpoint_t* failpoint; \
        GEN_TYPEOF(v) variable; \
    } cleanup_##v = { &_gen_failpoint, v };

#ifndef GEN_ERROR_MAXIMUM_CONTEXT_LENGTH
#define GEN_ERROR_MAXIMUM_CONTEXT_LENGTH 2048
#endif

typedef struct {
    gen_error_type_t type;

    const char* line;
    char context[GEN_ERROR_MAXIMUM_CONTEXT_LENGTH + 1];

    gen_backtrace_t backtrace;
    gen_size_t backtrace_length;
} gen_error_t;

const char* gen_error_type_name(const gen_error_type_t error);
const char* gen_error_type_description(const gen_error_type_t error);

gen_error_t* gen_error_attach_backtrace(
            const gen_error_type_t type, const char* const restrict line,
            const char* const restrict format, ...);

GEN_NORETURN void gen_abort(void);

#endif
