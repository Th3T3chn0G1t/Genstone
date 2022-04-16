// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <gendl.h>
#include <genfs.h>
#include <genlocale.h>
#include <genproc.h>
#include <genstring.h>
#include <genuwin.h>

int main(void) {
	GEN_FRAME_BEGIN(main);

	glog(INFO, "Hello, Genstone!");

	gen_error_t error = GEN_OK;

	gen_window_system_t window_system = {0};
	error = gen_window_system_create(&window_system);
	GEN_REQUIRE_NO_ERROR(error);

	gen_window_system_event_t event = {0};
	do {
		error = gen_window_system_poll(&window_system, &event);
		GEN_REQUIRE_NO_ERROR(error);

		switch(event.type) {
			case GEN_WINDOW_SYSTEM_EVENT_NONE: break;
			case GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED"); break;
			case GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED"); break;
			case GEN_WINDOW_SYSTEM_EVENT_WINDOW_HOVER_CHANGED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_WINDOW_HOVER_CHANGED"); break;
			case GEN_WINDOW_SYSTEM_EVENT_MOUSE_CURSOR_MOVED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_MOUSE_CURSOR_MOVED"); break;
			case GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED"); break;
			case GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED"); break;
			case GEN_WINDOW_SYSTEM_EVENT_FOCUS_CHANGED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_FOCUS_CHANGED"); break;
		}
	} while(event.type != GEN_WINDOW_SYSTEM_EVENT_NONE);

	gint2 position = {0, 0};

	gen_window_t window = {0};
	error = gen_window_create(&window_system, &window, GEN_GFX_BITDEPTH_24, position, (gint2){640, 480});
	GEN_REQUIRE_NO_ERROR(error);

	gen_window_attribute_t name = {.type = GEN_WINDOW_ATTRIBUTE_NAME, .name = "Genuwin"};
	error = gen_window_modify(&window_system, &window, &name);
	GEN_REQUIRE_NO_ERROR(error);

	while(true) {
		// gen_window_system_event_t event = {0};
		do {
			error = gen_window_system_poll(&window_system, &event);
			GEN_REQUIRE_NO_ERROR(error);

			switch(event.type) {
				case GEN_WINDOW_SYSTEM_EVENT_NONE: break;
				case GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED"); break;
				case GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED"); break;
				case GEN_WINDOW_SYSTEM_EVENT_WINDOW_HOVER_CHANGED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_WINDOW_HOVER_CHANGED"); break;
				case GEN_WINDOW_SYSTEM_EVENT_MOUSE_CURSOR_MOVED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_MOUSE_CURSOR_MOVED"); break;
				case GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED"); break;
				case GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED"); break;
				case GEN_WINDOW_SYSTEM_EVENT_FOCUS_CHANGED: glog(DEBUG, "GEN_WINDOW_SYSTEM_EVENT_FOCUS_CHANGED"); break;
			}
		} while(event.type != GEN_WINDOW_SYSTEM_EVENT_NONE);

		// const gen_window_attribute_t attribute = {GEN_WINDOW_ATTRIBUTE_POSITION, .position = position};
		// error = gen_window_modify(&window_system, &window, &attribute);
		// GEN_REQUIRE_NO_ERROR(error);
		// position.x++;
	}
}
