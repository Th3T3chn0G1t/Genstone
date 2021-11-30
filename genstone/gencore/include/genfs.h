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
/**
 * File watcher instance handle
 */
typedef int gen_filewatch_handle_t;
#elif PLATFORM == DWN
#include <CoreServices/CoreServices.h>
typedef FSEventStreamRef gen_filewatch_handle_t;
#endif

/**
 * Bitmasks for filewatch event reporting
 * @see gen_filewatch_poll
 */
typedef enum {
    /**
     * No event occurred
     */
    GEN_FILEWATCH_NONE = 0,
    /**
     * A file was created
     */
    GEN_FILEWATCH_CREATED = 1,
    /**
     * A file was modified
     */
    GEN_FILEWATCH_MODIFIED = 2,
    /**
     * A file was deleted
     */
    GEN_FILEWATCH_DELETED = 4,
    /**
     * A file was moved
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
    union {
        /**
         * Handle for a file if dir is false.
         */
        int file_handle;
        /**
         * Handle for a directory if dir is true.
         */
        DIR* directory_handle;
    };
    /**
     * Whether this handle is for a directory.
     */
    bool is_directory;
} gen_filesystem_handle_t;

/**
 * Gets the canonical representation of a path.
 * @param[out] output_path storage for the output path. Should be `GEN_PATH_MAX` in size if length is unknown.
 * @param[in] path the path to get a canonical representation for.
 * @return an error code.
 */
GEN_ERRORABLE gen_path_canonical(char* restrict output_path, const char* const restrict path);

/**
 * Gets the filename of a path.
 * @param[out] output_filename storage for the output filename. Should be `GEN_PATH_MAX` in size if length is unknown.
 * @param[in] path the path to get a filename from.
 * @return an error code.
 */
GEN_ERRORABLE gen_path_filename(char* restrict output_filename, const char* const restrict path);

/**
 * Gets the pathname of a path.
 * @param[out] output_path storage for the output path. Should be `GEN_PATH_MAX` in size if length is unknown.
 * @param[in] path the path to get a pathname from.
 * @return an error code.
 */
GEN_ERRORABLE gen_path_pathname(char* restrict output_path, const char* const restrict path);

/**
 * Gets the file extension of a path.
 * @param[out] output_extension storage for the output extension, Should be `GEN_PATH_MAX` in size if length is unknown.
 * @param[in] path the path to get an extension from.
 * @return an error code.
 */
GEN_ERRORABLE gen_path_extension(char* restrict output_extension, const char* const restrict path);

/**
 * Returns whether a filesystem object exists at a path.
 * @param[in] path the path to check.
 * @return whether an object exists at path.
 */
extern bool gen_path_exists(const char* const restrict path);

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
GEN_ERRORABLE gen_file_read(uint8_t* restrict output_buffer, const gen_filesystem_handle_t* const restrict handle, const size_t start, const size_t end);

/**
 * Writes to a file.
 * @param[in] handle a handle to a file object to write to. Must not be a directory.
 * @param[in] n_bytes the number of bytes of buffer to write.
 * @param[in] buffer the buffer to source bytes from for writing.
 * @return an error code.
 */
GEN_ERRORABLE gen_file_write(const gen_filesystem_handle_t* const restrict handle, const size_t n_bytes, const uint8_t* const restrict buffer);

/**
 * Lists the contents of a directory.
 * @param[in] handle a handle to a directory object to list from. Must not be a file.
 * @param[in] handler a handler to be called for each entry in the directory.
 * @param[in] passthrough a passthrough to the handler.
 * @return an error code.
 */
GEN_ERRORABLE gen_directory_list(const gen_filesystem_handle_t* const restrict handle, const gen_directory_list_handler_t handler, void* const restrict passthrough);

GEN_ERRORABLE gen_filewatch_create(gen_filewatch_handle_t* const restrict handle, const char* const restrict path);
GEN_ERRORABLE gen_filewatch_poll(const gen_filewatch_handle_t* const restrict handle, gen_filewatch_event_t* const restrict out_event);
GEN_ERRORABLE gen_filewatch_destroy(const gen_filewatch_handle_t* const restrict handle);

#endif
