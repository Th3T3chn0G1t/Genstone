// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genfs.h
 * Includes filesystem utilities for manipulation and introspection.
 */

#ifndef GEN_FS_H
#define GEN_FS_H

#include "gencommon.h"

#include <unistd.h>
#ifndef PATH_MAX
/**
 * The OS library's maximum length of a path.
 */
#define GEN_PATH_MAX 1024
#else
#define GEN_PATH_MAX PATH_MAX
#endif

#if PLATFORM == LNX
#include <sys/inotify.h>
#endif

/**
 * Bitmasks for filewatch event reporting.
 * Not all event types are reported on all platforms.
 * @see gen_filewatch_poll
 */
typedef enum
{
	/**
     * No event occurred.
     */
	GEN_FILEWATCH_NONE = 0,
	/**
     * A file was created.
     */
	GEN_FILEWATCH_CREATED = 1,
	/**
     * A file was modified.
     */
	GEN_FILEWATCH_MODIFIED = 2,
	/**
     * A file was deleted.
     */
	GEN_FILEWATCH_DELETED = 4,
	/**
     * A file was moved.
     */
	GEN_FILEWATCH_MOVED = 8
} gen_filewatch_event_t;

/**
 * Handler for directory listing.
 */
typedef void (*gen_directory_list_handler_t)(const char* const restrict, void* const restrict);

/**
 * A handle to a filesystem object.
 * @note Directly modifying the internal handles may cause undefined behaviour.
 */
typedef struct {
	/**
     * Handle for a file if dir is false.
     * May still contain important data if dir is true.
     */
	int file_handle;
	/**
     * Handle for a directory if dir is true.
     */
	DIR* directory_handle;
	/**
     * Whether this handle is for a directory.
     */
	bool is_directory;
	/**
     * Internal caching of `stat` retvals.
     * Please don't use this directly.
     */
	struct stat internal_descriptor_details;
	/**
     * Intenal caching of directory size.
     * Please don't use this directly.
     */
	size_t internal_directory_len;
} gen_filesystem_handle_t;

/**
 * `gen_filesystem_handle_t` wrapper for stdin.
 */
#define gen_stdin_handle ((gen_filesystem_handle_t){STDIN_FILENO, NULL, false, {0}, 0})
/**
 * `gen_filesystem_handle_t` wrapper for stdout.
 */
#define gen_stdout_handle ((gen_filesystem_handle_t){STDOUT_FILENO, NULL, false, {0}, 0})
/**
 * `gen_filesystem_handle_t` wrapper for stderr.
 */
#define gen_stderr_handle ((gen_filesystem_handle_t){STDERR_FILENO, NULL, false, {0}, 0})

/**
 * @example{lineno} example/gencore/gen_path.c
 * Example for how to use the `gen_path*` family of functions.
 * The `gen_path*` family of functions is used for manipulating paths in a better way than the libc provides.
 */

/**
 * Gets the canonical representation of a path.
 * @param[out] output_path storage for the output path. Should be `GEN_PATH_MAX + 1` in size if length is unknown.
 * @param[in] path the path to get a canonical representation for.
 * @return an error code.
 */
GEN_ERRORABLE gen_path_canonical(char* restrict output_path, const char* const restrict path);

/**
 * Returns whether a filesystem object exists at a path.
 * @param[in] path the path to check.
 * @return whether an object exists at path.
 */
GEN_ERRORABLE gen_path_exists(const char* const restrict path, bool* const restrict out_exists);

/**
 * Checks whether a path is valid.
 * @param[in] path the path to validate.
 * @return an error code. `GEN_TOO_LONG` or `GEN_TOO_SHORT` if a path is an invalid length. `GEN_WRONG_OBJECT_TYPE` if an invalid character is encountered in the path.
 */
GEN_ERRORABLE gen_path_validate(const char* const restrict path);

/**
 * Creates a file.
 * @param[in] path the file path to create.
 * @return an error code.
 */
GEN_ERRORABLE gen_path_create_file(const char* const restrict path);

/**
 * Creates a directory.
 * @param[in] path the directory path to create.
 * @return an error code.
 * @note Will create with the default access flags for the platform, or a reasonable default if not applicable.
 */
GEN_ERRORABLE gen_path_create_dir(const char* const restrict path);

/**
 * Deletes a filesystem object.
 * @param[in] path a path to the object to destroy.
 * @return an error code.
 */
GEN_ERRORABLE gen_path_delete(const char* const restrict path);

/**
 * @example{lineno} example/gencore/gen_filesystem_handle.c
 * Example for how to use `gen_handle_open` `gen_handle_size` `gen_handle_read` `gen_handle_write` and `gen_directory_list`.
 * The `gen_handle*` family of functions is used for managing files and directories in a better way than the libc provides.
 */

/**
 * Opens a path as a filesystem object handle for use by filesystem operations.
 * Creates a file at path if nothing exists there already.
 * @param[out] output_handle pointer to storage for the handle.
 * @param[in] path the path to open.
 * @return an error code.
 */
GEN_ERRORABLE gen_handle_open(gen_filesystem_handle_t* restrict output_handle, const char* const restrict path);

/**
 * Closes a filesystem object handle.
 * Storage of the handle may be reused after this call, though memory is not zeroed.
 * @param[in] handle pointer to the handle to close.
 * @return an error code.
 */
GEN_ERRORABLE gen_handle_close(gen_filesystem_handle_t* const restrict handle);

/**
 * Gets the size of a handle's object's content.
 * @param[out] out_size a pointer to storage for the size of the handle.
 * @param[in] handle a handle to an object whose size to get. Must not be a directory.
 * @return an error code.
 */
GEN_ERRORABLE gen_handle_size(size_t* const restrict out_size, const gen_filesystem_handle_t* const restrict handle);

/**
 * Gets a file's content.
 * @param[out] output_buffer storage for the read bytes.
 * @param[in] handle a handle to a file object whose content to read. Must not be a directory.
 * @param[in] start an offset from the start of the file to begin reading from.
 * @param[in] end an offset from the start of the file to end reading from.
 * @note Use `gen_handle_size` as the end mark to read the whole file.
 * @note Does not add a null terminator to the buffer.
 * @return an error code.
 */
GEN_ERRORABLE gen_handle_read(uint8_t* restrict output_buffer, const gen_filesystem_handle_t* const restrict handle, const size_t start, const size_t end);

/**
 * Writes to a file.
 * @param[in] handle a handle to a file object to write to. Must not be a directory.
 * @param[in] n_bytes the number of bytes of buffer to write.
 * @param[in] buffer the buffer to source bytes from for writing.
 * @return an error code.
 */
GEN_ERRORABLE gen_handle_write(const gen_filesystem_handle_t* const restrict handle, const size_t n_bytes, const uint8_t* const restrict buffer);

/**
 * Lists the contents of a directory.
 * @param[in] handle a handle to a directory object to list from. Must not be a file.
 * @param[in] handler a handler to be called for each entry in the directory.
 * @param[in] passthrough a passthrough to the handler.
 * @return an error code.
 */
GEN_ERRORABLE gen_directory_list(const gen_filesystem_handle_t* const restrict handle, const gen_directory_list_handler_t handler, void* const restrict passthrough);

/**
 * @example{lineno} example/gencore/gen_filewatch.c
 * Example for how to use `gen_filewatch_create` `gen_filewatch_poll` and `gen_filewatch_destroy`.
 * The `gen_filewatch*` family of functions is used for watching files and directories for changes in a platform agnostic way.
 */

/**
 * Creates a filesystem watcher for a file or directory.
 * @param[out] out_handle a pointer to storage for the created handle. This is not required to be the same as the filesystem handle used for IO.
 * @param[in] handle a filesystem handle to the file to watch.
 * @return an error code.
 */
GEN_ERRORABLE gen_filewatch_create(gen_filesystem_handle_t* const restrict out_handle, const gen_filesystem_handle_t* const restrict handle);

/**
 * Checks a filesystem watcher for events.
 * @param[in] handle the handle to the filesystem watcher to check. This is not required to be the same as the filesystem handle used for IO.
 * @param[out] out_event pointer to storage for events which have occurred. Bitmask of `gen_filewatch_event_t` values.
 * @return an error code.
 */
GEN_ERRORABLE gen_filewatch_poll(gen_filesystem_handle_t* const restrict handle, gen_filewatch_event_t* const restrict out_event);

/**
 * Destroys a filesystem watcher.
 * @param[in] handle the handle to the filesystem watcher to destroy. This is not required to be the same as the filesystem handle used for IO.
 * @return an error code.
 */
GEN_ERRORABLE gen_filewatch_destroy(gen_filesystem_handle_t* const restrict handle);

#endif
