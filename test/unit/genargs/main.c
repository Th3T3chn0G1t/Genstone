// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include <genargs.h>
#include <gencommon.h>

static const char short_args[] = {
    'f',
    'b'
};

static const char* long_args[] = {
    "fizz",
    "buzz"
};

static void arg_callback(const gen_arg_type_t type, const unsigned long argn, const char* value, void* passthrough) {
    switch(type) {
        case GEN_ARG_SHORT: {
            switch(argn) {
                case 0: {
                    // `-f`
                    gen_require_equal(NULL, value);
                    break;
                }
                case 1: {
                    // `-b`
                    gen_require_equal_string("foo", value);
                    break;
                }
                default: gen_require_no_reach;
            }
            break;
        }
        case GEN_ARG_LONG: {
            switch(argn) {
                case 0: {
                    // `--fizz`
                    gen_require_equal(NULL, value);
                    break;                    
                }
                case 1: {
                    // `--buzz`
                    gen_require_equal_string("foo", value);
                    break;
                }
                default: gen_require_no_reach;
            }
            break;
        }
        case GEN_ARG_RAW: {
            gen_require_equal_string("bar", value);
            break;
        }
        default: gen_require_no_reach;
    }
}

int main() {    
    const char* argv[] = { "-f", "-bfoo", "--fizz", "--buzz=foo", "bar" };
    const int argc = sizeof(argv) / sizeof(argv[0]);

    gen_error_t error = gen_parse_args(argc, argv, arg_callback, 2, short_args, 2, long_args, NULL);
    
    gen_require_equal(GEN_OK, error);
}
