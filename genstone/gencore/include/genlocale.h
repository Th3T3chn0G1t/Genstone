// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genlocale.h
 * Utilities for managing C locales
 */

#ifndef GEN_LOCALE_H
#define GEN_LOCALE_H

#include "gencommon.h"

/**
 * Converts a currency amount to a locale-formatted string
 * @param out_string pointer to storage for the formatted string. Nullable
 * @param out_size pointer for storage for the required size of the formatted string. Nullable
 * @param units the units part of the currency amount
 * @param cents the cents part of the currency amount
 * @return an error code
 */
extern GEN_ERRORABLE_RETURN gen_locale_convert_currency(char* const restrict out_string, size_t* const restrict out_size, const uint64_t units, const uint64_t cents);

#endif
