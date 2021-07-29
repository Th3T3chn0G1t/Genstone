/**
 * @file genfs.h
 * Includes filesystem utilities for manipulation and introspection
 * @see genres.h for variants of utilities which take a state pointer and have various optimizations
 */

#ifndef GEN_FS_H
#define GEN_FS_H

#include "gencommon.h"
#include "generrors.h"

#if PLATFORM == WIN
#include <windows.h>
#include "Shlwapi.h" // Apparently this required according to MSDN
#define GEN_PATH_MAX MAX_PATH
#else
#include <unistd.h>
/**
 * The OS library's maximum length of a path
 * @note does not currently support Windows 10 1607 long paths
 * @note does not neccesarily represent the maximum length of a path supported by every filesystem
 */
#define GEN_PATH_MAX PATH_MAX
#endif

/**
 * Handler for directory listing
 */
typedef void (*gen_directory_list_handler_t)(const char*, void*);

/**
 * A handle to a filesystem object
 * @note directly modifying the internal handles may cause undefined behaviour
 */
typedef struct {
    /**
     * A path to the object this handle is for
     */
    char* path;
    /**
     * Whether this handle is for a directory
     */
    bool dir;
    union {
        /**
         * Handles for a file if dir is false
         * A handle is opened for each mode (r & w) as-needed
         */
        FILE* file_handles[2];
        /**
         * Handle for a directory if dir is true
         */
        DIR* directory_handle;
    };
} gen_filesystem_handle_t;

/**
 * Gets the canonical representation of a path
 * @param output_path storage for the output path. Should be GEN_PATH_MAX in size if length is unknown
 * @param path the path to get a canonical representation for
 * @note under windows, GEN_PATH_MAX will be provided to GetFullPathNameA nBufferLength in all cases
 * @return an error code
 */
extern gen_error_t gen_path_canonical(char* output_path, const char* path);

/**
 * Gets the relative representation of a path
 * @param output_path storage for the output path. Should be GEN_PATH_MAX in size if length is unknown
 * @param path the canonical path to get a relative representation for
 * @param to the canonical path to make path relative to
 * @note if the path cannot be made relative, the output is set equal to path
 * @return an error code
 */
extern gen_error_t gen_path_relative(char* output_path, const char* path, const char* to);

/**
 * Gets the filename of a path
 * @param output_filename storage for the output filename. Should be GEN_PATH_MAX in size if length is unknown
 * @param path the path to get a filename from
 * @return an error code
 */
extern gen_error_t gen_path_filename(char* output_filename, const char* path);

/**
 * Gets the pathname of a path
 * @param output_path storage for the output path. Should be GEN_PATH_MAX in size if length is unknown
 * @param path the path to get a pathname from
 * @return an error code
 */
extern gen_error_t gen_path_pathname(char* output_path, const char* path);

/**
 * Gets the file extension of a path
 * @param output_extension storage for the output extension, Should be GEN_PATH_MAX in size if length is unknown
 * @param path the path to get an extension from
 * @return an error code
 */
extern gen_error_t gen_path_extension(char* output_extension, const char* path);

/**
 * Returns whether a filesystem object exists at a path
 * @param path the path to check
 * @return whether an object exists at path
 */
extern bool gen_path_exists(const char* path);

/**
 * Checks whether a path is valid
 * @param path the path to validate
 * @return an error code. GEN_TOO_LONG or GEN_TOO_SHORT if a path is an invalid length. GEN_WRONG_OBJECT_TYPE if an invalid character is encountered in the path
 */
extern gen_error_t gen_path_validate(const char* path);

/**
 * Creates a directory
 * @param path the directory path to create
 * @return an error code
 * @note will create with the default access flags for the platform, or a reasonable default if not applicable
 */
extern gen_error_t gen_path_create_dir(const char* path);

/**
 * Deletes a filesystem object
 * @param path a path to the object to destroy
 * @return an error code
 */
extern gen_error_t gen_path_delete(const char* path);

/**
 * Opens a path as a filesystem object handle for use by filesystem operations
 * Creates a file at path if nothing exists there already
 * @param output_handle pointer to storage for the handle
 * @param path the path to open
 * @return an error code
 */
extern gen_error_t gen_handle_open(gen_filesystem_handle_t* output_handle, const char* path);

/**
 * Closes a filesystem object handle
 * Storage of the handle may be reused after this call, though memory is not zeroed
 * @param handle pointer to the handle to close
 * @return an error code
 */
extern gen_error_t gen_handle_close(gen_filesystem_handle_t* handle);

/**
 * Gets the size of a handle's object's content
 * @param handle a handle to an object whose size to get
 * @return the size of the object's content
 * @note for directories, this will be the collective size of children
 */
extern size_t gen_handle_size(const gen_filesystem_handle_t* handle);

/**
 * Gets a file's content
 * @param output_buffer storage for the read bytes
 * @param handle a handle to a file object whose content to read. Must not be a directory
 * @param start an offset from the start of the file to begin reading from
 * @param end an offset from the start of the file to end reading from
 * @note use gen_handle_size as the end mark to read the whole file
 * @note does not add a null terminator to the buffer
 * @return an error code
 */
extern gen_error_t gen_file_read(unsigned char* output_buffer, const gen_filesystem_handle_t* handle, const size_t start, const size_t end);

/**
 * Writes to a file
 * @param handle a handle to a file object to write to. Must not be a directory
 * @param n_bytes the number of bytes of buffer to write
 * @param buffer the buffer to source bytes from for writing
 * @return an error code
 */
extern gen_error_t gen_file_write(const gen_filesystem_handle_t* handle, const size_t n_bytes, const unsigned char* buffer);

/**
 * Lists the contents of a directory
 * @param handle a handle to a directory object to list from. Must not be a file
 * @param handler a handler to be called for each entry in the directory
 * @param passthrough a passthrough to the handler
 * @return an error code
 */
extern gen_error_t gen_directory_list(const gen_filesystem_handle_t* handle, const gen_directory_list_handler_t handler, void* passthrough);

#endif
