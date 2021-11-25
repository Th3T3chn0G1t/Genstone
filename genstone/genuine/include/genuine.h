// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genuine.h
 * Abstract utilities for constructing drawing data from high-level UI elements
 */

#ifndef GEN_UI_H
#define GEN_UI_H

#include <gencommon.h>

typedef ssize_t gen_ui_extent_t;

typedef struct {
	gen_ui_extent_t x;
	gen_ui_extent_t y;
	gen_ui_extent_t w;
	gen_ui_extent_t h;
} const gen_ui_rect_t;

typedef void (*gen_ui_draw_handler_t)(void* const restrict, const gen_ui_rect_t, const gen_ui_rect_t, void* const restrict);

GEN_ERRORABLE gen_ui_draw_ninepatch_direct(void* const restrict ninepatch, const gen_ui_draw_handler_t draw_handler, const gen_ui_rect_t extent, const gen_ui_extent_t src_scale, const gen_ui_extent_t dest_scale, void* const restrict passthrough);

#endif
