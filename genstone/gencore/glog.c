// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include "include/gencommon.h"

FILE* gen_glog_out_streams[GEN_GLOG_STREAM_COUNT + 1] = {0};
FILE* gen_glog_err_streams[GEN_GLOG_STREAM_COUNT + 1] = {0};

#if PLATFORM == WIN
#include <Windows.h>
__attribute__((constructor)) static void gen_internal_initialize_vt100emu(void) {
	HANDLE stdout_hnd = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE stderr_hnd = GetStdHandle(STD_ERROR_HANDLE);
	DWORD oldmode;
	GetConsoleMode(stdout_hnd, &oldmode);
	SetConsoleMode(stdout_hnd, oldmode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	GetConsoleMode(stderr_hnd, &oldmode);
	SetConsoleMode(stderr_hnd, oldmode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#endif

__attribute__((constructor)) static void gen_internal_initialize_glog_streams(void) {
	gen_glog_out_streams[0] = stdout;
	gen_glog_err_streams[0] = stderr;
}

GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wformat-nonliteral"
gen_error_t gen_format_to_buffer_len(size_t* const restrict out_size, const char* const restrict format, ...) {
	va_list args;
	va_start(args, format);
	int bytes_written = vsnprintf(NULL, 0, format, args);
	if(bytes_written < 0) return gen_convert_errno(errno);

	*out_size = (size_t) bytes_written;
	return GEN_OK;
}

gen_error_t gen_format_to_buffer(char* const restrict out, const size_t maxchars, const char* const restrict format, ...) {
	va_list args;
	va_start(args, format);
	if(vsnprintf_s(out, maxchars, format, args) < 0) return gen_convert_errno(errno);

	return GEN_OK;
}
GEN_DIAG_REGION_END
