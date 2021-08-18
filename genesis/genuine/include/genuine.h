// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @file genuine.h
 * Abstract utilities for constructing drawing data from high-level UI elements
 */

#ifndef GEN_UI_H
#define GEN_UI_H

#include <gencommon.h>

/**
 * A quad used for UI drawing
 * Consists of position and extent
 */
typedef union {
    /**
     * All attributes of the rect condensed into an array for easy initialisation and copying
     */
    uint16_t xywh[4];
    struct {
        /**
         * The x component of the rect's position
         */
        uint16_t x;
        /**
         * The y component of the rect's position
         */
        uint16_t y;
        /**
         * The w component of the rect's extent
         */
        uint16_t w;
        /**
         * The h component of the rect's extent
         */
        uint16_t h;
    };
} gen_ui_rect_t;

/**
 * A ninepatch to draw elements with
 */
typedef struct {
    /**
     * User pointer for the texture of the ninepatch
     */
    void* texture;
    /**
     * The scale of each segment of the source of the texture
     */
    int source_scale;
} gen_ui_ninepatch_t;


/**
 * A representation of a
 */
typedef struct {
    /**
     * The ninepatch to render this element with
     */
    gen_ui_ninepatch_t* ninepatch;
    /**
     * The rectangle this element should occupy
     */
    gen_ui_rect_t rect;
} gen_ui_element_t;

/**
 * Handler for rendering rects from UI
 */
typedef void (*gen_ui_render_handler_t) (const gen_ui_ninepatch_t* const restrict, const gen_ui_rect_t, const gen_ui_rect_t, void* restrict);

/**
 * Indices for the segments of a ninepatch
 */
typedef enum {
    /**
     * Index for top left segment
     */
    GEN_UI_SOURCE_RECT_TL = 0,
    /**
     * Index for top centre segment
     */
    GEN_UI_SOURCE_RECT_T,
    /**
     * Index for top right segment
     */
    GEN_UI_SOURCE_RECT_TR,
    /**
     * Index for middle left segment
     */
    GEN_UI_SOURCE_RECT_L,
    /**
     * Index for middle centre segment
     */
    GEN_UI_SOURCE_RECT_C,
    /**
     * Index for middle right segment
     */
    GEN_UI_SOURCE_RECT_R,
    /**
     * Index for bottom left segment
     */
    GEN_UI_SOURCE_RECT_BL,
    /**
     * Index for bottom centre segment
     */
    GEN_UI_SOURCE_RECT_B,
    /**
     * Index for bottom right segment
     */
    GEN_UI_SOURCE_RECT_BR,
} gen_ui_ninepatch_source_rect_index_t;

/**
 * Delivers draw data to a user rendering handler from a UI element
 * @param handler a handler to draw the component rects of the element
 * @param element the element to draw
 * @param destination_scale the scale to draw the element at
 * @param passthrough a passthrough to the handler
 */
void gen_ui_draw_element(const gen_ui_render_handler_t handler, const gen_ui_element_t* const restrict element, const uint16_t destination_scale, void* restrict passthrough);

#endif
