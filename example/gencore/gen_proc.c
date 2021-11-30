// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genproc.h>

int main(void) {
    FILE* proc_stream;
    gen_process_t proc;
    gen_error_t error = gen_proc_start_redirected(&proc, "echo Genstone Rules!", proc_stream);
	GEN_REQUIRE_NO_ERROR(error);

    int exit_code;
    error = gen_proc_wait(&exit_code, proc);
	GEN_REQUIRE_NO_ERROR(error);

	char* output = NULL;
	int result = 0;
	error = gen_proc_get_output(&output, &result, "echo Genstone Rules!");
	GEN_REQUIRE_NO_ERROR(error);

    error = gfree(output);
    GEN_REQUIRE_NO_ERROR(error);
}
