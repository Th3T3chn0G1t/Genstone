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
            assert(argn < 2);
            if(argn == 0) { // `-f`
                assert(!value);
            }
            else if(argn == 1) { // `-b`
                assert(value);
                assert(!strcmp(value, "foo"));
            }
            break;
        }
        case GEN_ARG_LONG: {
            assert(argn < 2);
            if(argn == 0) { // `--fizz`
                assert(!value);
            }
            else if(argn == 1) { // `--buzz`
                assert(value);
                assert(!strcmp(value, "foo"));
            }
            break;
        }
        case GEN_ARG_RAW: {
            assert(value);
            assert(!strcmp(value, "bar"));
            break;
        }
    }
}

int main() {
    const char* argv[] = { "-f", "-bfoo", "--fizz", "--buzz=foo", "bar" };
    const int argc = sizeof(argv) / sizeof(argv[0]);

    gen_arg_error_t error = gen_parse_args(argc, argv, arg_callback, 2, short_args, 2, long_args, NULL);
    
    assert(error == GEN_ARG_OK);
}
