// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genproc.h"

gen_error_t gen_handle_create_proc_redirect_target(gen_filesystem_handle_t* restrict output_handle_read, gen_filesystem_handle_t* restrict output_handle_write) {
	GEN_FRAME_BEGIN(gen_handle_create_proc_redirect_target);

	GEN_INTERNAL_BASIC_PARAM_CHECK(output_handle_read);
	GEN_INTERNAL_BASIC_PARAM_CHECK(output_handle_write);

	int pipedes[2];
	pipe(pipedes);
	GEN_ERROR_OUT_IF_ERRNO(pipe, errno);

	fcntl(pipedes[0], F_SETFL, O_RDONLY | O_NONBLOCK);
	GEN_ERROR_OUT_IF_ERRNO(fcntl, errno);

	output_handle_read->file_handle = pipedes[0];
	output_handle_read->is_directory = false;

	output_handle_write->file_handle = pipedes[1];
	output_handle_write->is_directory = false;

	GEN_ALL_OK;
}

gen_error_t gen_proc_create_redirected_to(gen_process_t* const restrict out_process, const char* const restrict exec, const gen_filesystem_handle_t* const restrict redirect) {
	GEN_FRAME_BEGIN(gen_proc_create_redirected_to);

	GEN_INTERNAL_BASIC_PARAM_CHECK(out_process);
	GEN_INTERNAL_BASIC_PARAM_CHECK(exec);
	GEN_INTERNAL_BASIC_PARAM_CHECK(redirect);
	if(redirect->is_directory) GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "`redirect` was a directory");

	gen_process_t pid = fork();
	// Fork does some weird stuff to errno here
	// It sets EINVAL which obviously doesn't make sense
	// So we just have to reset whatever it does
	// This is similar to how we're handling the errno-leaking way mimalloc does NUMA detection
	if(pid == -1) GEN_ERROR_OUT_ERRNO(fork, errno);
	errno = EOK;

	if(!pid) {
		dup2(redirect->file_handle, STDOUT_FILENO);
		GEN_ERROR_OUT_IF_ERRNO(dup2, errno);
		dup2(redirect->file_handle, STDERR_FILENO);
		GEN_ERROR_OUT_IF_ERRNO(dup2, errno);
		execl("/bin/sh", "sh", "-c", exec, NULL);
		GEN_ERROR_OUT_IF_ERRNO(execl, errno);
	}

	*out_process = pid;

	GEN_ALL_OK;
}

gen_error_t gen_proc_wait(const gen_process_t* const restrict process, int* const restrict out_exitcode) {
	GEN_FRAME_BEGIN(gen_proc_wait);

	GEN_INTERNAL_BASIC_PARAM_CHECK(process);
	GEN_INTERNAL_BASIC_PARAM_CHECK(out_exitcode);

	int exitcode = 0;
	waitpid(*process, &exitcode, 0);
	GEN_ERROR_OUT_IF_ERRNO(waitpid, errno);

	if(WIFEXITED(exitcode)) {
		*out_exitcode = WEXITSTATUS(exitcode);
	}
	else {
		*out_exitcode = 0;
	}

	GEN_ALL_OK;
}

gen_error_t gen_proc_kill(const gen_process_t* const restrict process) {
	GEN_FRAME_BEGIN(gen_proc_kill);

	GEN_INTERNAL_BASIC_PARAM_CHECK(process);

	kill(*process, SIGTERM);
	// Since kill doesn't handle "zombie" processes gracefully we just need to exit here
	// It's possible that in future `gen_proc_kill` will signal the status of the killed process to the caller
	if(errno == ESRCH) {
		errno = EOK;
		GEN_ALL_OK;
	}
	GEN_ERROR_OUT_IF_ERRNO(kill, errno);

	GEN_ALL_OK;
}
