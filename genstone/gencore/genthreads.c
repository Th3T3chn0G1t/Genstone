// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genthreads.h"

gen_error_t* gen_threads_mutex_create(gen_threads_mutex_t* const restrict out_mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_create, GEN_FILE_NAME);
	if(error) return error;

	int result = pthread_mutex_init(out_mutex, NULL);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not create mutex: %t", gen_error_description_from_errno());

	return NULL;
}

gen_error_t* gen_threads_mutex_destroy(gen_threads_mutex_t* const restrict mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_destroy, GEN_FILE_NAME);
	if(error) return error;

	int result = pthread_mutex_destroy(mutex);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not destroy mutex: %t", gen_error_description_from_errno());

	return NULL;
}

gen_error_t* gen_threads_mutex_create_and_lock(gen_threads_mutex_t* const restrict out_mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_create_and_lock, GEN_FILE_NAME);
	if(error) return error;

	error = gen_threads_mutex_create(out_mutex);
	if(error) return error;

	error = gen_threads_mutex_lock(out_mutex);
	if(error) return error;

	return NULL;
}

gen_error_t* gen_threads_mutex_unlock_and_destroy(gen_threads_mutex_t* const restrict mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_unlock_and_destroy, GEN_FILE_NAME);
	if(error) return error;

	error = gen_threads_mutex_unlock(mutex);
	if(error) return error;

	error = gen_threads_mutex_destroy(mutex);
	if(error) return error;

	return NULL;
}

gen_error_t* gen_threads_mutex_lock(gen_threads_mutex_t* const restrict mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_lock, GEN_FILE_NAME);
	if(error) return error;

	int result = pthread_mutex_lock(mutex);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not lock mutex: %t", gen_error_description_from_errno());

	return NULL;
}

gen_error_t* gen_threads_mutex_unlock(gen_threads_mutex_t* const restrict mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_unlock, GEN_FILE_NAME);
	if(error) return error;

	int result = pthread_mutex_unlock(mutex);
	if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not lock mutex: %t", gen_error_description_from_errno());

	return NULL;
}
