#if PLATFORM == WIN
#include <Windows.h>
__attribute__((constructor))
static void gen_internal_initialize_vt100emu(void) {
    HANDLE stdout_hnd = GetStdHandle(STD_OUTPUT_HANDLE);
    int oldmode;
    GetConsoleMode(stdout_hnd, &oldmode)
    SetConsoleMode(stdout_hnd, oldmode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
#endif
