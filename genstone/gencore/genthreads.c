// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genthreads.h"
#include "include/genmemory.h"

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#include <pthread.h>
#include <signal.h>
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)
#endif

GEN_THREAD_LOCAL gen_threads_handle_t gen_threads_self = {0};

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED

static gen_error_t* gen_threads_internal_initialize_thread_message_modes(gen_threads_handle_t* const restrict handle) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_internal_initialize_thread_message_modes, GEN_FILE_NAME);
    if(error) return error;

    error = gen_threads_handle_set_message_mode(handle, GEN_THREADS_MESSAGE_EOF, gen_true);
	if(error) return error;
    error = gen_threads_handle_set_message_mode(handle, GEN_THREADS_MESSAGE_TERMINATE, gen_true);
	if(error) return error;
    error = gen_threads_handle_set_message_mode(handle, GEN_THREADS_MESSAGE_FATAL, gen_true);
	if(error) return error;
    error = gen_threads_handle_set_message_mode(handle, GEN_THREADS_MESSAGE_BREAKPOINT, gen_true);
	if(error) return error;
    error = gen_threads_handle_set_message_mode(handle, GEN_THREADS_MESSAGE_TIMER, gen_true);
	if(error) return error;
    error = gen_threads_handle_set_message_mode(handle, GEN_THREADS_MESSAGE_PENDING, gen_true);
	if(error) return error;
    error = gen_threads_handle_set_message_mode(handle, GEN_THREADS_MESSAGE_SUBPROCESS, gen_true);
	if(error) return error;
    error = gen_threads_handle_set_message_mode(handle, GEN_THREADS_MESSAGE_APPLICATION, gen_true);
	if(error) return error;

    return GEN_NULL;
}

#endif

GEN_INITIALIZER static void gen_threads_internal_initialize_main_thread_self(void) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_internal_initialize_main_thread_self, GEN_FILE_NAME);
    if(error) {
        gen_error_print("genthreads", error, GEN_ERROR_SEVERITY_FATAL);
        gen_error_abort();
    }
    
    error = gen_memory_allocate_zeroed((void**) &gen_threads_self.native, 1, sizeof(pthread_t));
    if(error) {
        gen_error_print("genthreads", error, GEN_ERROR_SEVERITY_FATAL);
        gen_error_abort();
    }

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
    *(pthread_t*) gen_threads_self.native = pthread_self();
#endif

    error = gen_threads_internal_initialize_thread_message_modes(&gen_threads_self);
    if(error) {
        gen_error_print("genthreads", error, GEN_ERROR_SEVERITY_FATAL);
        gen_error_abort();
    }
}

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

gen_error_t* gen_threads_handle_set_message_mode(gen_threads_handle_t* const restrict handle, const gen_threads_message_type_t message, const gen_bool_t allow) {
 	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_mutex_lock, GEN_FILE_NAME);
	if(error) return error;

    if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED

    sigset_t set = {0};
    int result = sigemptyset(&set);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not set thread message mode: %t", gen_error_description_from_errno());

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Wsign-conversion"))
    switch(message) {
        case GEN_THREADS_MESSAGE_EOF: {
            handle->message_mask.eof = allow;

            result = sigaddset(&set, SIGHUP);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGPIPE);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGTSTP);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());

            break;
        }
        case GEN_THREADS_MESSAGE_TERMINATE: {
            handle->message_mask.terminate = allow;

            result = sigaddset(&set, SIGINT);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGQUIT);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGABRT);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            // result = sigaddset(&set, SIGKILL);
            // if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGTERM);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            // result = sigaddset(&set, SIGSTOP);
            // if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());

            break;
        }
        case GEN_THREADS_MESSAGE_FATAL: {
            handle->message_mask.fatal = allow;

            result = sigaddset(&set, SIGILL);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGEMT);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGFPE);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGBUS);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGSEGV);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGSYS);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGXCPU);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGXFSZ);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());

            break;
        }
        case GEN_THREADS_MESSAGE_BREAKPOINT: {
            handle->message_mask.breakpoint = allow;

            result = sigaddset(&set, SIGTRAP);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());

            break;
        }
        case GEN_THREADS_MESSAGE_TIMER: {
            handle->message_mask.timer = allow;

            result = sigaddset(&set, SIGALRM);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGVTALRM);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGPROF);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());

            break;
        }
        case GEN_THREADS_MESSAGE_PENDING: {
            handle->message_mask.pending = allow;

            result = sigaddset(&set, SIGURG);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            else if(result == 1) handle->message_mask.pending = gen_true;
            result = sigaddset(&set, SIGIO);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            else if(result == 1) handle->message_mask.pending = gen_true;

            break;
        }
        case GEN_THREADS_MESSAGE_SUBPROCESS: {
            handle->message_mask.subprocess = allow;

            result = sigaddset(&set, SIGCHLD);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());

            break;
        }
        case GEN_THREADS_MESSAGE_APPLICATION: {
            handle->message_mask.application = allow;

            result = sigaddset(&set, SIGUSR1);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());
            result = sigaddset(&set, SIGUSR2);
            if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not populate thread message mask: %t", gen_error_description_from_errno());

            break;
        }
        case GEN_THREADS_MESSAGE_UNHANDLED: break;
    }
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

    result = pthread_sigmask(allow ? SIG_UNBLOCK : SIG_BLOCK, &set, GEN_NULL);
    if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not set thread message mode: %t", gen_error_description_from_errno());

#endif
   
	return GEN_NULL;
}

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
static void gen_threads_internal_signal_handler(int signal, GEN_UNUSED siginfo_t* siginfo, GEN_UNUSED void* ucontext) {
 	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_internal_signal_handler, GEN_FILE_NAME);
	if(error) {
        gen_error_print("genthreads", error, GEN_ERROR_SEVERITY_FATAL);
        gen_error_abort();
    }

    gen_threads_message_type_t type = GEN_THREADS_MESSAGE_UNHANDLED;
    switch(signal) {
        case SIGHUP: GEN_FALLTHROUGH;
        case SIGPIPE: GEN_FALLTHROUGH;
        case SIGTSTP: {
            type = GEN_THREADS_MESSAGE_EOF;
            break;
        }
        case SIGINT: GEN_FALLTHROUGH;
        case SIGQUIT: GEN_FALLTHROUGH;
        case SIGABRT: GEN_FALLTHROUGH;
        case SIGTERM: {
            type = GEN_THREADS_MESSAGE_TERMINATE;
            break;
        }
        case SIGILL: GEN_FALLTHROUGH;
        case SIGEMT: GEN_FALLTHROUGH;
        case SIGFPE: GEN_FALLTHROUGH;
        case SIGBUS: GEN_FALLTHROUGH;
        case SIGSEGV: GEN_FALLTHROUGH;
        case SIGSYS: GEN_FALLTHROUGH;
        case SIGXCPU: GEN_FALLTHROUGH;
        case SIGXFSZ: {
            type = GEN_THREADS_MESSAGE_FATAL;
            break;
        }

        case SIGTRAP: {
            type = GEN_THREADS_MESSAGE_BREAKPOINT;
            break;
        }

        case SIGALRM: GEN_FALLTHROUGH;
        case SIGVTALRM: GEN_FALLTHROUGH;
        case SIGPROF: {
            type = GEN_THREADS_MESSAGE_TIMER;
            break;
        }

        case SIGURG: GEN_FALLTHROUGH;
        case SIGIO: {
            type = GEN_THREADS_MESSAGE_PENDING;
            break;
        }

        case SIGCHLD: {
            type = GEN_THREADS_MESSAGE_SUBPROCESS;
            break;
        }

        case SIGUSR1: GEN_FALLTHROUGH;
        case SIGUSR2: {
            type = GEN_THREADS_MESSAGE_APPLICATION;
            break;
        }

        case SIGCONT: GEN_FALLTHROUGH;
        case SIGTTIN: GEN_FALLTHROUGH;
        case SIGTTOU: GEN_FALLTHROUGH;
        case SIGWINCH: GEN_FALLTHROUGH;
        case SIGINFO: GEN_FALLTHROUGH;
        case SIGKILL: GEN_FALLTHROUGH;
        case SIGSTOP: break;

        default: {
            error = gen_error_attach_backtrace_formatted(GEN_ERROR_INVALID_CONTROL, GEN_LINE_NUMBER, "Reached unknown signal case `%si`", signal);
            gen_error_print("genthreads", error, GEN_ERROR_SEVERITY_FATAL);
            gen_error_abort();
        }
    }

    if(type == GEN_THREADS_MESSAGE_UNHANDLED) return;

    if(gen_threads_self.message_handler) {
/*
    // This should be handled by the thread mask already so these checks are unneccesary.
 && (
           (type == GEN_THREADS_MESSAGE_EOF && gen_threads_self.message_mask.eof)
        || (type == GEN_THREADS_MESSAGE_TERMINATE && gen_threads_self.message_mask.terminate)
        || (type == GEN_THREADS_MESSAGE_FATAL && gen_threads_self.message_mask.fatal)
        || (type == GEN_THREADS_MESSAGE_BREAKPOINT && gen_threads_self.message_mask.breakpoint)
        || (type == GEN_THREADS_MESSAGE_TIMER && gen_threads_self.message_mask.timer)
        || (type == GEN_THREADS_MESSAGE_PENDING && gen_threads_self.message_mask.pending)
        || (type == GEN_THREADS_MESSAGE_SUBPROCESS && gen_threads_self.message_mask.subprocess)
        || (type == GEN_THREADS_MESSAGE_APPLICATION && gen_threads_self.message_mask.application))
*/
        gen_threads_message_t message = {type, {0}};

        gen_tooling_backtrace_t* populate_backtrace = GEN_NULL;
        gen_size_t* populate_backtrace_length = GEN_NULL;

        if(type == GEN_THREADS_MESSAGE_FATAL) {
            populate_backtrace = &message.fatal.backtrace;
            populate_backtrace_length = &message.fatal.backtrace_length;
        }
        if(type == GEN_THREADS_MESSAGE_BREAKPOINT) {
            populate_backtrace = &message.breakpoint.backtrace;
            populate_backtrace_length = &message.breakpoint.backtrace_length;
        }

        if(populate_backtrace) {
            gen_size_t length = 0;
            error = gen_tooling_get_backtrace(*populate_backtrace, &length);
            if(error) {
                gen_error_print("genthreads", error, GEN_ERROR_SEVERITY_FATAL);
                gen_error_abort();
            }

            *populate_backtrace_length = length - 1; // Elide the frame for this handler.
        }

        gen_bool_t terminate = gen_false;
        if(type == GEN_THREADS_MESSAGE_FATAL || type == GEN_THREADS_MESSAGE_TERMINATE) terminate = gen_true;
        error = gen_threads_self.message_handler(&message, &terminate);
        if(error) {
            gen_error_print("genthreads", error, GEN_ERROR_SEVERITY_FATAL);
            gen_error_abort();
        }
        if(terminate) pthread_exit(GEN_NULL);
    }
    else if(type == GEN_THREADS_MESSAGE_FATAL || type == GEN_THREADS_MESSAGE_TERMINATE) pthread_exit(GEN_NULL);
}
#endif

gen_error_t* gen_threads_install_message_handlers(void) {
 	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_handle_set_message_handler, GEN_FILE_NAME);
	if(error) return error;

#if GEN_PLATFORM == GEN_LINUX || GEN_PLATFORM == GEN_OSX || GEN_FORCE_UNIX == GEN_ENABLED
    for(int i = 1; i < NSIG; ++i) {
        // [EINVAL] will also be
        // generated if an attempt is made to change the action for
        // SIGKILL or SIGSTOP, which cannot be caught or ignored.
        if(i == SIGKILL || i == SIGSTOP) continue;

        sigset_t set = {0};
        int result = sigemptyset(&set); 
        if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not install program-level message handlers: %t", gen_error_description_from_errno());

        struct sigaction action = {0};
        action.sa_sigaction = gen_threads_internal_signal_handler;
        action.sa_mask = set;
        action.sa_flags = SA_SIGINFO;

        struct sigaction old = {0};
        result = sigaction(i, &action, &old);
        if(result == -1) return gen_error_attach_backtrace_formatted(gen_error_type_from_errno(), GEN_LINE_NUMBER, "Could not install program-level message handlers: %t", gen_error_description_from_errno());
    }
#endif

	return GEN_NULL;
}

gen_error_t* gen_threads_handle_set_message_handler(gen_threads_handle_t* const restrict handle, const gen_threads_message_handler_t handler) {
 	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_threads_handle_set_message_handler, GEN_FILE_NAME);
	if(error) return error;

    if(!handle) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`handle` was `GEN_NULL`");

    handle->message_handler = handler;

	return GEN_NULL;
}
