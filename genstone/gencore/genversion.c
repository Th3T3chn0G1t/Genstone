// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2023 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genversion.h"

gen_error_t* gen_version_compare(const gen_version_t* const restrict yours, const gen_version_t* const restrict theirs, gen_version_comparison_result_t* const restrict result) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_version_compare, GEN_FILE_NAME);
    if(error) return error;

    if(!yours) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`yours` was `GEN_NULL`");
    if(!theirs) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`theirs` was `GEN_NULL`");
    if(!result) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`result` was `GEN_NULL`");

    if(theirs->major > yours->major) {
        *result = GEN_VERSION_OLDER;
        return GEN_NULL;
    }
    if(theirs->major < yours->major) {
        *result = GEN_VERSION_NEWER;
        return GEN_NULL;
    }
    if(theirs->major == yours->major) {
        if(theirs->minor > yours->minor) {
            *result = GEN_VERSION_OLDER;
            return GEN_NULL;
        }
        if(theirs->minor < yours->minor) {
            *result = GEN_VERSION_NEWER;
            return GEN_NULL;
        }
        if(theirs->minor == yours->minor) {
            if(theirs->patch > yours->patch) {
                *result = GEN_VERSION_OLDER;
                return GEN_NULL;
            }
            if(theirs->patch < yours->patch) {
                *result = GEN_VERSION_NEWER;
                return GEN_NULL;
            }
            if(theirs->patch == yours->patch) {
                *result = GEN_VERSION_SAME;
                return GEN_NULL;
            }
        }
    }

    return gen_error_attach_backtrace(GEN_ERROR_INVALID_CONTROL, GEN_LINE_NUMBER, "Invalid control path");
}
