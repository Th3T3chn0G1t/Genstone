#include <gencommon.h>

int thread_func(void* pass) {
    printf("Hello, C11 threads!\n");

    return 0;
}

int main() {
    printf("Hello, Genesis!\n");

    thrd_t thread;
    thrd_create(&thread, thread_func, NULL);

    int result;
    thrd_join(thread, &result);
}
