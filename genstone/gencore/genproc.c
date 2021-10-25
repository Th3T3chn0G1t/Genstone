#include "include/genproc.h"

#if PLATFORM == WIN
#define GEN_INTERNAL_WIN_EXEC_MAX 32767
#endif

gen_error_t gen_proc_start_redirected(gen_process_t* const restrict process_out, const char* const restrict exec, FILE* const restrict redirect) {
	GEN_FRAME_BEGIN(gen_proc_start_redirected);

#if PLATFORM == WIN
	STARTUPINFOA process_settings = {0};
	process_settings.cb = sizeof(STARTUPINFO);
	process_settings.wShowWindow = SW_HIDE;
	process_settings.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wbad-function-cast"
	process_settings.hStdOutput = (HANDLE) _get_osfhandle(_fileno(redirect));
	process_settings.hStdError = (HANDLE) _get_osfhandle(_fileno(redirect));
GEN_DIAG_REGION_END

	PROCESS_INFORMATION process;
	const size_t exec_size = strnlen_s(exec, GEN_INTERNAL_WIN_EXEC_MAX) + 1;
	char local_exec[exec_size];
	errno_t error = strcpy_s(local_exec, exec_size, exec);
	if(error) GEN_ERROR_OUT_ERRNO(strcpy_s, error);
	if(!CreateProcessA(NULL, local_exec, NULL, NULL, true, 0, NULL, NULL, &process_settings, &process)) GEN_ERROR_OUT_WINERR(CreateProcessA, GetLastError());
	CloseHandle(process.hThread);

	*process_out = process.hProcess;

	GEN_ERROR_OUT(GEN_OK, "");
#else
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

	GEN_ERROR_OUT(GEN_OK, "");
#endif
}

gen_error_t gen_proc_wait(int* const restrict out_result, gen_process_t process) {
	GEN_FRAME_BEGIN(gen_proc_wait);

#if PLATFORM == WIN
	unsigned int result;

	while((result = GetExitCodeProcess(process, (long* const) out_result)) && *out_result == STILL_ACTIVE);

	CloseHandle(process);

	if(!result) *out_result = -1;

	GEN_ERROR_OUT(GEN_OK, "");
#else
	waitpid(process, out_result, 0);

	GEN_ERROR_OUT(GEN_OK, "");
#endif
}

gen_error_t gen_proc_get_output(char** const restrict out_output, int* const restrict out_result, const char* const restrict exec) {
	GEN_FRAME_BEGIN(gen_proc_get_output);

	if(!out_output) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`out_output` was NULL");
	if(!out_result) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`out_result` was NULL");

#if PLATFORM == WIN
	HANDLE read;
	HANDLE write;
	SECURITY_ATTRIBUTES security = {0};
	security.nLength = sizeof(SECURITY_ATTRIBUTES);
	security.lpSecurityDescriptor = NULL;
	security.bInheritHandle = true;

	if(CreatePipe(&read, &write, &security, 0)) {
		char* output = NULL;
		size_t output_buffsize = 0;

		FILE* const redirect_handle = _fdopen(_open_osfhandle((intptr_t) write, _O_APPEND), "w");
		gen_process_t pid;
		GEN_ERROR_OUT_IF(gen_proc_start_redirected(&pid, exec, redirect_handle), "`gen_proc_start_redirected` failed");
		GEN_ERROR_OUT_IF(gen_proc_wait(out_result, pid), "`gen_proc_start_redirected` failed");

		unsigned long n_readable;
		if(PeekNamedPipe(read, NULL, 0, NULL, &n_readable, NULL) && n_readable) {
			output_buffsize += n_readable;
			GEN_ERROR_OUT_IF(grealloc((void**) &output, sizeof(char), output_buffsize), "`grealloc` failed");

			ReadFile(read, output, n_readable, NULL, NULL);
		}

		fclose(redirect_handle);
		CloseHandle(read);

		*out_output = output;

		GEN_ERROR_OUT(GEN_OK, "");
	}

	GEN_ERROR_OUT(GEN_UNKNOWN, "Something went wrong in `gen_proc_get_output`");
#else
	int fds[2];
	pipe(fds);
	fcntl(fds[0], F_SETFL, O_NONBLOCK); // Don't want to block on read

	FILE* redirect_handle = fdopen(fds[1], "w");
	gen_process_t pid;
	
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

	GEN_ERROR_OUT(GEN_OK, "");
#endif
}
