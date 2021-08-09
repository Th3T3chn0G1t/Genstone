// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include "include/genargs.h"

gen_arg_error_t gen_parse_args(const int argc, const char** argv, const gen_arg_handler_t callback, unsigned long n_short_args, char* short_args, unsigned long n_long_args, char** long_args, void* passthrough) {
    // Precalculating the long args' lengths to save time while looping
    unsigned long long_arg_lens[n_long_args];
    for(unsigned long i = 0; i < n_long_args; i++) {
        // Calculating inline instead of using strlen to remove the stdlib dependency
        unsigned long len = 0;
        while(long_args[i][len]) len++;
        long_arg_lens[i] = len;
    }

    for(int i = 1; i < argc; i++) {
        gen_arg_type_t type;
        unsigned long argn = (unsigned long) -1;
        const char* value = (const char*) 0;
        if(argv[i][0] == '-' && argv[i][1] == '-') {
            type = GEN_ARG_LONG;
            for(unsigned long j = 0; j < n_long_args; j++) {
                // Calculating inline instead of using strncmp to remove the stdlib dependency
                // Also might be very slightly faster given the specific use-case
                char equal = 1;
                for(unsigned long k = 0; k < long_arg_lens[j]; k++) {
                    if(long_args[j][k] != (argv[i] + 2)[k]) {
                        equal = 0;
                        break;
                    }
                }
                if(equal) {
                    argn = j;
                    if(argv[i][2 + long_arg_lens[j]]) { // If there are more chars after the switch
                        value = argv[i] + 2 + long_arg_lens[j] + 1;
                    }
                    break;
                }
            }
        }
        else if(argv[i][0] == '-') {
            type = GEN_ARG_SHORT;
            for(unsigned long j = 0; j < n_short_args; j++) {
                if(short_args[j] == argv[i][1]) {
                    argn = j;
                    if(argv[i][2]) { // If there are more chars after the switch
                        value = argv[i] + 2;
                    }
                    break;
                }
            }
        }
        else {
            type = GEN_ARG_RAW;
            argn = 0;
            value = argv[i];
        }
        if(argn == (unsigned long) -1) {
            return GEN_ARG_NO_EXIST;
        }
        callback(type, argn, value, passthrough);
    }

    return GEN_ARG_OK;
}
