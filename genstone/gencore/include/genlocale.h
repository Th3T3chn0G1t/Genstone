// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genlocale.h
 * Utilities for managing C locales
 */

#ifndef GEN_LOCALE_H
#define GEN_LOCALE_H

#include "gencommon.h"

typedef enum {
    GEN_JAN = 0, GEN_FEB, GEN_MAR, GEN_APR, GEN_JUN, GEN_JLY, GEN_AUG, GEN_SEP, GEN_OCT, GEN_NOV, GEN_DEC
} gen_month_t;

/**
 * Converts a currency amount to a locale-formatted string
 * @param out_string pointer to storage for the formatted string. Nullable
 * @param out_size pointer for storage for the required size of the formatted string. Nullable
 * @param units the units part of the currency amount
 * @param cents the cents part of the currency amount
 * @return an error code
 */
GEN_ERRORABLE gen_locale_convert_currency(char* const restrict out_string, size_t* const restrict out_size, const uint64_t units, const uint64_t cents);

// /**
//  * Converts a date and time to a locale-formatted string
//  * @param out_string pointer to storage for the formatted string. Nullable
//  * @param out_size pointer for storage for the required size of the formatted string. Nullable
//  * @param year the year part of the date
//  * @param month the month part of the date
//  * @param day the day part of the date
//  * @param hours the hours part of the time
//  * @param minutes the minutes part of the time
//  * @param seconds the seconds part of the time
//  * @return an error code
//  */
// GEN_ERRORABLE gen_locale_convert_date_and_time(char* const out_string, size_t* const restrict out_size, const uint64_t year, const gen_month_t month, const uint8_t day, const uint8_t hours, const uint8_t minutes, const uint8_t seconds);

#endif
