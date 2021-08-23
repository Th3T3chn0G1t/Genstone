// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include "include/genargs.h"
#include "include/gentooling.h"

gen_error_t gen_parse_args(const int argc, const char* const restrict * restrict const argv, const gen_arg_handler_t handler, const size_t n_short_args, const char* restrict short_args, const size_t n_long_args, const char* const restrict * const restrict long_args, void* restrict passthrough) {
    GEN_FRAME_BEGIN;

    if(!argc) {
        return GEN_OK;
    }

    if(!argv) {
        return GEN_INVALID_PARAMETER;
    }
    if(argc < 0) {
        return GEN_INVALID_PARAMETER;
    }
    if(!handler) {
        return GEN_INVALID_PARAMETER;
    }
    if(n_short_args && !short_args) {
        return GEN_INVALID_PARAMETER;
    }
    if(n_long_args && !long_args) {
        return GEN_INVALID_PARAMETER;
    }

    // Precalculating the long args' lengths to save time while looping
    // We need this weirdness to avoid creating a zero-length VLA
    size_t long_arg_lens[n_long_args ? n_long_args : 1];
    if(n_long_args) {
        GEN_FOREACH_PTR(i, len, n_long_args, long_arg_lens) {
            *len = 0;
            while(long_args[i][*len]) ++(*len);
        }
    }

    GEN_FOREACH_PTR(i, arg, argc - 1, argv + 1) {
        gen_arg_type_t type;
        size_t argn = SIZE_MAX;
        const char* value = NULL;

        if((*arg)[0] == '-') {
            if((*arg)[1] == '-' && n_long_args) {
                type = GEN_ARG_LONG;
                GEN_FOREACH_PTR(j, long_arg, n_long_args, long_args) {
                    const size_t arg_len = long_arg_lens[j];

                    // Calculating inline instead of using strncmp to remove the
                    // stdlib dependency Also might be very slightly faster
                    // given the specific use-case
                    GEN_FOREACH_PTR(k, arg_char, arg_len, *long_arg) {
                        if(*arg_char != (*arg)[k + 2])
                            goto long_arg_continue;
                    }

                    argn = j;
                    if((*arg)[arg_len + 2])
                        value = (*arg) + 2 + arg_len + 1;

                long_arg_continue:
                    continue;
                }
            }
            else if(n_short_args) {
                type = GEN_ARG_SHORT;
                GEN_FOREACH_PTR(j, short_arg, n_short_args, short_args) {
                    if((*short_arg) != (*arg)[1])
                        continue;

                    argn = j;

                    if((*arg)[2])
                        value = (*arg) + 2;
                    break;
                }
            }
            else {
                type = GEN_ARG_RAW;
                argn = 0;
                value = (*arg);
            }
        }
        else {
            type = GEN_ARG_RAW;
            argn = 0;
            value = (*arg);
        }

        if(argn == SIZE_MAX) {
                return GEN_NO_SUCH_OBJECT;
        }
        handler(type, argn, value, passthrough);
    }

    return GEN_OK;
}
