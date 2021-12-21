// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#ifndef DCS_H
#define DCS_H

#include <gencommon.h>
#include <genargs.h>
#include <gendl.h>
#include <genfs.h>
#include <genlocale.h>
#include <genproc.h>

typedef struct {
	const char* source_filename;
} dcs_options_t;

typedef enum
{
    // Keywords
    DCS_TOKEN_EXTERNAL,
    DCS_TOKEN_INTERNAL,
    DCS_TOKEN_STATIC,
    DCS_TOKEN_DYNAMIC,
    DCS_TOKEN_ROUTINE,
    DCS_TOKEN_ALIGN,
    DCS_TOKEN_RETURN,

    // Symbols
    DCS_TOKEN_SPECIFIER_START,
    DCS_TOKEN_SPECIFIER_END,
    DCS_TOKEN_BOUNDED_EXPRESSION_START,
    DCS_TOKEN_BOUNDED_EXPRESSION_END,
    DCS_TOKEN_BLOCK_START,
    DCS_TOKEN_BLOCK_END,
    DCS_TOKEN_STATEMENT_DELIMITER,
    DCS_TOKEN_PARAMETER_DELIMITER,
    DCS_TOKEN_CONDITIONAL,

    // Literals
    DCS_TOKEN_STRING_LITERAL,
    DCS_TOKEN_NUMBER_LITERAL,
    DCS_TOKEN_FLOAT_LITERAL,

    // Other
    DCS_TOKEN_IDENTIFIER,
    DCS_TOKEN_EOF,
    DCS_TOKEN_UNKNOWN
} dcs_token_t;

GEN_ERRORABLE dcs_tokenize(const char* const restrict source, const size_t size);

#endif
