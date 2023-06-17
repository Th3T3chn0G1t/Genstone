// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"

const char* gen_error_type_name(const gen_error_type_t error) {
    return (const char*[]) {
        [GEN_ERROR_UNKNOWN] =
            "GEN_ERROR_UNKNOWN",
        [GEN_ERROR_PERMISSION] =
            "GEN_ERROR_PERMISSION",
        [GEN_ERROR_INVALID_PARAMETER] =
            "GEN_ERROR_INVALID_PARAMETER",
        [GEN_ERROR_IO] =
            "GEN_ERROR_IO",
        [GEN_ERROR_TOO_LONG] =
            "GEN_ERROR_TOO_LONG",
        [GEN_ERROR_NO_SUCH_OBJECT] =
            "GEN_ERROR_NO_SUCH_OBJECT",
        [GEN_ERROR_OUT_OF_MEMORY] =
            "GEN_ERROR_OUT_OF_MEMORY",
        [GEN_ERROR_WRONG_OBJECT_TYPE] =
            "GEN_ERROR_WRONG_OBJECT_TYPE",
        [GEN_ERROR_ALREADY_EXISTS] =
            "GEN_ERROR_ALREADY_EXISTS",
        [GEN_ERROR_OUT_OF_SPACE] =
            "GEN_ERROR_OUT_OF_SPACE",
        [GEN_ERROR_OUT_OF_HANDLES] =
            "GEN_ERROR_OUT_OF_HANDLES",
        [GEN_ERROR_TOO_SHORT] =
            "GEN_ERROR_TOO_SHORT",
        [GEN_ERROR_BAD_CONTENT] =
            "GEN_ERROR_BAD_CONTENT",
        [GEN_ERROR_BAD_OPERATION] =
            "GEN_ERROR_BAD_OPERATION",
        [GEN_ERROR_IN_USE] =
            "GEN_ERROR_IN_USE",
        [GEN_ERROR_NOT_IMPLEMENTED] =
            "GEN_ERROR_NOT_IMPLEMENTED",
        [GEN_ERROR_OUT_OF_BOUNDS] =
            "GEN_ERROR_OUT_OF_BOUNDS",
        [GEN_ERROR_INVALID_CONTROL] =
            "GEN_ERROR_INVALID_CONTROL",
        [GEN_ERROR_BAD_ALIGNMENT] =
            "GEN_ERROR_BAD_ALIGNMENT",
        [GEN_ERROR_OPERATION_FAILED] =
            "GEN_ERROR_OPERATION_FAILED",
        [GEN_ERROR_BAD_TIMING] =
            "GEN_ERROR_BAD_TIMING",
        [GEN_ERROR_TIMEOUT] =
            "GEN_ERROR_TIMEOUT",
        [GEN_ERROR_DOES_NOT_MATCH] =
            "GEN_ERROR_DOES_NOT_MATCH",
    }[error];
}

const char* gen_error_type_description(const gen_error_type_t error) {
    return (const char*[]) {
        [GEN_ERROR_UNKNOWN] =
            "An unknown error occurred",
        [GEN_ERROR_PERMISSION] =
            "A permission error occurred",
        [GEN_ERROR_INVALID_PARAMETER] =
            "The provided parameter was invalid",
        [GEN_ERROR_IO] =
            "An IO error occurred",
        [GEN_ERROR_TOO_LONG] =
            "The provided argument is too long",
        [GEN_ERROR_NO_SUCH_OBJECT] =
            "A nonexistent object was referenced",
        [GEN_ERROR_OUT_OF_MEMORY] =
            "Program ran out of usable memory",
        [GEN_ERROR_WRONG_OBJECT_TYPE] =
            "An object of the wrong type was provided",
        [GEN_ERROR_ALREADY_EXISTS] =
            "An object already exists by the provided identifier",
        [GEN_ERROR_OUT_OF_SPACE] =
            "The specified destination is out of space",
        [GEN_ERROR_OUT_OF_HANDLES] =
            "Too many platform handles are open",
        [GEN_ERROR_TOO_SHORT] =
            "The provided argument is too short",
        [GEN_ERROR_BAD_CONTENT] =
            "The provided argument contains bad or invalid content",
        [GEN_ERROR_BAD_OPERATION] =
            "A bad or invalid operation was requested",
        [GEN_ERROR_IN_USE] =
            "The specified target is in use elsewhere",
        [GEN_ERROR_NOT_IMPLEMENTED] =
            "An unimplemented feature was used",
        [GEN_ERROR_OUT_OF_BOUNDS] =
            "The specified value is out of bounds",
        [GEN_ERROR_INVALID_CONTROL] =
            "The program reached an invalid control path",
        [GEN_ERROR_BAD_ALIGNMENT] =
            "The provided data was misaligned",
        [GEN_ERROR_OPERATION_FAILED] =
            "An operation could not be performed",
        [GEN_ERROR_BAD_TIMING] =
            "An operation was performed untimely manner",
        [GEN_ERROR_TIMEOUT] =
            "An operation timed out",
        [GEN_ERROR_DOES_NOT_MATCH] =
            "The specified values did not match",
    }[error];
}
