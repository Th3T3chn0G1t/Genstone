#include "include/genproc.h"

gen_error_t gen_proc_start_redirected(gen_process_t* const restrict process_out, const char* const restrict exec, FILE* const restrict redirect) {
#if PLATFORM == WIN
	STARTUPINFOA process_settings = { 0 };
	process_settings.cb = sizeof(STARTUPINFO);
	process_settings.wShowWindow = SW_HIDE;
	process_settings.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	process_settings.hStdOutput = (HANDLE) _get_osfhandle(fileno(redirect));
	process_settings.hStdError = (HANDLE) _get_osfhandle(fileno(redirect));

	PROCESS_INFORMATION process;
	if(!CreateProcessA(NULL, exec, NULL, NULL, true, 0, NULL, NULL, &process_settings, &process))) {
		unsigned long winerror = GetLastError();
#if GEN_GLOGGIFY_EH == ENABLED
		size_t size = 0;
		gen_winerr_as_string(NULL, &size, winerror);
		char winerror_string[size];
		gen_winerr_as_string(winerror_string, 0, winerror);
		glogf(ERROR, "Failed to create process '%s': %s", exec, winerror_string);
#endif
		GEN_ERROR_OUT(winerror, "`CreateProcessA` failed");
	}
	CloseHandle(process.hThread);

	*process_out = process.hProcess;

	return GEN_OK;
#else
	gen_process_t pid = fork();

	if(!pid) { // The child
		// Take control of output
		dup2(fileno(redirect), 1);
		dup2(fileno(redirect), 2);

		execl("/bin/sh", "/bin/sh", "-c", exec, NULL);

		errno_t error = errno;
#if GEN_GLOGGIFY_EH == ENABLED
		glogf(ERROR, "`execl` failed: %s", strerror(error));
#endif
		GEN_ERROR_OUT(gen_convert_errno(error), "`execl` failed");

		GEN_REQUIRE_NO_REACH;
	}

	return pid;
#endif
}

gen_error_t gen_proc_wait(int* const restrict out_result, gen_process_t process) {
#if PLATFORM == WIN
	unsigned char result;

	while(result = GetexitcodeProcess(pid, out_result)) && *out_result == STILL_ACTIVE);

	CloseHandle(pid);

	if(!result) out_result = -1;

	return GEN_OK;
#else
	waitpid(pid, out_result, 0);

	return GEN_OK;
#endif
}

gen_error_t gen_proc_get_output(char** const restrict out_buffer_pointer, int* const restrict out_result, const char* const restrict exec) {
#if PLATFORM == WIN
	HANDLE read;
	HANDLE write;
	SECURITY_ATTRIBUTES SecurityAttributes = { 0 };
	SecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	SecurityAttributes.bInheritHandle = true;
	SecurityAttributes.lpSecurityDescriptor = NULL;

	if(CreatePipe(&read, &write, &SecurityAttributes, 0)) {
		char* const output = NULL;
		char* const ansi_output = NULL;
		FILE* redirect_handle = _fdopen(_open_osfhandle((intptr_t) write, _O_APPEND), "w");
		gen_process_t pid = gen_proc_start_redirected(exec, redirect_handle);
		gen_proc_wait(out_result, pid);

		unsigned int BytesAvailable;
		if(PeekNamedPipe(read, NULL, 0, NULL, &BytesAvailable, NULL) && BytesAvailable) {
			ansi_output.resize(BytesAvailable);
			output.resize(BytesAvailable);

			readFile(read, ansi_output.data(), BytesAvailable, NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, ansi_output.c_str(), BytesAvailable, output.data(), BytesAvailable);
		}

		fclose(redirect_handle);
		CloseHandle(read);

		return output;
	}

	return std::wstring();

#elif defined(__linux__) || defined(__APPLE__) // _WIN32

	int FileDescriptors[ 2 ];
	pipe(FileDescriptors);
	fcntl(FileDescriptors[ 0 ], F_SETFL, O_NONBLOCK); // Don't want to block on read

	FILE*     pStream = fdopen(FileDescriptors[ 1 ], "w");
	ProcessID pid     = StartProcess(CommandLine, pStream);
	rResult           = WaitProcess(pid);

	char        Buffer[ 1024 ];
	ssize_t     Length;
	std::string output;

	while((Length = read(FileDescriptors[ 0 ], Buffer, std::size(Buffer))) > 0)
	{
		output.append(Buffer, Length);
	}

	fclose(pStream);
	close(FileDescriptors[ 0 ]);

	return UTF8Converter().from_bytes(output);

#endif // __linux__ || __APPLE__

} // OutputOf
