// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

/**
 * @file genfilesystem.h
 * Includes filesystem utilities for manipulation and introspection.
 */

#ifndef GEN_FILESYSTEM_H
#define GEN_FILESYSTEM_H

#include "gencommon.h"
#include "genthreads.h"

/**
 * Bitmasks for file watcher event reporting.
 */
typedef enum
{
    /**
     * No event occurred.
     */
    GEN_FILESYSTEM_WATCHER_EVENT_NONE = 1 << 0,

    /**
     * A file was created.
     */
    GEN_FILESYSTEM_WATCHER_EVENT_CREATED = 1 << 1,

    /**
     * A file was modified.
     */
    GEN_FILESYSTEM_WATCHER_EVENT_MODIFIED = 1 << 2,

    /**
     * A file was deleted.
     */
    GEN_FILESYSTEM_WATCHER_EVENT_DELETED = 1 << 3,

    /**
     * A file was moved.
     */
    GEN_FILESYSTEM_WATCHER_EVENT_MOVED = 1 << 4
} gen_filesystem_watcher_event_t;

/**
 * The type of filesystem entry a filesystem handle refers to.
 */
typedef enum {
    /**
     * A regular file.
     */
    GEN_FILESYSTEM_HANDLE_FILE,

    /**
     * A directory.
     */
    GEN_FILESYSTEM_HANDLE_DIRECTORY,

    /**
     * An anonymous file.
     */
    GEN_FILESYSTEM_HANDLE_ANONYMOUS
} gen_filesystem_handle_type_t;

typedef struct gen_backends_filesystem_handle_t gen_backends_filesystem_handle_t;

// TODO: Filesystem context struct w/ settings

/**
 * A handle to a filesystem entry.
 */
typedef struct {
    /**
     * The type of entry this handle references.
     */
    gen_filesystem_handle_type_t type;

    /**
     * Structure access and operation lock.
     */
    gen_threads_mutex_t lock;

    /**
     * The underlying filesystem handle.
     */
    gen_backends_filesystem_handle_t* native;
} gen_filesystem_handle_t;

typedef struct gen_backends_filesystem_watcher_handle_t gen_backends_filesystem_watcher_handle_t;

/**
 * Handle to a filesystem watcher.
 */
typedef struct {
    /**
     * The underlying filesystem watcher handle.
     */
    gen_backends_filesystem_watcher_handle_t* native;
} gen_filesystem_watcher_t;

/**
 * ID for a handle being watched by a filesystem watcher.
 */
typedef gen_size_t gen_filesystem_watcher_id_t;

/**
 * ID output by `gen_filesystem_watcher_poll` when no more events are available.
 */
#define GEN_FILESYSTEM_WATCHER_ID_NONE GEN_SIZE_MAX

/**
 * A filesystem handle for the standard input stream.
 */
extern gen_filesystem_handle_t gen_filesystem_standard_input;
/**
 * A filesystem handle for the standard output stream.
 */
extern gen_filesystem_handle_t gen_filesystem_standard_output;
/**
 * A filesystem handle for the standard error stream.
 */
extern gen_filesystem_handle_t gen_filesystem_standard_error;

/**
 * Gets the canonical representation of a path. A filesystem entry must exist at path.
 * @param[in] path The path to get a canonical representation for.
 * @param[in] path_bounds The bounds of `path`.
 * @param[in] path_length The length of `path`.
 * @param[out] out_canonical A pointer to storage for the canonicalized path.
 * @param[out] out_length A pointer to storage for the length of the canonicalized path.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_path_canonicalize(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length, char* const restrict out_canonical, gen_size_t* const restrict out_length);

/**
 * Checks whether a filesystem entry exists at a path.
 * @param[in] path The path to check.
 * @param[in] path_bounds The bounds of `path`.
 * @param[in] path_length The length of `path`.
 * @param[out] out_exists A pointer to storage for the result of whether a filesystem entry exists at `path`.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_path_exists(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length, gen_bool_t* const restrict out_exists);

/**
 * Checks whether a path is valid.
 * @param[in] path The path to validate.
 * @param[in] path_bounds The bounds of `path`.
 * @param[in] path_length The length of `path`.
 * @param[out] out_valid A pointer to storage for the result of whether `path` is valid.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_path_validate(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length, gen_bool_t* const restrict out_valid);

/**
 * Creates a file at a path.
 * @param[in] path The path to create a file at.
 * @param[in] path_bounds The bounds of `path`.
 * @param[in] path_length The length of `path`.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_path_create_file(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length);

/**
 * Creates a directory at a path.
 * @param[in] path The path to create a directory at.
 * @param[in] path_bounds The bounds of `path`.
 * @param[in] path_length The length of `path`.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_path_create_directory(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length);

/**
 * Deletes a filesystem entry at a path.
 * @param[in] path The path to the entry to destroy.
 * @param[in] path_bounds The bounds of `path`.
 * @param[in] path_length The length of `path`.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_path_delete(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length);

/**
 * Opens an anonymous file as a filesystem handle.
 * @param[out] out_handle A pointer to storage for the opened handle.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_handle_open_anonymous(gen_filesystem_handle_t* const restrict out_handle);

/**
 * Opens a path as a filesystem handle.
 * @param[in] path The path to open.
 * @param[in] path_bounds The bounds of `path`.
 * @param[in] path_length The length of `path`.
 * @param[out] out_handle A pointer to storage for the opened handle.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_handle_open(const char* const restrict path, const gen_size_t path_bounds, const gen_size_t path_length, gen_filesystem_handle_t* const restrict out_handle);

/**
 * Closes a filesystem handle.
 * @param[in,out] handle The handle to close.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_handle_close(gen_filesystem_handle_t* const restrict handle);

/**
 * Gets the size of a file from a filesystem handle.
 * @param[in] handle The handle to the file to get the size of.
 * @param[out] out_size A pointer to storage for the file size.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_handle_file_size(gen_filesystem_handle_t* const restrict handle, gen_size_t* const restrict out_size);

// TODO: R/W retry timeouts

/**
 * Reads a file's contents from a filesystem handle.
 * @param[in] handle The handle to the file to content to read from.
 * @param[in] offset An offset to start reading from.
 * @param[in] length The number of bytes to read.
 * @param[out] out_buffer A pointer to storage for the file's contents.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_handle_file_read(gen_filesystem_handle_t* const restrict handle, const gen_size_t offset, const gen_size_t length, unsigned char* restrict out_buffer);

/**
 * Writes to a file through a filesystem handle.
 * @param[in] handle The handle to the file to write to.
 * @param[in] offset The offset to starting writing at.
 * @param[in] buffer The buffer to write.
 * @param[in] length The number of bytes to write.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_handle_file_write(gen_filesystem_handle_t* const restrict handle, const gen_size_t offset, const unsigned char* const restrict buffer, const gen_size_t length);

/**
 * Lists the contents of a directory from a filesystem handle.
 * @param[in] handle The handle to the directory to list from.
 * @param[out] out_list A pointer to pointers to storage for the directory entries.
 * @param[out] out_lengths A pointer to storage for the lengths for the directory entries.
 * @param[out] out_length A pointer to storage for the length of the list.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_handle_directory_list(gen_filesystem_handle_t* const restrict handle, char* restrict * const restrict out_list, gen_size_t* const restrict out_lengths, gen_size_t* const restrict out_length);

/**
 * Locks a filesystem handle and it's filesystem entry.
 * @param[in] handle The handle to lock.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_handle_lock(gen_filesystem_handle_t* const restrict handle);

/**
 * Unlocks a filesystem handle and it's filesystem entry.
 * @param[in] handle The handle to unlock.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_handle_unlock(gen_filesystem_handle_t* const restrict handle);

/**
 * Creates a filesystem watcher for a handle.
 * @param[out] out_watcher A pointer to storage for the created watcher.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_watcher_create(gen_filesystem_watcher_t* const restrict out_watcher);

/**
 * Destroys an filesystem watcher.
 * @param[in,out] watcher The watcher to destroy.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_watcher_destroy(gen_filesystem_watcher_t* const restrict watcher);

/**
 * Adds a filesystem handle to be watched by a filesystem watcher.
 * @param[in,out] watcher The watcher to add to.
 * @param[in,out] handle The handle to watch.
 * @param[out] out_id The ID for the watched handle in the watcher.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_watcher_add(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_handle_t* const restrict handle, gen_filesystem_watcher_id_t* const restrict out_id);

/**
 * Removes a filesystem handle from a filesystem watcher.
 * @param[in,out] watcher The watcher to remove from.
 * @param[in] id The ID of the handle to remove.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_watcher_remove(gen_filesystem_watcher_t* const restrict watcher, const gen_filesystem_watcher_id_t id);

/**
 * Checks an filesystem watcher for events.
 * Returns the event mask for a single file, should be called until ID is `GEN_FILESYSTEM_WATCHER_ID_NONE`.
 * @param[in] watcher The watcher to poll.
 * @param[out] out_event A pointer to storage for events which have occurred.
 * @param[out] out_id A pointer to storage for the ID on which the events occurred.
 * @return An error, otherwise `GEN_NULL`.
 */
extern gen_error_t* gen_filesystem_watcher_poll(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_watcher_event_t* const restrict out_event, gen_filesystem_watcher_id_t* const restrict out_id);

#endif
