// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include <gencommon.h>
#include <gendl.h>

int main() {
	GEN_FRAME_BEGIN(main);

	gen_error_t error;
	gen_dylib_t testlib;

	glog(INFO, "Testing gen_dylib_load()...");
	error = gen_dylib_load(&testlib, "gendl_test");

	GEN_REQUIRE_NO_ERROR(error);

	glog(INFO, "Testing gen_dylib_symbol()...");
	const char* (*foo)(void);
	error = gen_dylib_symbol((void**) &foo, testlib, "foo");

	GEN_REQUIRE_NO_ERROR(error);
	GEN_REQUIRE_EQUAL_STRING("This is from a dylib!", foo());

	glog(INFO, "Testing gen_dylib_unload()...");
	error = gen_dylib_unload(testlib);

	GEN_REQUIRE_NO_ERROR(error);
}
