// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include "include/genargs.h"

gen_error_t gen_parse_args(const int argc, const char** argv, const gen_arg_handler_t callback, size_t n_short_args, const char* short_args, size_t n_long_args, const char** long_args, void* passthrough) {
    // Precalculating the long args' lengths to save time while looping
    size_t long_arg_lens[n_long_args];
    GEN_FOREACH_PTR(i, len, n_long_args, long_arg_lens) {
        *len = 0;
        while(long_args[i][*len]) (*len)++;
    }

    GEN_FOREACH_PTR(i, arg, argc - 1, argv + 1) {
        gen_arg_type_t type;
        size_t argn = SIZE_MAX;
        const char* value = NULL;

        if((*arg)[0] == '-') {
            if((*arg)[1] == '-') {
                type = GEN_ARG_LONG;
                GEN_FOREACH_PTR(j, long_arg, n_long_args, long_args) {
                    const size_t len = long_arg_lens[j];

                    // Calculating inline instead of using strncmp to remove the
                    // stdlib dependency Also might be very slightly faster
                    // given the specific use-case
                    GEN_FOREACH_PTR(k, arg_char, len, *long_arg) {
                        if(*arg_char != (*arg)[k + 2])
                            goto long_arg_continue;
                    }

                    argn = j;
                    if((*arg)[len + 2])
                        value = (*arg) + 2 + len + 1;

                long_arg_continue:
                    continue;
                }
            }
            else {
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
        }
        else {
            type = GEN_ARG_RAW;
            argn = 0;
            value = (*arg);
        }

        if(argn == SIZE_MAX)
            return GEN_NO_SUCH_OBJECT;
        callback(type, argn, value, passthrough);
    }

    return GEN_OK;
}
