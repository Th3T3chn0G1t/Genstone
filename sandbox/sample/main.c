// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <gendl.h>
#include <genfs.h>
#include <gengfx.h>
#include <genlocale.h>
#include <genproc.h>
#include <genstring.h>

int main(void) {
	GEN_FRAME_BEGIN(main);

	gen_window_system_t window_system = {0};
	gen_error_t error = gen_window_system_create(&window_system);
	GEN_REQUIRE_NO_ERROR(error);

	gen_window_t window = {0};
	error = gen_window_create(&window_system, &window, GEN_GFX_BITDEPTH_INHERIT, (gint2){0, 0}, (gint2){640, 480});
	GEN_REQUIRE_NO_ERROR(error);

	const gen_window_attribute_t attribute = {GEN_WINDOW_ATTRIBUTE_NAME, .name = "Genuwin"};
	error = gen_window_modify(&window_system, &window, &attribute);
	GEN_REQUIRE_NO_ERROR(error);

	bool run = true;
	while(run) {
		gen_window_system_event_t event = {0};
		do {
			error = gen_window_system_poll(&window_system, &event);
			GEN_REQUIRE_NO_ERROR(error);

			switch(event.type) {
				case GEN_WINDOW_SYSTEM_EVENT_NONE: break;
				case GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED: {
					switch(event.keycode) {
						case GEN_KEYCODE_NONE: glog(DEBUG, "GEN_KEYCODE_NONE"); break;
						case GEN_KEYCODE_SPACE: glog(DEBUG, "GEN_KEYCODE_SPACE"); break;
						case GEN_KEYCODE_APOSTROPHE: glog(DEBUG, "GEN_KEYCODE_APOSTROPHE"); break;
						case GEN_KEYCODE_COMMA: glog(DEBUG, "GEN_KEYCODE_COMMA"); break;
						case GEN_KEYCODE_MINUS: glog(DEBUG, "GEN_KEYCODE_MINUS"); break;
						case GEN_KEYCODE_FULL_STOP: glog(DEBUG, "GEN_KEYCODE_FULL_STOP"); break;
						case GEN_KEYCODE_SLASH: glog(DEBUG, "GEN_KEYCODE_SLASH"); break;
						case GEN_KEYCODE_0: glog(DEBUG, "GEN_KEYCODE_0"); break;
						case GEN_KEYCODE_1: glog(DEBUG, "GEN_KEYCODE_1"); break;
						case GEN_KEYCODE_2: glog(DEBUG, "GEN_KEYCODE_2"); break;
						case GEN_KEYCODE_3: glog(DEBUG, "GEN_KEYCODE_3"); break;
						case GEN_KEYCODE_4: glog(DEBUG, "GEN_KEYCODE_4"); break;
						case GEN_KEYCODE_5: glog(DEBUG, "GEN_KEYCODE_5"); break;
						case GEN_KEYCODE_6: glog(DEBUG, "GEN_KEYCODE_6"); break;
						case GEN_KEYCODE_7: glog(DEBUG, "GEN_KEYCODE_7"); break;
						case GEN_KEYCODE_8: glog(DEBUG, "GEN_KEYCODE_8"); break;
						case GEN_KEYCODE_9: glog(DEBUG, "GEN_KEYCODE_9"); break;
						case GEN_KEYCODE_SEMICOLON: glog(DEBUG, "GEN_KEYCODE_SEMICOLON"); break;
						case GEN_KEYCODE_EQUAL: glog(DEBUG, "GEN_KEYCODE_EQUAL"); break;
						case GEN_KEYCODE_A: glog(DEBUG, "GEN_KEYCODE_A"); break;
						case GEN_KEYCODE_B: glog(DEBUG, "GEN_KEYCODE_B"); break;
						case GEN_KEYCODE_C: glog(DEBUG, "GEN_KEYCODE_C"); break;
						case GEN_KEYCODE_D: glog(DEBUG, "GEN_KEYCODE_D"); break;
						case GEN_KEYCODE_E: glog(DEBUG, "GEN_KEYCODE_E"); break;
						case GEN_KEYCODE_F: glog(DEBUG, "GEN_KEYCODE_F"); break;
						case GEN_KEYCODE_G: glog(DEBUG, "GEN_KEYCODE_G"); break;
						case GEN_KEYCODE_H: glog(DEBUG, "GEN_KEYCODE_H"); break;
						case GEN_KEYCODE_I: glog(DEBUG, "GEN_KEYCODE_I"); break;
						case GEN_KEYCODE_J: glog(DEBUG, "GEN_KEYCODE_J"); break;
						case GEN_KEYCODE_K: glog(DEBUG, "GEN_KEYCODE_K"); break;
						case GEN_KEYCODE_L: glog(DEBUG, "GEN_KEYCODE_L"); break;
						case GEN_KEYCODE_M: glog(DEBUG, "GEN_KEYCODE_M"); break;
						case GEN_KEYCODE_N: glog(DEBUG, "GEN_KEYCODE_N"); break;
						case GEN_KEYCODE_O: glog(DEBUG, "GEN_KEYCODE_O"); break;
						case GEN_KEYCODE_P: glog(DEBUG, "GEN_KEYCODE_P"); break;
						case GEN_KEYCODE_Q: glog(DEBUG, "GEN_KEYCODE_Q"); break;
						case GEN_KEYCODE_R: glog(DEBUG, "GEN_KEYCODE_R"); break;
						case GEN_KEYCODE_S: glog(DEBUG, "GEN_KEYCODE_S"); break;
						case GEN_KEYCODE_T: glog(DEBUG, "GEN_KEYCODE_T"); break;
						case GEN_KEYCODE_U: glog(DEBUG, "GEN_KEYCODE_U"); break;
						case GEN_KEYCODE_V: glog(DEBUG, "GEN_KEYCODE_V"); break;
						case GEN_KEYCODE_W: glog(DEBUG, "GEN_KEYCODE_W"); break;
						case GEN_KEYCODE_X: glog(DEBUG, "GEN_KEYCODE_X"); break;
						case GEN_KEYCODE_Y: glog(DEBUG, "GEN_KEYCODE_Y"); break;
						case GEN_KEYCODE_Z: glog(DEBUG, "GEN_KEYCODE_Z"); break;
						case GEN_KEYCODE_LEFT_BRACKET: glog(DEBUG, "GEN_KEYCODE_LEFT_BRACKET"); break;
						case GEN_KEYCODE_BACKSLASH: glog(DEBUG, "GEN_KEYCODE_BACKSLASH"); break;
						case GEN_KEYCODE_RIGHT_BRACKET: glog(DEBUG, "GEN_KEYCODE_RIGHT_BRACKET"); break;
						case GEN_KEYCODE_TILDE: glog(DEBUG, "GEN_KEYCODE_TILDE"); break;
						case GEN_KEYCODE_ESCAPE: glog(DEBUG, "GEN_KEYCODE_ESCAPE"); break;
						case GEN_KEYCODE_RETURN: glog(DEBUG, "GEN_KEYCODE_RETURN"); break;
						case GEN_KEYCODE_TAB: glog(DEBUG, "GEN_KEYCODE_TAB"); break;
						case GEN_KEYCODE_BACKSPACE: glog(DEBUG, "GEN_KEYCODE_BACKSPACE"); break;
						case GEN_KEYCODE_INSERT: glog(DEBUG, "GEN_KEYCODE_INSERT"); break;
						case GEN_KEYCODE_DELETE: glog(DEBUG, "GEN_KEYCODE_DELETE"); break;
						case GEN_KEYCODE_RIGHT: glog(DEBUG, "GEN_KEYCODE_RIGHT"); break;
						case GEN_KEYCODE_LEFT: glog(DEBUG, "GEN_KEYCODE_LEFT"); break;
						case GEN_KEYCODE_DOWN: glog(DEBUG, "GEN_KEYCODE_DOWN"); break;
						case GEN_KEYCODE_UP: glog(DEBUG, "GEN_KEYCODE_UP"); break;
						case GEN_KEYCODE_PAGE_UP: glog(DEBUG, "GEN_KEYCODE_PAGE_UP"); break;
						case GEN_KEYCODE_PAGE_DOWN: glog(DEBUG, "GEN_KEYCODE_PAGE_DOWN"); break;
						case GEN_KEYCODE_HOME: glog(DEBUG, "GEN_KEYCODE_HOME"); break;
						case GEN_KEYCODE_END: glog(DEBUG, "GEN_KEYCODE_END"); break;
						case GEN_KEYCODE_CAPS_LOCK: glog(DEBUG, "GEN_KEYCODE_CAPS_LOCK"); break;
						case GEN_KEYCODE_SCROLL_LOCK: glog(DEBUG, "GEN_KEYCODE_SCROLL_LOCK"); break;
						case GEN_KEYCODE_NUM_LOCK: glog(DEBUG, "GEN_KEYCODE_NUM_LOCK"); break;
						case GEN_KEYCODE_PRINT_SCREEN: glog(DEBUG, "GEN_KEYCODE_PRINT_SCREEN"); break;
						case GEN_KEYCODE_F1: glog(DEBUG, "GEN_KEYCODE_F1"); break;
						case GEN_KEYCODE_F2: glog(DEBUG, "GEN_KEYCODE_F2"); break;
						case GEN_KEYCODE_F3: glog(DEBUG, "GEN_KEYCODE_F3"); break;
						case GEN_KEYCODE_F4: glog(DEBUG, "GEN_KEYCODE_F4"); break;
						case GEN_KEYCODE_F5: glog(DEBUG, "GEN_KEYCODE_F5"); break;
						case GEN_KEYCODE_F6: glog(DEBUG, "GEN_KEYCODE_F6"); break;
						case GEN_KEYCODE_F7: glog(DEBUG, "GEN_KEYCODE_F7"); break;
						case GEN_KEYCODE_F8: glog(DEBUG, "GEN_KEYCODE_F8"); break;
						case GEN_KEYCODE_F9: glog(DEBUG, "GEN_KEYCODE_F9"); break;
						case GEN_KEYCODE_F10: glog(DEBUG, "GEN_KEYCODE_F10"); break;
						case GEN_KEYCODE_F11: glog(DEBUG, "GEN_KEYCODE_F11"); break;
						case GEN_KEYCODE_F12: glog(DEBUG, "GEN_KEYCODE_F12"); break;
						case GEN_KEYCODE_KEYPAD_0: glog(DEBUG, "GEN_KEYCODE_KEYPAD_0"); break;
						case GEN_KEYCODE_KEYPAD_1: glog(DEBUG, "GEN_KEYCODE_KEYPAD_1"); break;
						case GEN_KEYCODE_KEYPAD_2: glog(DEBUG, "GEN_KEYCODE_KEYPAD_2"); break;
						case GEN_KEYCODE_KEYPAD_3: glog(DEBUG, "GEN_KEYCODE_KEYPAD_3"); break;
						case GEN_KEYCODE_KEYPAD_4: glog(DEBUG, "GEN_KEYCODE_KEYPAD_4"); break;
						case GEN_KEYCODE_KEYPAD_5: glog(DEBUG, "GEN_KEYCODE_KEYPAD_5"); break;
						case GEN_KEYCODE_KEYPAD_6: glog(DEBUG, "GEN_KEYCODE_KEYPAD_6"); break;
						case GEN_KEYCODE_KEYPAD_7: glog(DEBUG, "GEN_KEYCODE_KEYPAD_7"); break;
						case GEN_KEYCODE_KEYPAD_8: glog(DEBUG, "GEN_KEYCODE_KEYPAD_8"); break;
						case GEN_KEYCODE_KEYPAD_9: glog(DEBUG, "GEN_KEYCODE_KEYPAD_9"); break;
						case GEN_KEYCODE_KEYPAD_FULL_STOP: glog(DEBUG, "GEN_KEYCODE_KEYPAD_FULL_STOP"); break;
						case GEN_KEYCODE_KEYPAD_DIVIDE: glog(DEBUG, "GEN_KEYCODE_KEYPAD_DIVIDE"); break;
						case GEN_KEYCODE_KEYPAD_MULTIPLY: glog(DEBUG, "GEN_KEYCODE_KEYPAD_MULTIPLY"); break;
						case GEN_KEYCODE_KEYPAD_SUBTRACT: glog(DEBUG, "GEN_KEYCODE_KEYPAD_SUBTRACT"); break;
						case GEN_KEYCODE_KEYPAD_ADD: glog(DEBUG, "GEN_KEYCODE_KEYPAD_ADD"); break;
						case GEN_KEYCODE_KEYPAD_RETURN: glog(DEBUG, "GEN_KEYCODE_KEYPAD_RETURN"); break;
						case GEN_KEYCODE_KEYPAD_EQUAL: glog(DEBUG, "GEN_KEYCODE_KEYPAD_EQUAL"); break;
						case GEN_KEYCODE_LEFT_SHIFT: glog(DEBUG, "GEN_KEYCODE_LEFT_SHIFT"); break;
						case GEN_KEYCODE_LEFT_CONTROL: glog(DEBUG, "GEN_KEYCODE_LEFT_CONTROL"); break;
						case GEN_KEYCODE_LEFT_ALT: glog(DEBUG, "GEN_KEYCODE_LEFT_ALT"); break;
						case GEN_KEYCODE_LEFT_META: glog(DEBUG, "GEN_KEYCODE_LEFT_META"); break;
						case GEN_KEYCODE_RIGHT_SHIFT: glog(DEBUG, "GEN_KEYCODE_RIGHT_SHIFT"); break;
						case GEN_KEYCODE_RIGHT_CONTROL: glog(DEBUG, "GEN_KEYCODE_RIGHT_CONTROL"); break;
						case GEN_KEYCODE_RIGHT_ALT: glog(DEBUG, "GEN_KEYCODE_RIGHT_ALT"); break;
						case GEN_KEYCODE_RIGHT_META: glog(DEBUG, "GEN_KEYCODE_RIGHT_META"); break;
					}
					break;
				}
				case GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED: {
					// glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED");
					run = false;
					break;
				}
				case GEN_WINDOW_SYSTEM_EVENT_WINDOW_HOVER_CHANGED: {
					// glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_WINDOW_HOVER_CHANGED");
					break;
				}
				case GEN_WINDOW_SYSTEM_EVENT_MOUSE_CURSOR_MOVED: {
					// glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_MOUSE_CURSOR_MOVED");
					break;
				}
				case GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED: {
					// glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED");
					break;
				}
				case GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED: {
					switch(event.attribute.type) {
						case GEN_WINDOW_ATTRIBUTE_VISIBILITY: {
							glogf(DEBUG, "Window %s", event.attribute.visible ? "shown" : "hidden");
							break;
						}
						case GEN_WINDOW_ATTRIBUTE_FULLSCREEN: {
							glogf(DEBUG, "Window %s", event.attribute.fullscreen ? "fullscreened" : "windowed");
							break;
						}
						case GEN_WINDOW_ATTRIBUTE_DECORATION: {
							glogf(DEBUG, "Window %s", event.attribute.decorated ? "decorated" : "undecorated");
							break;
						}
						case GEN_WINDOW_ATTRIBUTE_POSITION: {
							glogf(DEBUG, "Window position updated to (%i, %i)", event.attribute.position.x, event.attribute.position.y);
							break;
						}
						case GEN_WINDOW_ATTRIBUTE_EXTENT: {
							glogf(DEBUG, "Window extent updated to %ix%i", event.attribute.extent.x, event.attribute.extent.y);
							break;
						}
						case GEN_WINDOW_ATTRIBUTE_NAME: {
							glogf(DEBUG, "Window name updated to %s", event.attribute.name);
							break;
						}
						case GEN_WINDOW_ATTRIBUTE_ICON: {
							glog(DEBUG, "Window icon updated");
							break;
						}
					}
					break;
				}
				case GEN_WINDOW_SYSTEM_EVENT_FOCUS_CHANGED: {
					// glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_FOCUS_CHANGED");
					break;
				}
			}
		} while(event.type != GEN_WINDOW_SYSTEM_EVENT_NONE);
	}

	error = gen_window_destroy(&window_system, &window);
	GEN_REQUIRE_NO_ERROR(error);

	error = gen_window_system_destroy(&window_system);
	GEN_REQUIRE_NO_ERROR(error);
}

// #include <gencommon.h>

// int main(void) {
// 	GEN_FRAME_BEGIN(main);

// 	glog(INFO, "Hello, Genstone!");
// }

// #include <gencommon.h>
// #include <gendl.h>
// #include <genfs.h>
// #include <genlocale.h>
// #include <genproc.h>
// #include <genstring.h>
// #include <genwindow.h>

// #define PAYLOAD_0 ""
// #define PAYLOAD_1 "a"
// #define PAYLOAD_4095 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
// #define PAYLOAD_4096 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

// int main(void) {
// 	GEN_FRAME_BEGIN(main);

// 	glog(INFO, "Hello, Genstone!");

// gen_filesystem_handle_t file = {0};
// gen_error_t error = gen_filesystem_handle_open(&file, "test.txt");
// GEN_REQUIRE_NO_ERROR(error);
// gen_filesystem_handle_t handle = {0};
// error = gen_filewatch_create(&handle, &file);
// GEN_REQUIRE_NO_ERROR(error);

// while(true) {
// 	gen_filewatch_event_t e = {0};
// 	error = gen_filewatch_poll(&handle, &e);
// 	GEN_REQUIRE_NO_ERROR(error);

// 	switch(e) {
// 		case GEN_FILEWATCH_CREATED:
// 		case GEN_FILEWATCH_MODIFIED:
// 		case GEN_FILEWATCH_DELETED:
// 		case GEN_FILEWATCH_MOVED:
// 			glog(DEBUG, "Filesystem event");
// 			break;
// 		case GEN_FILEWATCH_NONE: break;
// 	}
// }

// gen_error_t error = GEN_OK;

// char append[3] = {0};
// error = gen_string_append(append, sizeof(append), PAYLOAD_1, sizeof(PAYLOAD_1), sizeof(PAYLOAD_1) - 1);
// GEN_REQUIRE_NO_ERROR(error);
// error = gen_string_append(append, sizeof(append), PAYLOAD_1, sizeof(PAYLOAD_1), sizeof(PAYLOAD_1) - 1);
// GEN_REQUIRE_NO_ERROR(error);
// glogf(DEBUG, "%s", append);

// char destination[4096 + 1] = {0};
// size_t length = 0;
// bool equal = false;

// error = gen_string_copy(destination, sizeof(destination), PAYLOAD_0, sizeof(PAYLOAD_0), sizeof(PAYLOAD_0) - 1);
// GEN_REQUIRE_NO_ERROR(error);
// error = gen_string_length(destination, sizeof(destination), GEN_STRING_NO_BOUND, &length);
// GEN_REQUIRE_NO_ERROR(error);
// GEN_REQUIRE_EQUAL(sizeof(PAYLOAD_0) - 1, length);
// error = gen_string_compare(destination, sizeof(destination), PAYLOAD_0, sizeof(PAYLOAD_0), sizeof(PAYLOAD_0) - 1, &equal);
// GEN_REQUIRE_NO_ERROR(error);
// GEN_REQUIRE_EQUAL(true, equal);

// error = gen_string_copy(destination, sizeof(destination), PAYLOAD_1, sizeof(PAYLOAD_1), sizeof(PAYLOAD_1) - 1);
// GEN_REQUIRE_NO_ERROR(error);
// error = gen_string_length(destination, sizeof(destination), GEN_STRING_NO_BOUND, &length);
// GEN_REQUIRE_NO_ERROR(error);
// GEN_REQUIRE_EQUAL(sizeof(PAYLOAD_1) - 1, length);
// error = gen_string_compare(destination, sizeof(destination), PAYLOAD_1, sizeof(PAYLOAD_1), sizeof(PAYLOAD_1) - 1, &equal);
// GEN_REQUIRE_NO_ERROR(error);
// GEN_REQUIRE_EQUAL(true, equal);

// error = gen_string_copy(destination, sizeof(destination), PAYLOAD_4095, sizeof(PAYLOAD_4095), sizeof(PAYLOAD_4095) - 1);
// GEN_REQUIRE_NO_ERROR(error);
// error = gen_string_length(destination, sizeof(destination), GEN_STRING_NO_BOUND, &length);
// GEN_REQUIRE_NO_ERROR(error);
// GEN_REQUIRE_EQUAL(sizeof(PAYLOAD_4095) - 1, length);
// error = gen_string_compare(destination, sizeof(destination), PAYLOAD_4095, sizeof(PAYLOAD_4095), sizeof(PAYLOAD_4095) - 1, &equal);
// GEN_REQUIRE_NO_ERROR(error);
// GEN_REQUIRE_EQUAL(true, equal);

// error = gen_string_copy(destination, sizeof(destination), PAYLOAD_4096, sizeof(PAYLOAD_4096), sizeof(PAYLOAD_4096) - 1);
// GEN_REQUIRE_NO_ERROR(error);
// error = gen_string_length(destination, sizeof(destination), GEN_STRING_NO_BOUND, &length);
// GEN_REQUIRE_NO_ERROR(error);
// GEN_REQUIRE_EQUAL(sizeof(PAYLOAD_4096) - 1, length);
// error = gen_string_compare(destination, sizeof(destination), PAYLOAD_4096, sizeof(PAYLOAD_4096), sizeof(PAYLOAD_4096) - 1, &equal);
// GEN_REQUIRE_NO_ERROR(error);
// GEN_REQUIRE_EQUAL(true, equal);

// while(true) {
// 	// gen_window_system_event_t event = {0};
// 	do {
// 		error = gen_window_system_poll(&window_system, &event);
// 		GEN_REQUIRE_NO_ERROR(error);

// 		switch(event.type) {
// 			case GEN_WINDOW_SYSTEM_EVENT_NONE: break;
// 	} while(event.type != GEN_WINDOW_SYSTEM_EVENT_NONE);

// const gen_window_attribute_t attribute = {GEN_WINDOW_ATTRIBUTE_POSITION, .position = position};
// error = gen_window_modify(&window_system, &window, &attribute);
// GEN_REQUIRE_NO_ERROR(error);
// position.x++;
// 	}
// }
