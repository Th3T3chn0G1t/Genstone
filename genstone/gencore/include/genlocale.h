// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genlocale.h
 * Utilities for managing C locales.
 */

#ifndef GEN_LOCALE_H
#define GEN_LOCALE_H

#include "gencommon.h"

// /**
//  * Enumeration of months for locale-based date conversion
//  */
// typedef enum {
//     GEN_JAN = 0, GEN_FEB, GEN_MAR, GEN_APR, GEN_JUN, GEN_JLY, GEN_AUG, GEN_SEP, GEN_OCT, GEN_NOV, GEN_DEC
// } gen_month_t;

// /**
//  * Flags for selecting conversions in locale-based date and time conversion
//  * @see gen_locale_convert_date_and_time
//  */
// typedef enum {
//     GEN_DT_FMT_NONE = 0,
//     GEN_DT_FMT_TIME = 1,
//     GEN_DT_FMT_TIMEZONE = 2,
//     GEN_DT_FMT_WEEKDAY = 4,
//     GEN_DT_FMT_DATE = 8,
//     GEN_DT_FMT_ALL = GEN_DT_FMT_TIME | GEN_DT_FMT_TIMEZONE | GEN_DT_FMT_WEEKDAY | GEN_DT_FMT_DATE
// } gen_dt_fmt_flags_t;

/**
 * Converts a currency amount to a locale-formatted string.
 * @param[out] out_string pointer to storage for the formatted string. Nullable.
 * @param[in] buffer_size the size of the output buffer.
 * @param[out] out_size pointer for storage for the required size of the formatted string. Nullable.
 * @param[in] units the units part of the currency amount.
 * @param[in] cents the cents part of the currency amount.
 * @return an error code.
 */
extern gen_error_t gen_locale_convert_currency(char* const restrict out_string, const size_t buffer_size, size_t* const restrict out_size, const uint64_t units, const uint64_t cents);
/**
 * @example{lineno} example/gencore/gen_locale_convert_currency.c
 * Example for how to use `gen_locale_convert_currency`.
 * `gen_locale_convert_currency` is used for managing locale-based data conversion in a better way than the libc provides.
 */

// /**
//  * Converts a date and time to a locale-formatted string based on selection flags.
//  * @param out_string pointer to storage for the formatted string. Nullable.
//  * @param out_size pointer for storage for the required size of the formatted string. Nullable.
//  * @param year the year part of the date.
//  * @param month the month part of the date.
//  * @param day the day part of the date.
//  * @param hours the hours part of the time.
//  * @param minutes the minutes part of the time.
//  * @param seconds the seconds part of the time.
//  * @param flags the aspects of the string to generate, or'd together
//  * @return an error code.
//  */
// extern gen_error_t gen_locale_convert_date_and_time(char* const out_string, size_t* const restrict out_size, const uint64_t year, const gen_month_t month, const unsigned char day, const unsigned char hours, const unsigned char minutes, const unsigned char seconds, const gen_dt_fmt_flags_t flags);

#endif
