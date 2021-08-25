// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include <genargs.h>
#include <gencommon.h>

static const char short_args[] = {'f', 'b'};

static const char* long_args[] = {"fizz", "buzz"};

static void arg_callback(const gen_arg_type_t type, const unsigned long argn, const char* value, __unused void* passthrough) {
	switch(type) {
		case GEN_ARG_SHORT: {
			switch(argn) {
				case 0: {
					// `-f`
					GEN_REQUIRE_EQUAL(NULL, (const void*) value);
					return;
				}
				case 1: {
					// `-b`
					GEN_REQUIRE_EQUAL_STRING("foo", value);
					return;
				}
			}
			GEN_REQUIRE_NO_REACH;
		}
		case GEN_ARG_LONG: {
			switch(argn) {
				case 0: {
					// `--fizz`
					GEN_REQUIRE_EQUAL(NULL, (const void*) value);
					return;
				}
				case 1: {
					// `--buzz`
					GEN_REQUIRE_EQUAL_STRING("foo", value);
					return;
				}
			}
			GEN_REQUIRE_NO_REACH;
		}
		case GEN_ARG_RAW: {
			GEN_REQUIRE_EQUAL_STRING("bar", value);
			return;
		}
	}
	GEN_REQUIRE_NO_REACH;
}

int main() {
	const char* argv[] = {"-f", "-bfoo", "--fizz", "--buzz=foo", "bar"};
	const int argc = sizeof(argv) / sizeof(argv[0]);

	gen_error_t error = gen_parse_args(argc, argv, arg_callback, 2, short_args, 2, long_args, NULL);

	GEN_REQUIRE_EQUAL(GEN_OK, error);
}
