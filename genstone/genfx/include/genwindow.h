// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genwindow.h
 * Windowing utilities.
 */

#ifndef GEN_WINDOW_H
#define GEN_WINDOW_H

#include <gencommon.h>
#include <xcb/xcb.h>

/**
 * Handle for a window.
 */
typedef xcb_window_t gen_window_t;

/**
 * Bitmasks for window attributes.
 * @see gen_window_modify
 */
typedef enum {
    /**
     * Window visibility.
     */
    GEN_WINDOW_VISIBILITY,
    /**
     * Window position.
     */
    GEN_WINDOW_POSITION,
    /**
     * Window size.
     */
    GEN_WINDOW_SIZE
} gen_window_attrib_t;

/**
 * Creates a new window.
 * @param[out] out_window pointer to storage for the created window.
 * @return an error code.
 */
GEN_ERRORABLE gen_window_create(gen_window_t* const restrict out_window);

/**
 * Modifies a window attribute.
 * @param[in] window the window whose attributes should be modified.
 * @param[in] attrib the attribute to modify.
 */
GEN_ERRORABLE gen_window_modify(const gen_window_t* const restrict window, const gen_window_attrib_t attrib);

#endif
