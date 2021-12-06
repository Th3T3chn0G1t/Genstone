// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genfs.h>
#include <genproc.h>

int main(void) {
	GEN_FRAME_BEGIN(main);

	gen_filesystem_handle_t pipedes[2];
	glog(INFO, "Testing gen_handle_create_proc_redirect_target()...");
	gen_error_t error = gen_handle_create_proc_redirect_target(&pipedes[0], &pipedes[1]);
	GEN_REQUIRE_NO_ERROR(error);

	gen_process_t pid = 0;
	glog(INFO, "Testing gen_proc_create_redirected_to()...");
	error = gen_proc_create_redirected_to(&pid, "echo Genstone Rules!", &pipedes[1]);
	GEN_REQUIRE_NO_ERROR(error);

	int exitcode;
	glog(INFO, "Testing gen_proc_wait()...");
	error = gen_proc_wait(&pid, &exitcode);
	GEN_REQUIRE_NO_ERROR(error);

	glog(INFO, "Testing gen_handle_read_all_available()...");
	char* output = NULL;
	error = gen_handle_read_all_available(&pipedes[0], &output);
	GEN_REQUIRE_NO_ERROR(error);
	GEN_REQUIRE_EQUAL_STRING("Genstone Rules!\n", output);

	error = gfree(output);
	GEN_REQUIRE_NO_ERROR(error);

	glog(INFO, "Testing gen_proc_kill()...");
	gen_process_t sleeper = 0;
	error = gen_proc_create_redirected_to(&sleeper, "sleep 60", &gen_stdout_handle);
	GEN_REQUIRE_NO_ERROR(error);
	error = gen_proc_kill(&sleeper);
	GEN_REQUIRE_NO_ERROR(error);
}
