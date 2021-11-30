// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genproc.h>

#define EXEC_TEST_CONTENT "Genstone Rules!"

int main(void) {
	GEN_FRAME_BEGIN(main);

	glog(INFO, "Testing `gen_proc_get_output`...");
	char* output = NULL;
	int result = 0;
	gen_error_t error = gen_proc_get_output(&output, &result, "echo \"" EXEC_TEST_CONTENT "\"");

	GEN_REQUIRE_NO_ERROR(error);
	GEN_REQUIRE_EQUAL_STRING(EXEC_TEST_CONTENT, output);

	(void) gfree(output);
}
