// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

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
#else
/** "ISO C requires a translation unit to contain at least one declaration" */
void gen_internal_glog_dummy(void);
void gen_internal_glog_dummy(void) {}
#endif
