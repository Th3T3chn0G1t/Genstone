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

GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_BEGIN)
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_IGNORE("-Weverything"))
#if GEN_PLATFORM != GEN_WINDOWS
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#endif
GEN_PRAGMA(GEN_PRAGMA_DIAGNOSTIC_REGION_END)

#ifndef GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY
#if GEN_PLATFORM == GEN_LINUX
/**
 * Whether to use the system library where implemented to get file watcher functionality.
 * @note Disabling may make results more consistent across platforms as the alternative uses UNIX standard utilities.
 */
#define GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY GEN_ENABLED
#else
#define GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY GEN_DISABLED
#endif
#endif

/**
 * The maximum size of a directory listing.
 */
#define GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX (sizeof(((struct dirent*) NULL)->d_name))

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
 * The native file handle type.
 */
typedef int gen_filesystem_file_handle_t;

/**
 * The native directory handle type.
 */
typedef DIR* gen_filesystem_directory_handle_t;

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
     * A watcher.
     */
    GEN_FILESYSTEM_HANDLE_WATCHER
} gen_filesystem_handle_type_t;

/**
 * A handle to a filesystem entry.
 */
typedef struct {
    /**
     * The type of entry this handle references.
     */
    gen_filesystem_handle_type_t type;

    /**
     * The underlying file handle for the handle.
     */
    gen_filesystem_file_handle_t file_handle;

    /**
     * The underlying directory handle for the handle.
     * Invalid if `type` is not `GEN_FILESYSTEM_HANDLE_DIRECTORY`.
     */
    gen_filesystem_directory_handle_t directory_handle;

    /**
     * Structure access and operation lock.
     */
    gen_threads_mutex_t lock; // TODO: Switch directory listing to `getdents` and remove need for lock/move into non-syslib watching.

#if GEN_FILESYSTEM_WATCHER_USE_SYSTEM_LIBRARY == GEN_DISABLED
    /**
     * Internal caching of `stat` for file watching.
     */
    struct stat internal_stat_cached;

    /**
     * Intenal caching of directory length for file watching.
     */
    size_t internal_directory_length_cached;
#endif
} gen_filesystem_handle_t;

/**
 * Handle to a filesystem watcher.
 */
typedef gen_filesystem_handle_t gen_filesystem_watcher_t;

/**
 * `gen_filesystem_handle_t` wrapper for stdin.
 */
#define GEN_FILESYSTEM_HANDLE_STDIN ((gen_filesystem_handle_t){GEN_FILESYSTEM_HANDLE_FILE, STDIN_FILENO, NULL})

/**
 * `gen_filesystem_handle_t` wrapper for stdout.
 */
#define GEN_FILESYSTEM_HANDLE_STDOUT ((gen_filesystem_handle_t){GEN_FILESYSTEM_HANDLE_FILE, STDOUT_FILENO, NULL})

/**
 * `gen_filesystem_handle_t` wrapper for stderr.
 */
#define GEN_FILESYSTEM_HANDLE_STDERR ((gen_filesystem_handle_t){GEN_FILESYSTEM_HANDLE_FILE, STDERR_FILENO, NULL})

/**
 * Gets the canonical representation of a path.
 * @param[in] path The path to get a canonical representation for.
 * @param[in] path_length The length of the path to get a canonical representation for.
 * @param[out] out_canonical A pointer to storage for the canonicalized path.
 * @param[out] out_length A pointer to storage for the length of the canonicalized path.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_path_canonicalize(const char* const restrict path, const size_t path_length, char* restrict out_canonical, size_t* const restrict out_length);

/**
 * Returns whether a filesystem entry exists at a path.
 * @param[in] path The path to check.
 * @param[in] path_length The length of the path to check.
 * @param[out] out_exists A pointer to storage for the result of whether a filesystem entry exists at `path`.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_path_exists(const char* const restrict path, const size_t path_length, bool* const restrict out_exists);

/**
 * Checks whether a path is valid.
 * @param[in] path The path to validate.
 * @param[in] path_length The length of the path to validate.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_path_validate(const char* const restrict path, const size_t path_length);

/**
 * Creates a file at a path.
 * @param[in] path The path to create a file at.
 * @param[in] path_length The length of the path to create a file at.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_path_create_file(const char* const restrict path, const size_t path_length);

/**
 * Creates a directory at a path.
 * @param[in] path The path to create a directory at.
 * @param[in] path_length The length of the path to create a directory at.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_path_create_directory(const char* const restrict path, const size_t path_length);

/**
 * Deletes a filesystem entry at a path.
 * @param[in] path The path to the entry to destroy.
 * @param[in] path_length The length of the path to the entry to destroy.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_path_delete(const char* const restrict path, const size_t path_length);

/**
 * Opens a path as a filesystem handle for use by filesystem operations.
 * @param[in] path The path to open.
 * @param[in] path_length The length of the path to open.
 * @param[out] out_handle A pointer to storage for the handle.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_handle_open(const char* const restrict path, const size_t path_length, gen_filesystem_handle_t* restrict out_handle);

/**
 * Opens an anonymous file as a filesystem handle for use by filesystem operations.
 * @param[out] out_handle A pointer to storage for the handle.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_handle_open_anonymous(gen_filesystem_handle_t* restrict out_handle);

/**
 * Closes a filesystem handle.
 * @param[in,out] handle The handle to close.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_handle_close(gen_filesystem_handle_t* const restrict handle);

/**
 * Gets the size of a file.
 * @param[in] handle The handle whose size to get.
 * @param[out] out_size A pointer to storage for the size of the handle.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_handle_file_size(gen_filesystem_handle_t* const restrict handle, size_t* const restrict out_size);

/**
 * Reads a file's content.
 * @param[in] handle The handle whose content to read.
 * @param[in] start An offset from the start of the file to begin reading from.
 * @param[in] end An offset from the start of the file to end reading at.
 * @param[out] out_buffer A pointer to storage for the read bytes.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_handle_file_read(gen_filesystem_handle_t* const restrict handle, const size_t start, const size_t end, unsigned char* restrict out_buffer);

/**
 * Writes to a file.
 * @param[in] handle The handle to write to.
 * @param[in] buffer the buffer to source bytes from for writing.
 * @param[in] buffer_size The number of bytes of buffer to write.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_handle_file_write(gen_filesystem_handle_t* const restrict handle, const unsigned char* const restrict buffer, const size_t buffer_size);

/**
 * Lists the contents of a directory.
 * @param[in] handle The handle to list from.
 * @param[out] out_list A pointer to pointers for storage each list entry. Each entry should be `GEN_FILESYSTEM_DIRECTORY_ENTRY_MAX` in size.
 * @param[out] out_length A pointer to storage for the length of the list.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_handle_directory_list(gen_filesystem_handle_t* const restrict handle, char* restrict * const restrict out_list, size_t* const restrict out_length);

/**
 * Locks a file.
 * @param[in] handle The handle to the file to lock.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_handle_lock(gen_filesystem_handle_t* const restrict handle);

/**
 * Unlocks a file.
 * @param[in] handle The handle to the file to lock.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_handle_unlock(gen_filesystem_handle_t* const restrict handle);

/**
 * Creates an update watcher for a handle.
 * @param[in] handle The handle to watch.
 * @param[out] out_watcher A pointer to storage for the created watcher.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_watcher_create(gen_filesystem_handle_t* const restrict handle, gen_filesystem_watcher_t* const restrict out_watcher);

/**
 * Destroys an update watcher.
 * @param[in] watcher The watcher to destroy.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_watcher_destroy(gen_filesystem_watcher_t* const restrict watcher);

/**
 * Checks an update watcher for events.
 * @param[in] watcher The watcher to poll.
 * @param[out] out_event A pointer to storage for events which have occurred.
 * @return An error, otherwise `NULL`.
 */
extern gen_error_t* gen_filesystem_watcher_poll(gen_filesystem_watcher_t* const restrict watcher, gen_filesystem_watcher_event_t* const restrict out_event);

#endif
