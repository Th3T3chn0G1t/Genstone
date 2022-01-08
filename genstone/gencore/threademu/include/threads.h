// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>
// Modified from Mesa threads emulation library
// See source for licence information of original

#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>

#define thread_local _Thread_local

typedef pthread_cond_t cnd_t;
typedef pthread_t thrd_t;
typedef pthread_key_t tss_t;
typedef pthread_mutex_t mtx_t;
typedef pthread_once_t once_flag;

typedef void (*tss_dtor_t)(void*);
typedef int (*thrd_start_t)(void*);

struct impl_thrd_param {
    thrd_start_t func;
    void* arg;
};

enum { mtx_plain, mtx_try, mtx_timed, mtx_recursive };
enum { thrd_success, thrd_timeout, thrd_error, thrd_busy, thrd_nomem };

#define TSS_DTOR_ITERATIONS PTHREAD_DESTRUCTOR_ITERATIONS

extern void call_once(once_flag* flag, void (*func)(void));

extern int cnd_broadcast(cnd_t* cond);
extern void cnd_destroy(cnd_t* cond);
extern int cnd_init(cnd_t* cond);
extern int cnd_signal(cnd_t* cond);
extern int cnd_timedwait(cnd_t* cond, mtx_t* mtx, const struct timespec* abs_time);
extern int cnd_wait(cnd_t* cond, mtx_t* mtx);

extern void mtx_destroy(mtx_t* mtx);
extern int mtx_init(mtx_t* mtx, int type);
extern int mtx_lock(mtx_t* mtx);
extern int mtx_trylock(mtx_t* mtx);
extern int mtx_timedlock(mtx_t* mtx, const struct timespec* ts);
extern int mtx_unlock(mtx_t* mtx);

extern void* impl_thrd_routine(void* p);
extern int thrd_create(thrd_t* thr, thrd_start_t func, void* arg);
extern thrd_t thrd_current(void);
extern int thrd_detach(thrd_t thr);
extern int thrd_equal(thrd_t thr0, thrd_t thr1);
extern void thrd_exit(int res);
extern int thrd_join(thrd_t thr, int* res);
extern void thrd_sleep(const struct timespec* time_point, struct timespec* remaining);
extern void thrd_yield(void);

extern int tss_create(tss_t* key, tss_dtor_t dtor);
extern void tss_delete(tss_t key);
extern void* tss_get(tss_t key);
extern int tss_set(tss_t key, void* val);

#endif
