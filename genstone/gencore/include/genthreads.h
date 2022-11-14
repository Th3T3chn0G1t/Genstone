// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

/**
 * @file genthreads.h
 * Includes utilities for threading and synchronization.
 */

#ifndef GEN_THREADS_H
#define GEN_THREADS_H

#include "gencommon.h"

/**
 * Types of message a thread can recieve.
 * The descriptions of these are indicative, exact meanings vary by platform.
 */
typedef enum {
    /**
     * The thread has reached the end of input in a stream.
     */
    GEN_THREADS_MESSAGE_EOF, // SIGHUP SIGPIPE SIGTSTP
    /**
     * The program is being terminated.
     */
    GEN_THREADS_MESSAGE_TERMINATE, // SIGINT SIGQUIT SIGABRT SIGTERM
    /**
     * A fatal error was encountered.
     */
    GEN_THREADS_MESSAGE_FATAL, // SIGILL SIGEMT SIGFPE SIGBUS SIGSEGV SIGSYS SIGXCPU SIGXFSZ
    /**
     * A breakpoint was hit.
     */
    GEN_THREADS_MESSAGE_BREAKPOINT, // SIGTRAP
    /**
     * A timer expired.
     */
    GEN_THREADS_MESSAGE_TIMER, // SIGALRM SIGVTALRM SIGPROF
    /**
     * A stream has pending data.
     */
    GEN_THREADS_MESSAGE_PENDING, // SIGURG SIGIO
    /**
     * A subprocess changed.
     */
    GEN_THREADS_MESSAGE_SUBPROCESS, // SIGCHLD
    /**
     * An application-defined message occurred.
     */
    GEN_THREADS_MESSAGE_APPLICATION, // SIGUSR1 SIGUSR2
    /**
     * An unhandleable/ignored message was recieved.
     * This should never be delivered to the application.
     */
    GEN_THREADS_MESSAGE_UNHANDLED // SIGCONT SIGTTIN SIGTTOU SIGWINCH SIGINFO SIGKILL SIGSTOP
} gen_threads_message_type_t;

/**
 * A message to a thread.
 */
typedef struct {
    /**
     * The type of the message.
     */
    gen_threads_message_type_t type;
    union {
        /**
         * Fatal message data.
         */
        struct {
            /**
             * A backtrace to the origin of the message.
             */
            gen_tooling_backtrace_t backtrace;
            /**
             * The length of the backtrace.
             */
            gen_size_t backtrace_length;
        } fatal;
        /**
         * Breakpoint message data.
         */
        struct {
            /**
             * A backtrace to the origin of the message.
             */
            gen_tooling_backtrace_t backtrace;
            /**
             * The length of the backtrace.
             */
            gen_size_t backtrace_length;
        } breakpoint;
    };
} gen_threads_message_t;

/**
 * Handler for thread messages.
 */
typedef gen_error_t* (*gen_threads_message_handler_t)(const gen_threads_message_t* const restrict message, gen_bool_t* const restrict out_terminate);

typedef struct gen_backends_threads_handle_t gen_backends_threads_handle_t;

/**
 * Handle to a thread.
 */
typedef struct {
    /**
     * The underlying process handle.
     */
    gen_backends_threads_handle_t* native;

    /**
     * A mask of the messages accepted by this thread.
     * This is only for checking accepted messages, for setting the mask see `gen_threads_handle_set_message_mode`.
     */
    GEN_PACKED struct {
        gen_bool_t eof: 1;
        gen_bool_t terminate: 1;
        gen_bool_t fatal: 1;
        gen_bool_t breakpoint: 1;
        gen_bool_t timer: 1;
        gen_bool_t pending: 1;
        gen_bool_t subprocess: 1;
        gen_bool_t application: 1;
    } message_mask;

    /**
     * Handler for messages delivered to this thread.
     */
    gen_threads_message_handler_t message_handler;

    /**
     * Extra data which can be tagged onto the thread for use by the application.
     * This is available in message handlers through `gen_threads_self`.
     */
    void* passthrough;
} gen_threads_handle_t;

/**
 * A handle to the current thread.
 * Only valid for threads initialized by genthreads and the main thread.
 */
extern GEN_THREAD_LOCAL gen_threads_handle_t gen_threads_self; // TODO: Make this atomic

typedef struct gen_backends_threads_mutex_t gen_backends_threads_mutex_t;

/**
 * Lockable synchronization mechanism.
 */
typedef struct {
    /**
     * The underlying mutex handle.
     */
    gen_backends_threads_mutex_t* native;
} gen_threads_mutex_t;

/**
 * Creates a mutex.
 * @param[out] out_mutex A pointer to storage for the created mutex.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_threads_mutex_create(gen_threads_mutex_t* const restrict out_mutex);

/**
 * Destroys a mutex.
 * @param[in,out] mutex The mutex to destroy.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_threads_mutex_destroy(gen_threads_mutex_t* const restrict mutex);

/**
 * Locks a mutex.
 * Blocks until the mutex is unlocked before locking.
 * @param[in,out] mutex The mutex to lock.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_threads_mutex_lock(gen_threads_mutex_t* const restrict mutex);

/**
 * Unlocks a mutex.
 * @param[in,out] mutex The mutex to unlock.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_threads_mutex_unlock(gen_threads_mutex_t* const restrict mutex);

/**
 * Installs process-wide thread message handlers.
 * @note This may affect program state.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_threads_install_message_handlers(void);

/**
 * Sets the message reception mode for a thread.
 * @param[in,out] handle The thread to set the message mode on.
 * @param[in] message The message to set the mode for.
 * @param[in] allow Whether to allow or block the message.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_threads_handle_set_message_mode(gen_threads_handle_t* const restrict handle, const gen_threads_message_type_t message, const gen_bool_t allow);

/**
 * Sets the message handler for a thread.
 * @param[in,out] handle The thread to set the message handler on. 
 * @param[in] handler The message handler to set.
 * @return An error, otherwise `GEN_NULL`. 
 */
extern gen_error_t* gen_threads_handle_set_message_handler(gen_threads_handle_t* const restrict handle, const gen_threads_message_handler_t handler);

#endif
