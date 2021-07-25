#include <stdio.h>

#ifdef _WIN32
#define API __declspec(dllexport)
#else
#define API
#endif

API void foo(void) {
    printf("foo\n");
    return;
}
