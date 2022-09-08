// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

/**
 * @file genthreads.h
 * Includes utilities for threading and synchronization.
 */

#ifndef GEN_THREADS_H
#define GEN_THREADS_H

#include "gencommon.h"

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <pthread.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

/**
 * Lockable synchronization mechanism.
 */
typedef pthread_mutex_t gen_threads_mutex_t; 
#else
typedef void* gen_threads_mutex_t;
#endif

#ifndef GEN_THREAD_LOCAL
/**
 * Pretty wrapper for `_Thread_local`.
 */
#define GEN_THREAD_LOCAL _Thread_local
#endif

/**
 * Creates a mutex.
 * @param[out] out_mutex A pointer to storage for the created mutex.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_threads_mutex_create(gen_threads_mutex_t* const restrict out_mutex);

/**
 * Destroys a mutex.
 * @param[in,out] mutex The mutex to destroy.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_threads_mutex_destroy(gen_threads_mutex_t* const restrict mutex);

/**
 * Creates and locks a mutex.
 * @param[out] out_mutex A pointer to storage for the created mutex.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_threads_mutex_create_and_lock(gen_threads_mutex_t* const restrict out_mutex);

/**
 * Unlocks and destroys a mutex.
 * @param[in,out] mutex The mutex to unlock and destroy.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_threads_mutex_unlock_and_destroy(gen_threads_mutex_t* const restrict mutex);

/**
 * Locks a mutex.
 * Blocks until the mutex is unlocked before locking.
 * @param[in,out] mutex The mutex to lock.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_threads_mutex_lock(gen_threads_mutex_t* const restrict mutex);

/**
 * Unlocks a mutex.
 * @param[in,out] mutex The mutex to unlock.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_threads_mutex_unlock(gen_threads_mutex_t* const restrict mutex);

#endif
