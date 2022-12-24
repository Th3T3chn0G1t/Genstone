// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genthreads.h"
#include "include/genmemory.h"

#include <genthreads_be.h>
#include <genbackends.h>

static void gen_threads_internal_mutex_create_cleanup_native(gen_backends_threads_mutex_t** native) {
    if(!*native) return;

    gen_error_t* error = gen_memory_free((void**) native);
    if(error) gen_error_abort_with_error(error, "genthreads");
}

gen_error_t* gen_threads_mutex_create(gen_threads_mutex_t* const restrict out_mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_create, GEN_FILE_NAME);
	if(error) return error;

    if(!out_mutex) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_mutex` was `GEN_NULL`");

    error = gen_memory_allocate_zeroed((void**) &out_mutex->native, 1, sizeof(gen_backends_threads_mutex_t));
	if(error) return error;

    GEN_CLEANUP_FUNCTION(gen_threads_internal_mutex_create_cleanup_native) gen_backends_threads_mutex_t* native_scope_variable = out_mutex->native;

    error = GEN_BACKENDS_CALL(threads_mutex_create)(out_mutex);
	if(error) return error;

    native_scope_variable = GEN_NULL;

	return GEN_NULL;
}

gen_error_t* gen_threads_mutex_destroy(gen_threads_mutex_t* const restrict mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_destroy, GEN_FILE_NAME);
	if(error) return error;

    if(!mutex) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`mutex` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(threads_mutex_destroy)(mutex);
	if(error) return error;

    error = gen_memory_free((void**) &mutex->native);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_threads_mutex_lock(gen_threads_mutex_t* const restrict mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_lock, GEN_FILE_NAME);
	if(error) return error;

    if(!mutex) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`mutex` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(threads_mutex_lock)(mutex);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_threads_mutex_unlock(gen_threads_mutex_t* const restrict mutex) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_unlock, GEN_FILE_NAME);
	if(error) return error;

    if(!mutex) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`mutex` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(threads_mutex_unlock)(mutex);
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_threads_install_message_handlers(void) {
 	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_handle_set_message_handler, GEN_FILE_NAME);
	if(error) return error;

    error = GEN_BACKENDS_CALL(threads_install_message_handlers)();
	if(error) return error;

	return GEN_NULL;
}

gen_error_t* gen_threads_handle_set_message_mode(gen_threads_handle_t* const restrict handle, const gen_threads_message_type_t message, const gen_bool_t allow) {
 	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_lock, GEN_FILE_NAME);
	if(error) return error;

    if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");

    error = GEN_BACKENDS_CALL(threads_handle_set_message_mode)(handle, message, allow);
	if(error) return error;
   
	return GEN_NULL;
}

gen_error_t* gen_threads_handle_set_message_handler(gen_threads_handle_t* const restrict handle, const gen_threads_message_handler_t handler) {
 	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_handle_set_message_handler, GEN_FILE_NAME);
	if(error) return error;

    if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");

    handle->message_handler = handler;

	return GEN_NULL;
}
