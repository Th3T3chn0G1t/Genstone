#include <stdio.h>

#ifdef _WIN32
#define API(t) __declspec(dllexport ) t __cdecl
#else
#define API(t) t
#endif

API(void) foo(void) {
    printf("foo\n");
    return;
}
