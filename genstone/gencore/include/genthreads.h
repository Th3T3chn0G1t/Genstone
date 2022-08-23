// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genthreads.h
 * Includes utilities for threading and synchronization.
 */

#ifndef GEN_THREADS_H
#define GEN_THREADS_H

#include "gencommon.h"

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#if GEN_PLATFORM != GEN_WINDOWS
#include <pthread.h>
#endif
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

/**
 * Lockable synchronization mechanism.
 */
typedef pthread_mutex_t gen_threads_mutex_t; 

/**
 * Pretty wrapper for `_Thread_local`.
 */
#define GEN_THREAD_LOCAL _Thread_local

extern void gen_threads_internal_mutex_scoped_lock_cleanup(gen_threads_mutex_t** mutex);

/**
 * Locks a mutex for the current scope.
 * @param[in,out] mutex The mutex to lock.
 * @returns An error code.
 */
#define GEN_THREADS_MUTEX_SCOPED_LOCK(mutex) \
    gen_threads_mutex_lock(mutex); \
    GEN_MAYBE_UNUSED GEN_CLEANUP_FUNCTION(gen_threads_internal_mutex_scoped_lock_cleanup) gen_threads_mutex_t* gen_threads_internal_mutex_scoped_lock_scope_variable = mutex

/**
 * Creates a mutex.
 * @param[out] out_mutex A pointer to storage for the created mutex.
 * @return An error code.
 */
extern gen_error_t gen_threads_mutex_create(gen_threads_mutex_t* const restrict out_mutex);

/**
 * Destroys a mutex.
 * @param[in,out] mutex The mutex to destroy.
 * @return An error code.
 */
extern gen_error_t gen_threads_mutex_destroy(gen_threads_mutex_t* const restrict mutex);

/**
 * Creates and locks a mutex.
 * @param[out] out_mutex A pointer to storage for the created mutex.
 * @return An error code.
 */
extern gen_error_t gen_threads_mutex_create_and_lock(gen_threads_mutex_t* const restrict out_mutex);

/**
 * Unlocks and destroys a mutex.
 * @param[in,out] mutex The mutex to unlock and destroy.
 * @return An error code.
 */
extern gen_error_t gen_threads_mutex_unlock_and_destroy(gen_threads_mutex_t* const restrict mutex);

/**
 * Locks a mutex.
 * Blocks until the mutex is unlocked before locking.
 * @param[in,out] mutex The mutex to lock.
 * @return An error code.
 */
extern gen_error_t gen_threads_mutex_lock(gen_threads_mutex_t* const restrict mutex);

/**
 * Unlocks a mutex.
 * @param[in,out] mutex The mutex to unlock.
 * @return An error code.
 */
extern gen_error_t gen_threads_mutex_unlock(gen_threads_mutex_t* const restrict mutex);

#endif
