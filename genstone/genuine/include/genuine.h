// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genuine.h
 * Abstract utilities for constructing drawing data from high-level UI elements.
 */

#ifndef GEN_UI_H
#define GEN_UI_H

#include <gencommon.h>

/**
 * Type to use for extents in a UI such as positions or lengths.
 */
typedef ssize_t gen_ui_extent_t;

/**
 * A rectangle to be used to represent UI elements in terms of position and extent.
 */
typedef struct {
	/**
	 * The x position of the element.
	 */
	gen_ui_extent_t x;
	/**
	 * The y position of the element.
	 */
	gen_ui_extent_t y;
	/**
	 * The x axis extent of the element.
	 */
	gen_ui_extent_t w;
	/**
	 * The y axis extent of the element.
	 */
	gen_ui_extent_t h;
} gen_ui_rect_t;

/**
 * Handler for drawing textured UI rects.
 */
typedef void (*gen_ui_draw_handler_t)(void* const restrict, const gen_ui_rect_t, const gen_ui_rect_t, void* const restrict);

/**
 * Invokes a draw handler to correctly lay out a ninepatch UI element.
 * @param[in] ninepatch the texture to draw - this should be your graphics-system's renderable texture representation.
 * @param[in] draw_handler the handler to use for drawing textured UI rects.
 * @param[in] extent the position and extent of the element.
 * @param[in] src_scale the scale of the ninepatch at source in pixels - one third of the image width.
 * @param[in] dest_scale the scale to draw the UI element at in pixels - maps to `src_scale` 1:1.
 * @param[in] passthrough a passthrough for the draw handler.
 * @return an error code.
 */
GEN_ERRORABLE gen_ui_draw_ninepatch_direct(void* const restrict ninepatch, const gen_ui_draw_handler_t draw_handler, const gen_ui_rect_t extent, const gen_ui_extent_t src_scale, const gen_ui_extent_t dest_scale, void* const restrict passthrough);

#endif
