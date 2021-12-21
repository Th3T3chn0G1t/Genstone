// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/dcs.h"

#define find(in, chars) \
    ({ \
        bool found = false; \
        GEN_FOREACH(i, c, sizeof(#chars) - 1, #chars) { \
            if(in == c) { \
                found = true; \
                break; \
            } \
        } \
        found; \
    })

gen_error_t dcs_tokenize(const char* const restrict source, const size_t size) {
	GEN_FRAME_BEGIN(dcs_tokenize);

    size_t offset = 0;

    while(offset < size) {
        char current = source[offset];

        if(isalpha(current)) {

        }
        else if(isdigit(current)) {
            while(true) {
                current = source[++offset];
                if(isdigit())
            }
        }
        else if(find(current, "<")) {
            DCS_TOKEN_SPECIFIER_START;
        }
        else if(find(current, ">")) {
            DCS_TOKEN_SPECIFIER_END;
        }
        else if(find(current, ";")) {
            DCS_TOKEN_STATEMENT_DELIMITER;
        }
        else if(find(current, ",")) {
            DCS_TOKEN_PARAMETER_DELIMITER;
        }
        else if(find(current, "?")) {
            DCS_TOKEN_CONDITIONAL;
        }
        else if(find(current, "{")) {
            DCS_TOKEN_BLOCK_START;
        }
        else if(find(current, "}")) {
            DCS_TOKEN_BLOCK_END;
        }
        else if(find(current, "(")) {
            DCS_TOKEN_BOUNDED_EXPRESSION_START;
        }
        else if(find(current, ")")) {
            DCS_TOKEN_BOUNDED_EXPRESSION_END;
        }
    }

    GEN_ALL_OK;
}
