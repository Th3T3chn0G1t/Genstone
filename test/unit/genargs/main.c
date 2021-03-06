// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <genargs.h>
#include <gencommon.h>

static const char short_args[] = {'f', 'b'};

static const char* long_args[] = {"fizz", "buzz"};

static gen_error_t arg_callback(const gen_arg_type_t type, const size_t argn, const char* const restrict value, __unused void* const restrict passthrough) {
	GEN_FRAME_BEGIN(arg_callback);

	if(type < GEN_ARG_RAW)
		if(!argn)
			GEN_REQUIRE_EQUAL(NULL, (const void*) value);
		else
			GEN_REQUIRE_EQUAL_STRING("foo", value);
	else
		GEN_REQUIRE_EQUAL_STRING("bar", value);

	GEN_ALL_OK;
}

int main(void) {
	const char* argv[] = {"-f", "-bfoo", "--fizz", "--buzz=foo", "bar"};
	const int argc = sizeof(argv) / sizeof(argv[0]);

	gen_error_t error = gen_parse_args(argc, argv, arg_callback, 2, short_args, 2, long_args, NULL);

	GEN_REQUIRE_NO_ERROR(error);
}
