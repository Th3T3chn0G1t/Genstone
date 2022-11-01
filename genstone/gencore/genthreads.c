// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genthreads.h"
#include "include/genmemory.h"

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <pthread.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)
#endif

static void gen_threads_internal_mutex_create_cleanup_mutex(gen_threads_mutex_t** mutex) {
    if(!*mutex) return;

    gen_error_t* error = gen_memory_free((void**) mutex);
    if(error) {
        gen_error_print("genthreads", error, GEN_ERROR_SEVERITY_FATAL);
        gen_error_abort();
    }
}

gen_error_t* gen_threads_mutex_create(gen_threads_mutex_t* const restrict out_mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_create, GEN_FILE_NAME);
	if(error) return error;

    if(!out_mutex) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_mutex` was `GEN_NULL`");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
    error = gen_memory_allocate_zeroed((void**) out_mutex, 1, sizeof(pthread_mutex_t));
	if(error) return error;

    GEN_CLEANUP_FUNCTION(gen_threads_internal_mutex_create_cleanup_mutex) gen_threads_mutex_t* mutex_scope_variable = out_mutex;

	int result = pthread_mutex_init(*out_mutex, GEN_NULL);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create mutex: %t", gen_error_description_from_errno());

    mutex_scope_variable = GEN_NULL;
#endif

	return GEN_NULL;
}

gen_error_t* gen_threads_mutex_destroy(gen_threads_mutex_t* const restrict mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_destroy, GEN_FILE_NAME);
	if(error) return error;

    if(!mutex) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`mutex` was `GEN_NULL`");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
	int result = pthread_mutex_destroy(*mutex);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not destroy mutex: %t", gen_error_description_from_errno());

    error = gen_memory_free((void**) mutex);
	if(error) return error;
#endif

	return GEN_NULL;
}

gen_error_t* gen_threads_mutex_create_and_lock(gen_threads_mutex_t* const restrict out_mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_create_and_lock, GEN_FILE_NAME);
	if(error) return error;

	error = gen_threads_mutex_create(out_mutex);
	if(error) return error;

	error = gen_threads_mutex_lock(out_mutex);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_threads_mutex_unlock_and_destroy(gen_threads_mutex_t* const restrict mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_unlock_and_destroy, GEN_FILE_NAME);
	if(error) return error;

	error = gen_threads_mutex_unlock(mutex);
	if(error) return error;

	error = gen_threads_mutex_destroy(mutex);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_threads_mutex_lock(gen_threads_mutex_t* const restrict mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_lock, GEN_FILE_NAME);
	if(error) return error;

    if(!mutex) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`mutex` was `GEN_NULL`");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
	int result = pthread_mutex_lock(*mutex);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not lock mutex: %t", gen_error_description_from_errno());
#endif

	return GEN_NULL;
}

gen_error_t* gen_threads_mutex_unlock(gen_threads_mutex_t* const restrict mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_unlock, GEN_FILE_NAME);
	if(error) return error;

    if(!mutex) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`mutex` was `GEN_NULL`");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
	int result = pthread_mutex_unlock(*mutex);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not lock mutex: %t", gen_error_description_from_errno());
#endif

	return GEN_NULL;
}
