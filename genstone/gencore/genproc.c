#include "include/genproc.h"

gen_error_t gen_proc_start_redirected(gen_process_t* const restrict process_out, const char* const restrict exec, FILE* const restrict redirect) {
	GEN_FRAME_BEGIN(gen_proc_start_redirected);

	gen_process_t pid = fork();

	if(pid == -1) GEN_ERROR_OUT_ERRNO(fork, errno);

	if(!pid) { // The child
		// Take control of output
		dup2(fileno(redirect), STDOUT_FILENO);
		dup2(fileno(redirect), STDERR_FILENO);

		execl("/bin/sh", "sh", "-c", exec, NULL);

		GEN_ERROR_OUT_ERRNO(execl, errno);
	}

	*process_out = pid;

	GEN_ALL_OK;
}

gen_error_t gen_proc_wait(int* const restrict out_result, gen_process_t process) {
	GEN_FRAME_BEGIN(gen_proc_wait);

	waitpid(process, out_result, 0);

	GEN_ALL_OK;
}

gen_error_t gen_proc_get_output(char** const restrict out_output, int* const restrict out_result, const char* const restrict exec) {
	GEN_FRAME_BEGIN(gen_proc_get_output);

	GEN_INTERNAL_BASIC_PARAM_CHECK(out_output);
	GEN_INTERNAL_BASIC_PARAM_CHECK(out_result);

	int fds[2];
	pipe(fds);
	fcntl(fds[0], F_SETFL, O_NONBLOCK); // Don't want to block on read

	FILE* redirect_handle = fdopen(fds[1], "w");
	gen_process_t pid = 0;

	GEN_ERROR_OUT_IF(gen_proc_start_redirected(&pid, exec, redirect_handle), "`gen_proc_start_redirected` failed");
	GEN_ERROR_OUT_IF(gen_proc_wait(out_result, pid), "`gen_proc_wait` failed");

	char* output = NULL;
	GEN_ERROR_OUT_IF(gzalloc((void**) &output, sizeof(char), GEN_PROC_READ_BUFFER_CHUNK), "`gzalloc` failed");
	size_t output_buffsize = 0;

	char read_data[GEN_PROC_READ_BUFFER_CHUNK] = {0};
	ssize_t read_size;

	while((read_size = read(fds[0], output, GEN_PROC_READ_BUFFER_CHUNK)) > 0) {
		output_buffsize += (size_t) read_size;
		GEN_ERROR_OUT_IF(grealloc((void**) &output, sizeof(char), output_buffsize), "`grealloc` failed");

		strcat_s(output, output_buffsize, read_data);
	}

	fclose(redirect_handle);
	close(fds[0]);

	*out_output = output;

	GEN_ALL_OK;
}
