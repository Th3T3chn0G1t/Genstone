// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

/**
 * @file genwindow.h
 * Windowing utilities.
 */

#ifndef GEN_WINDOW_H
#define GEN_WINDOW_H

#include "gengfxdefs.h"
#include <gencalc.h>

#include <gencommon.h>

GEN_DIAG_REGION_BEGIN
GEN_DIAG_IGNORE_ALL
#include <xcb/xcb.h>
#include <xcb/xkb.h>
#include <xcb/xcb_icccm.h>
GEN_DIAG_REGION_END

/**
 * Types for window attributes.
 * @see gen_window_modify
 */
typedef enum
{
	/**
     * Window visibility.
     */
	GEN_WINDOW_ATTRIBUTE_VISIBILITY,
	/**
     * Window fullscreen state.
     */
	GEN_WINDOW_ATTRIBUTE_FULLSCREEN,
	/**
     * Window decoration visibility.
     */
	GEN_WINDOW_ATTRIBUTE_DECORATION,
	/**
     * Window position.
     */
	GEN_WINDOW_ATTRIBUTE_POSITION,
	/**
     * Window extent.
     */
	GEN_WINDOW_ATTRIBUTE_EXTENT,
     /**
      * Window name/title.
      */
     GEN_WINDOW_ATTRIBUTE_NAME,
     /**
      * Window icon
      */
     GEN_WINDOW_ATTRIBUTE_ICON
} gen_window_attribute_type_t;

/**
 * Structure describing a window attribute.
 */
typedef struct {
     /**
      * The attribute that was modified.
      */
     gen_window_attribute_type_t type;
     union {
          /**
           * Whether the window is visible.
           * Valid only if `type` is `GEN_WINDOW_ATTRIBUTE_VISIBILITY`.
           */
          bool visible;
          /**
           * Whether the window is fullscreen.
           * Valid only if `type` is `GEN_WINDOW_ATTRIBUTE_FULLSCREEN`.
           */
          bool fullscreen;
          /**
           * Whether the window is decorated.
           * Valid only if `type` is `GEN_WINDOW_ATTRIBUTE_DECORATION`.
           */
          bool decorated;
          /**
           * The position of the window.
           * Valid only if `type` is `GEN_WINDOW_ATTRIBUTE_POSITION`.
           */
          gint2 position;
          /**
           * The extent of the window.
           * Valid only if `type` is `GEN_WINDOW_ATTRIBUTE_EXTENT`.
           */
          gint2 extent;
          /**
           * The name/title of the window.
           * Valid only if `type` is `GEN_WINDOW_ATTRIBUTE_NAME`.
           */
          const char* name;
          /**
           * The window icon.
           * Valid only if `type` is "GEN_WINDOW_ATTRIBUTE_ICON".
           */
          gen_gfx_buffer_t icon;
     };
} gen_window_attribute_t;

/**
 * Types for window system events.
 */
typedef enum
{
     /**
      * No event occurred.
      */
     GEN_WINDOW_SYSTEM_EVENT_NONE,
     /**
      * A key was pressed or released.
      */
     GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED,
     /**
      * A mouse button was pressed or released.
      */
     GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED,
     /**
      * The mouse cursor entered or exited a window.
      */
     GEN_WINDOW_SYSTEM_EVENT_WINDOW_HOVER_CHANGED,
     /**
      * The mouse cursor moved.
      */
     GEN_WINDOW_SYSTEM_EVENT_MOUSE_CURSOR_MOVED,
     /**
      * A window's content was damaged/exposed and requires redrawing.
      */
     GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED,
     /**
      * An attribute of a window was modified.
      * @note This may loopback from `gen_window_modify`.
      */
     GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED,
     /**
      * The focus of a window changed.
      */
     GEN_WINDOW_SYSTEM_EVENT_FOCUS_CHANGED,
} gen_window_system_event_type_t;

/**
 * Identifier for a window.
 * @see gen_window_system_poll
 * @see gen_window_matches_id
 */
typedef xcb_window_t gen_window_id_t;

/**
 * A keycode from a keyboard event.
 */
typedef enum {
     GEN_KEYCODE_NONE = -1,
     GEN_KEYCODE_SPACE = 0,
     GEN_KEYCODE_APOSTROPHE,
     GEN_KEYCODE_COMMA,
     GEN_KEYCODE_MINUS,
     GEN_KEYCODE_FULL_STOP,
     GEN_KEYCODE_SLASH,
     GEN_KEYCODE_0,
     GEN_KEYCODE_1,
     GEN_KEYCODE_2,
     GEN_KEYCODE_3,
     GEN_KEYCODE_4,
     GEN_KEYCODE_5,
     GEN_KEYCODE_6,
     GEN_KEYCODE_7,
     GEN_KEYCODE_8,
     GEN_KEYCODE_9,
     GEN_KEYCODE_SEMICOLON,
     GEN_KEYCODE_EQUAL,
     GEN_KEYCODE_A,
     GEN_KEYCODE_B,
     GEN_KEYCODE_C,
     GEN_KEYCODE_D,
     GEN_KEYCODE_E,
     GEN_KEYCODE_F,
     GEN_KEYCODE_G,
     GEN_KEYCODE_H,
     GEN_KEYCODE_I,
     GEN_KEYCODE_J,
     GEN_KEYCODE_K,
     GEN_KEYCODE_L,
     GEN_KEYCODE_M,
     GEN_KEYCODE_N,
     GEN_KEYCODE_O,
     GEN_KEYCODE_P,
     GEN_KEYCODE_Q,
     GEN_KEYCODE_R,
     GEN_KEYCODE_S,
     GEN_KEYCODE_T,
     GEN_KEYCODE_U,
     GEN_KEYCODE_V,
     GEN_KEYCODE_W,
     GEN_KEYCODE_X,
     GEN_KEYCODE_Y,
     GEN_KEYCODE_Z,
     GEN_KEYCODE_LEFT_BRACKET,
     GEN_KEYCODE_BACKSLASH,
     GEN_KEYCODE_RIGHT_BRACKET,
     GEN_KEYCODE_TILDE,
     GEN_KEYCODE_ESCAPE,
     GEN_KEYCODE_RETURN,
     GEN_KEYCODE_TAB,
     GEN_KEYCODE_BACKSPACE,
     GEN_KEYCODE_INSERT,
     GEN_KEYCODE_DELETE,
     GEN_KEYCODE_RIGHT,
     GEN_KEYCODE_LEFT,
     GEN_KEYCODE_DOWN,
     GEN_KEYCODE_UP,
     GEN_KEYCODE_PAGE_UP,
     GEN_KEYCODE_PAGE_DOWN,
     GEN_KEYCODE_HOME,
     GEN_KEYCODE_END,
     GEN_KEYCODE_CAPS_LOCK,
     GEN_KEYCODE_SCROLL_LOCK,
     GEN_KEYCODE_NUM_LOCK,
     GEN_KEYCODE_PRINT_SCREEN,
     GEN_KEYCODE_F1,
     GEN_KEYCODE_F2,
     GEN_KEYCODE_F3,
     GEN_KEYCODE_F4,
     GEN_KEYCODE_F5,
     GEN_KEYCODE_F6,
     GEN_KEYCODE_F7,
     GEN_KEYCODE_F8,
     GEN_KEYCODE_F9,
     GEN_KEYCODE_F10,
     GEN_KEYCODE_F11,
     GEN_KEYCODE_F12,
     GEN_KEYCODE_KEYPAD_0,
     GEN_KEYCODE_KEYPAD_1,
     GEN_KEYCODE_KEYPAD_2,
     GEN_KEYCODE_KEYPAD_3,
     GEN_KEYCODE_KEYPAD_4,
     GEN_KEYCODE_KEYPAD_5,
     GEN_KEYCODE_KEYPAD_6,
     GEN_KEYCODE_KEYPAD_7,
     GEN_KEYCODE_KEYPAD_8,
     GEN_KEYCODE_KEYPAD_9,
     GEN_KEYCODE_KEYPAD_FULL_STOP,
     GEN_KEYCODE_KEYPAD_DIVIDE,
     GEN_KEYCODE_KEYPAD_MULTIPLY,
     GEN_KEYCODE_KEYPAD_SUBTRACT,
     GEN_KEYCODE_KEYPAD_ADD,
     GEN_KEYCODE_KEYPAD_RETURN,
     GEN_KEYCODE_KEYPAD_EQUAL,
     GEN_KEYCODE_LEFT_SHIFT,
     GEN_KEYCODE_LEFT_CONTROL,
     GEN_KEYCODE_LEFT_ALT,
     GEN_KEYCODE_LEFT_META,
     GEN_KEYCODE_RIGHT_SHIFT,
     GEN_KEYCODE_RIGHT_CONTROL,
     GEN_KEYCODE_RIGHT_ALT,
     GEN_KEYCODE_RIGHT_META,

     GEN_INTERNAL_KEYCODE_N_KEYCODES = GEN_KEYCODE_RIGHT_META
} gen_keycode_t;

/**
 * Enumeration of mouse buttons.
 */
typedef enum {
     GEN_MOUSE_BUTTON_1 = 1,
     GEN_MOUSE_BUTTON_2,
     GEN_MOUSE_BUTTON_3,
     GEN_MOUSE_BUTTON_4,
     GEN_MOUSE_BUTTON_5,
     GEN_MOUSE_BUTTON_6,
     GEN_MOUSE_BUTTON_7,
     GEN_MOUSE_BUTTON_8,
     GEN_MOUSE_BUTTON_9,

     GEN_MOUSE_BUTTON_MAX = GEN_MOUSE_BUTTON_9
} gen_mouse_button_t;

/**
 * Structure describing a window system event.
 */
typedef struct {
     /**
      * The type of the event.
      */
     gen_window_system_event_type_t type;
     union {
          /**
           * Whether the button or key is pressed.
           * Valid only if `type` is `GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED` or `GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED`.
           */
          bool pressed;
          /**
           * Whether the window is hovered.
           * Valid only if `type` is `GEN_WINDOW_SYSTEM_EVENT_WINDOW_HOVER_CHANGED`.
           */
          bool hovered;
          /**
           * Whether the window is focused.
           * Valid only if the `type` is `GEN_WINDOW_SYSTEM_EVENT_FOCUS_CHANGED`.
           */
          bool focused;
     };
     union {
          /**
           * The keycode of the key whose state changed.
           * Valid only if `type` is `GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED`.
           */
          gen_keycode_t keycode;
          /**
           * The mouse button whose state changed.
           * Valid only if `type` is `GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED`.
           */
          gen_mouse_button_t button;
          /**
           * The position of the mouse cursor.
           * Valid only if `type` is `GEN_WINDOW_SYSTEM_EVENT_MOUSE_CURSOR_MOVED`.
           */
          gint2 position;
          /**
           * The modified attribute of the window.
           * Valid only if `type` is `GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED`.
           */
          gen_window_attribute_t attribute;
     };
     /**
      * The id of the window this event occurred for.
      */
     gen_window_id_t window_id;
} gen_window_system_event_t;

/**
 * A window system used for creating and managing windows.
 */
typedef struct {
     xcb_connection_t* internal_connection;
     xcb_screen_t* internal_screen;

     /**
      * Underlying API may require the system to create multiple events for a single poll.
      * Here we queue the events to return immediately until the buffer is empty.
      */
     gen_window_system_event_t* internal_event_queue;
     size_t internal_event_queue_length;

     gen_keycode_t internal_key_mapping[UINT8_MAX];
     
     xcb_atom_t internal_net_supported_atom;
     xcb_atom_t internal_net_wm_icon_atom;
     xcb_atom_t internal_net_wm_state_fullsreen_atom;

     xcb_atom_t internal_motif_wm_hints_atom;
} gen_window_system_t;

/**
 * Handle for a window.
 */
typedef struct {
     xcb_window_t window;
} gen_window_t;

/**
 * Creates a window system for creating and managing windows.
 * Only one window system should exist per-application.
 * @param[out] out_window_system pointer to storage for the created window system. Should be cleaned up using `gen_window_system_destroy`.
 * @return an error code.
 */
GEN_ERRORABLE gen_window_system_create(gen_window_system_t* const restrict out_window_system);

/**
 * Destroys a window system.
 * @param[in] window_system the window system to destroy.
 * @return an error code.
 */
GEN_ERRORABLE gen_window_system_destroy(gen_window_system_t* const restrict window_system);

/**
 * Polls for the next event in a window system.
 * It should be called until no event is present.
 * @param[in] window_system the window system to poll in.
 * @param[out] out_event pointer to storage for the generated event.
 * @return an error code.
 */
GEN_ERRORABLE gen_window_system_poll(gen_window_system_t* const restrict window_system, gen_window_system_event_t* const restrict out_event);

/**
 * Creates a new window.
 * @param[in,out] window_system the window system to create a window in.
 * @param[out] out_window pointer to storage for the created window.
 * @param[in] bitdepth the bitdepth for the created window's surface.
 * @param[in] position the position to create the window at.
 * @param[in] extent the size of the window to create.
 * @return an error code.
 */
GEN_ERRORABLE gen_window_create(gen_window_system_t* const restrict window_system, gen_window_t* const restrict out_window, const gen_gfx_bitdepth_t bitdepth, const gint2 position, const gint2 extent);
/**
 * Destroys a window.
 * @param[in,out] window_system the window system to destroy a window in.
 * @param[in] window the window to destroy.
 * @return an error code.
 */
GEN_ERRORABLE gen_window_destroy(gen_window_system_t* const restrict window_system, gen_window_t* const restrict window);

/**
 * Modifies a window attribute.
 * @param[in,out] window_system the window system to modify the window in.
 * @param[in,out] window the window whose attributes should be modified.
 * @param[in] attribute the attribute to assign to the window.
 * @return an error code.
 */
GEN_ERRORABLE gen_window_modify(gen_window_system_t* const restrict window_system, const gen_window_t* const restrict window, const gen_window_attribute_t* const restrict attribute);

/**
 * Fetches a window attribute.
 * @param[in,out] window_system the window system to fetch the window attribute from.
 * @param[in,out] window the window whose attribute should be fetched.
 * @param[in] type the attribute type to fetch.
 * @param[out] out_attribute the fetched attribute from the window.
 * @return an error code.
 */
GEN_ERRORABLE gen_window_fetch(gen_window_system_t* const restrict window_system, const gen_window_t* const restrict window, const gen_window_attribute_type_t type, gen_window_attribute_t* const restrict out_attribute);

/**
 * Checks whether a window id matches a window.
 * Used for matching events from `gen_window_system_poll` to windows.
 * @param[in] window the window to match against.
 * @param[in] window_id the window id to match.
 * @param[out] out_match pointer to storage for the result of the match-check.
 * @return an error code.
 */
GEN_ERRORABLE gen_window_matches_id(const gen_window_t* const restrict window, const gen_window_id_t window_id, bool* const restrict out_match);

#endif
