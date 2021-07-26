#include <gencommon.h>
#include <genesis.h>

static int appticks = 0;
static gen_node_t approot = { GEN_ROOT_TYPE, sizeof(int), &appticks, 0, NULL, NULL };
static mtx_t approot_mtx;
static gen_node_handler_t* nodehandlers;

int thread_func(void* pass) {
    printf("Hello, C11 threads!\n");

    for(size_t i = 0; i < 10; i++) {
        mtx_lock(&approot_mtx);
        ((int*) approot.data)[0]++;
        printf("Thread Ticks: %i\n", ((int*) approot.data)[0]);
        mtx_unlock(&approot_mtx);
    }

    return 0;
}

int main() {
    printf("Hello, Genesis!\n");

    mtx_init(&approot_mtx, mtx_plain);

    thrd_t thread;
    thrd_create(&thread, thread_func, NULL);

    for(size_t i = 0; i < 10; i++) {
        mtx_lock(&approot_mtx);
        ((int*) approot.data)[0]++;
        printf("Main Ticks: %i\n", ((int*) approot.data)[0]);
        mtx_unlock(&approot_mtx);
    }

    int result;
    thrd_join(thread, &result);

    mtx_destroy(&approot_mtx);
}
