// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genproc.h>

int main(void) {
    // Create the output target for the subprocess
	gen_filesystem_handle_t pipedes[2];
	gen_error_t error = gen_handle_create_proc_redirect_target(&pipedes[0], &pipedes[1]);
	GEN_REQUIRE_NO_ERROR(error);

    // Create the subprocess
	gen_process_t pid = 0;
	error = gen_proc_create_redirected_to(&pid, "echo Genstone Rules!", &pipedes[1] /* Replace with `&gen_stdout_handle` to output to stdout */);
	GEN_REQUIRE_NO_ERROR(error);

    // Wait for the subprocess to exit
	int exitcode;
	error = gen_proc_wait(&pid, &exitcode);
	GEN_REQUIRE_NO_ERROR(error);
	glogf(INFO, "exitcode: %i", exitcode);

    // Read all the data out of the redirect pipe
    // Unnecesary if you're using stdout
	char* output = NULL;
	error = gen_handle_read_all_available(&pipedes[0], &output);
	GEN_REQUIRE_NO_ERROR(error);
	glogf(INFO, "output: %s", output);

    // Free the output buffer
	error = gfree(output);
	GEN_REQUIRE_NO_ERROR(error);

	gen_process_t sleeper = 0;
    // Create a new process which should sleep for one minute
	error = gen_proc_create_redirected_to(&sleeper, "sleep 60", &gen_stdout_handle);
	GEN_REQUIRE_NO_ERROR(error);
    // Kill the process so it doesn't hang the terminal :^)
	error = gen_proc_kill(&sleeper);
	GEN_REQUIRE_NO_ERROR(error);
}
