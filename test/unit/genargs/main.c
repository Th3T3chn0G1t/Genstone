// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include <genargs.h>
#include <gencommon.h>

static char short_args[] = {
    'f',
    'b'
};

static char* long_args[] = {
    "fizz",
    "buzz"
};

static void arg_callback(const gen_arg_type_t type, const unsigned long argn, const char* value, void* passthrough) {
    switch(type) {
        case GEN_ARG_SHORT: {
            if(argn == 0) { // `-f`
                gen_require_equal(NULL, value);
            }
            else if(argn == 1) { // `-b`
                gen_require_equal_string("foo", value);
            }
            break;
        }
        case GEN_ARG_LONG: {
            if(argn == 0) { // `--fizz`
                gen_require_equal(NULL, value);
            }
            else if(argn == 1) { // `--buzz`
                gen_require_equal_string("foo", value);
            }
            break;
        }
        case GEN_ARG_RAW: {
            gen_require_equal_string("bar", value);
            break;
        }
    }
}

int main() {
    const char* argv[] = { "-f", "-bfoo", "--fizz", "--buzz=foo", "bar" };
    const int argc = sizeof(argv) / sizeof(argv[0]);

    gen_error_t error = gen_parse_args(argc, argv, arg_callback, 2, short_args, 2, long_args, NULL);
    
    gen_require_equal(GEN_OK, error);
}
