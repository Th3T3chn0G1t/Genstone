#include "include/genuwin.h"

#include <gemory.h>
#include <genstring.h>
#include <xcb/xkb.h>

enum {
	GEN_INTERNAL_DATA_FORMAT_CHAR = 8,
	GEN_INTERNAL_DATA_FORMAT_SHORT = 16,
	GEN_INTERNAL_DATA_FORMAT_INT = 32
};

GEN_INTERNAL_ERRORABLE gen_internal_window_system_check(gen_window_system_t* const restrict window_system) {
	GEN_FRAME_BEGIN(gen_internal_window_system_check);

	GEN_NULL_CHECK(window_system);

	int error = xcb_connection_has_error(window_system->internal_connection);
	switch(error) {
		case 0: GEN_ALL_OK;
		case XCB_CONN_ERROR: GEN_ERROR_OUT(GEN_BAD_CONTENT, "Failed to establish connection to window server");
		case XCB_CONN_CLOSED_EXT_NOTSUPPORTED: GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Failed to establish connection to window server");
		case XCB_CONN_CLOSED_MEM_INSUFFICIENT: GEN_ERROR_OUT(GEN_OUT_OF_MEMORY, "Failed to establish connection to window server");
		case XCB_CONN_CLOSED_REQ_LEN_EXCEED: GEN_ERROR_OUT(GEN_TOO_LONG, "Failed to establish connection to window server");
		case XCB_CONN_CLOSED_PARSE_ERR: GEN_ERROR_OUT(GEN_BAD_CONTENT, "Failed to establish connection to window server");
		case XCB_CONN_CLOSED_INVALID_SCREEN: GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "Failed to establish connection to window server");
		case XCB_CONN_CLOSED_FDPASSING_FAILED: GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "Failed to establish connection to window server");
		default: GEN_ERROR_OUT(GEN_UNKNOWN, "Failed to establish connection to window server");
	}
}

static const char* hard_mapping[] = {
	[GEN_KEYCODE_SPACE] = "SPCE",
	[GEN_KEYCODE_APOSTROPHE] = "AC11",
	[GEN_KEYCODE_COMMA] = "AB08",
	[GEN_KEYCODE_MINUS] = "AE11",
	[GEN_KEYCODE_FULL_STOP] = "AB09",
	[GEN_KEYCODE_SLASH] = "AB10",
	[GEN_KEYCODE_0] = "AE10",
	[GEN_KEYCODE_1] = "AE01",
	[GEN_KEYCODE_2] = "AE02",
	[GEN_KEYCODE_3] = "AE03",
	[GEN_KEYCODE_4] = "AE04",
	[GEN_KEYCODE_5] = "AE05",
	[GEN_KEYCODE_6] = "AE06",
	[GEN_KEYCODE_7] = "AE07",
	[GEN_KEYCODE_8] = "AE08",
	[GEN_KEYCODE_9] = "AE09",
	[GEN_KEYCODE_SEMICOLON] = "AC10",
	[GEN_KEYCODE_EQUAL] = "AE12",
	[GEN_KEYCODE_A] = "AC01",
	[GEN_KEYCODE_B] = "AB05",
	[GEN_KEYCODE_C] = "AB03",
	[GEN_KEYCODE_D] = "AC03",
	[GEN_KEYCODE_E] = "AD03",
	[GEN_KEYCODE_F] = "AC04",
	[GEN_KEYCODE_G] = "AC05",
	[GEN_KEYCODE_H] = "AC06",
	[GEN_KEYCODE_I] = "AD08",
	[GEN_KEYCODE_J] = "AC07",
	[GEN_KEYCODE_K] = "AC08",
	[GEN_KEYCODE_L] = "AC09",
	[GEN_KEYCODE_M] = "AB07",
	[GEN_KEYCODE_N] = "AB06",
	[GEN_KEYCODE_O] = "AD09",
	[GEN_KEYCODE_P] = "AD10",
	[GEN_KEYCODE_Q] = "AD01",
	[GEN_KEYCODE_R] = "AD04",
	[GEN_KEYCODE_S] = "AC02",
	[GEN_KEYCODE_T] = "AD05",
	[GEN_KEYCODE_U] = "AD07",
	[GEN_KEYCODE_V] = "AB04",
	[GEN_KEYCODE_W] = "AD02",
	[GEN_KEYCODE_X] = "AB02",
	[GEN_KEYCODE_Y] = "AD06",
	[GEN_KEYCODE_Z] = "AB01",
	[GEN_KEYCODE_LEFT_BRACKET] = "AD11",
	[GEN_KEYCODE_BACKSLASH] = "BKSL",
	[GEN_KEYCODE_RIGHT_BRACKET] = "AD12",
	[GEN_KEYCODE_TILDE] = "TLDE",
	[GEN_KEYCODE_ESCAPE] = "ESC",
	[GEN_KEYCODE_RETURN] = "RTRN",
	[GEN_KEYCODE_TAB] = "TAB",
	[GEN_KEYCODE_BACKSPACE] = "BKSP",
	[GEN_KEYCODE_INSERT] = "INS",
	[GEN_KEYCODE_DELETE] = "DELE",
	[GEN_KEYCODE_RIGHT] = "RGHT",
	[GEN_KEYCODE_LEFT] = "LEFT",
	[GEN_KEYCODE_DOWN] = "DOWN",
	[GEN_KEYCODE_UP] = "UP",
	[GEN_KEYCODE_PAGE_UP] = "PGUP",
	[GEN_KEYCODE_PAGE_DOWN] = "PGDN",
	[GEN_KEYCODE_HOME] = "HOME",
	[GEN_KEYCODE_END] = "END",
	[GEN_KEYCODE_CAPS_LOCK] = "CAPS",
	[GEN_KEYCODE_SCROLL_LOCK] = "SCLK",
	[GEN_KEYCODE_NUM_LOCK] = "NMLK",
	[GEN_KEYCODE_PRINT_SCREEN] = "PRSC",
	[GEN_KEYCODE_F1] = "FK01",
	[GEN_KEYCODE_F2] = "FK02",
	[GEN_KEYCODE_F3] = "FK03",
	[GEN_KEYCODE_F4] = "FK04",
	[GEN_KEYCODE_F5] = "FK05",
	[GEN_KEYCODE_F6] = "FK06",
	[GEN_KEYCODE_F7] = "FK07",
	[GEN_KEYCODE_F8] = "FK08",
	[GEN_KEYCODE_F9] = "FK09",
	[GEN_KEYCODE_F10] = "FK10",
	[GEN_KEYCODE_F11] = "FK11",
	[GEN_KEYCODE_F12] = "FK12",
	[GEN_KEYCODE_KEYPAD_0] = "KP0",
	[GEN_KEYCODE_KEYPAD_1] = "KP1",
	[GEN_KEYCODE_KEYPAD_2] = "KP2",
	[GEN_KEYCODE_KEYPAD_3] = "KP3",
	[GEN_KEYCODE_KEYPAD_4] = "KP4",
	[GEN_KEYCODE_KEYPAD_5] = "KP5",
	[GEN_KEYCODE_KEYPAD_6] = "KP6",
	[GEN_KEYCODE_KEYPAD_7] = "KP7",
	[GEN_KEYCODE_KEYPAD_8] = "KP8",
	[GEN_KEYCODE_KEYPAD_9] = "KP9",
	[GEN_KEYCODE_KEYPAD_FULL_STOP] = "KPDL",
	[GEN_KEYCODE_KEYPAD_DIVIDE] = "KPDV",
	[GEN_KEYCODE_KEYPAD_MULTIPLY] = "KPMU",
	[GEN_KEYCODE_KEYPAD_SUBTRACT] = "KPSU",
	[GEN_KEYCODE_KEYPAD_ADD] = "KPAD",
	[GEN_KEYCODE_KEYPAD_RETURN] = "KPEN",
	[GEN_KEYCODE_KEYPAD_EQUAL] = "KPEQ",
	[GEN_KEYCODE_LEFT_SHIFT] = "LFSH",
	[GEN_KEYCODE_LEFT_CONTROL] = "LCTL",
	[GEN_KEYCODE_LEFT_ALT] = "LALT",
	[GEN_KEYCODE_LEFT_META] = "LWIN",
	[GEN_KEYCODE_RIGHT_SHIFT] = "RTSH",
	[GEN_KEYCODE_RIGHT_CONTROL] = "RCTL",
	[GEN_KEYCODE_RIGHT_ALT] = "RALT",
	[GEN_KEYCODE_RIGHT_META] = "RWIN"};

// https://gist.github.com/bluetech/6038239
// Thank you for your service - documenting the undocumented
GEN_INTERNAL_ERRORABLE gen_internal_generate_keymap(gen_window_system_t* const restrict window_system) {
	GEN_FRAME_BEGIN(gen_internal_generate_keymap);

	GEN_NULL_CHECK(window_system);

	xcb_xkb_get_names_cookie_t cookie = xcb_xkb_get_names(window_system->internal_connection, XCB_XKB_ID_USE_CORE_KBD, XCB_XKB_NAME_DETAIL_KEY_NAMES);
	xcb_xkb_get_names_reply_t* reply = xcb_xkb_get_names_reply(window_system->internal_connection, cookie, NULL);
	if(!reply) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to get key names");
	xcb_xkb_get_names_value_list_t list = {0};
	void* buffer = xcb_xkb_get_names_value_list(reply);
	xcb_xkb_get_names_value_list_unpack(buffer, reply->nTypes, reply->indicators, reply->virtualMods, reply->groupNames, reply->nKeys, reply->nKeyAliases, reply->nRadioGroups, reply->which, &list);

	int length = xcb_xkb_get_names_value_list_key_names_length(reply, &list);

	gen_error_t error = gen_memory_set(window_system->internal_key_mapping, sizeof(window_system->internal_key_mapping), (unsigned char) GEN_KEYCODE_NONE);
	GEN_ERROR_OUT_IF(error, "`gen_memory_set` failed");

	for(int i = 0; i < length; ++i) {
		GEN_FOREACH_PTR(j, name, sizeof(hard_mapping) / sizeof(hard_mapping[0]), hard_mapping) {
			bool equal = false;
			error = gen_string_compare(*name, GEN_STRING_NO_BOUND, list.keyNames[i].name, sizeof(list.keyNames[i].name), sizeof(list.keyNames[i].name), &equal);
			GEN_ERROR_OUT_IF(error, "`gen_string_compare` failed");
			if(equal) {
				window_system->internal_key_mapping[i + reply->minKeyCode] = (gen_keycode_t) j;
				break;
			}
		}
	}

	free(reply);

	GEN_ALL_OK;
}

GEN_INTERNAL_ERRORABLE gen_internal_window_system_validate_request_cookie(gen_window_system_t* const restrict window_system, const xcb_void_cookie_t cookie) {
	GEN_FRAME_BEGIN(gen_internal_window_system_validate_request_cookie);
	GEN_NULL_CHECK(window_system);

	gen_error_t error = gen_internal_window_system_check(window_system);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_check` failed");

	if(!cookie.sequence) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "The provided request cookie was invalid");
	xcb_generic_error_t* xcb_error = xcb_request_check(window_system->internal_connection, cookie);
	if(xcb_error) {
		uint8_t error_code = xcb_error->error_code;
		free(xcb_error);
		switch(error_code) {
			case XCB_REQUEST: GEN_ERROR_OUT(GEN_BAD_OPERATION, "A nonexistent request was sent");
			case XCB_VALUE: GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "Provided data was outside of acceptable range");
			case XCB_WINDOW: GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "A nonexistent window was referenced");
			case XCB_PIXMAP: GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "A nonexistent image was referenced");
			case XCB_ATOM: GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "An nonexistent property identifier was referenced");
			case XCB_CURSOR: GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "A nonexistent cursor was referenced");
			case XCB_FONT: GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "A nonexistent font was referenced");
			case XCB_MATCH: GEN_ERROR_OUT(GEN_WRONG_OBJECT_TYPE, "An object-parameter type mismatch was encountered");
			case XCB_DRAWABLE: GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "A nonexistent drawing surface was referenced");
			case XCB_ACCESS: GEN_ERROR_OUT(GEN_PERMISSION, "A window system permission was violated");
			case XCB_ALLOC: GEN_ERROR_OUT(GEN_OUT_OF_MEMORY, "The window system failed to allocate");
			case XCB_COLORMAP: GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "A nonexistent colormap was referenced");
			case XCB_G_CONTEXT: GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "A nonexistent drawing context was referenced");
			case XCB_ID_CHOICE: GEN_ERROR_OUT(GEN_ALREADY_EXISTS, "Application tried to reuse existing identifier identifier");
			case XCB_NAME: GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "A nonexistent object was referenced");
			case XCB_LENGTH: GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "Provided data was of the wrong length");
			case XCB_IMPLEMENTATION: GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "A requested window server feature is not supported");
			default: GEN_ERROR_OUT(GEN_UNKNOWN, "An unknown windowing error occurred");
		}
	}

	GEN_ALL_OK;
}

__unused static const char* gen_internal_get_event_name(const xcb_generic_event_t* const restrict event) {
	GEN_FRAME_BEGIN(gen_internal_get_event_name);

	switch(event->response_type & ~0x80) {
		case XCB_KEY_PRESS: return "XCB_KEY_PRESS";
		case XCB_KEY_RELEASE: return "XCB_KEY_RELEASE";
		case XCB_BUTTON_PRESS: return "XCB_BUTTON_PRESS";
		case XCB_BUTTON_RELEASE: return "XCB_BUTTON_RELEASE";
		case XCB_MOTION_NOTIFY: return "XCB_MOTION_NOTIFY";
		case XCB_ENTER_NOTIFY: return "XCB_ENTER_NOTIFY";
		case XCB_LEAVE_NOTIFY: return "XCB_LEAVE_NOTIFY";
		case XCB_FOCUS_IN: return "XCB_FOCUS_IN";
		case XCB_FOCUS_OUT: return "XCB_FOCUS_OUT";
		case XCB_KEYMAP_NOTIFY: return "XCB_KEYMAP_NOTIFY";
		case XCB_EXPOSE: return "XCB_EXPOSE";
		case XCB_GRAPHICS_EXPOSURE: return "XCB_GRAPHICS_EXPOSURE";
		case XCB_NO_EXPOSURE: return "XCB_NO_EXPOSURE";
		case XCB_VISIBILITY_NOTIFY: return "XCB_VISIBILITY_NOTIFY";
		case XCB_CREATE_NOTIFY: return "XCB_CREATE_NOTIFY";
		case XCB_DESTROY_NOTIFY: return "XCB_DESTROY_NOTIFY";
		case XCB_UNMAP_NOTIFY: return "XCB_UNMAP_NOTIFY";
		case XCB_MAP_NOTIFY: return "XCB_MAP_NOTIFY";
		case XCB_MAP_REQUEST: return "XCB_MAP_REQUEST";
		case XCB_REPARENT_NOTIFY: return "XCB_REPARENT_NOTIFY";
		case XCB_CONFIGURE_NOTIFY: return "XCB_CONFIGURE_NOTIFY";
		case XCB_CONFIGURE_REQUEST: return "XCB_CONFIGURE_REQUEST";
		case XCB_GRAVITY_NOTIFY: return "XCB_GRAVITY_NOTIFY";
		case XCB_RESIZE_REQUEST: return "XCB_RESIZE_REQUEST";
		case XCB_CIRCULATE_NOTIFY: return "XCB_CIRCULATE_NOTIFY";
		case XCB_CIRCULATE_REQUEST: return "XCB_CIRCULATE_REQUEST";
		case XCB_PROPERTY_NOTIFY: return "XCB_PROPERTY_NOTIFY";
		case XCB_SELECTION_CLEAR: return "XCB_SELECTION_CLEAR";
		case XCB_SELECTION_REQUEST: return "XCB_SELECTION_REQUEST";
		case XCB_SELECTION_NOTIFY: return "XCB_SELECTION_NOTIFY";
		case XCB_COLORMAP_NOTIFY: return "XCB_COLORMAP_NOTIFY";
		case XCB_CLIENT_MESSAGE: return "XCB_CLIENT_MESSAGE";
		case XCB_MAPPING_NOTIFY: return "XCB_MAPPING_NOTIFY";
		case XCB_GE_GENERIC: return "XCB_GE_GENERIC";
	}

	return NULL;
}

__unused GEN_INTERNAL_ERRORABLE gen_internal_get_atom_name(const gen_window_system_t* const restrict window_system, const xcb_atom_t atom, char** const restrict out_name) {
	GEN_FRAME_BEGIN(gen_internal_get_atom_name);

	GEN_NULL_CHECK(window_system);
	GEN_NULL_CHECK(out_name);

	xcb_get_atom_name_cookie_t cookie = xcb_get_atom_name(window_system->internal_connection, atom);
	xcb_get_atom_name_reply_t* reply = xcb_get_atom_name_reply(window_system->internal_connection, cookie, NULL);
	if(!reply) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "Could not find the provided atom's name");

	size_t atom_name_length = reply->name_len;
	char* atom_name = xcb_get_atom_name_name(reply);

	gen_error_t error = gen_string_duplicate(atom_name, atom_name_length, atom_name_length, out_name);
	GEN_ERROR_OUT_IF(error, "`gen_string_duplicate` failed");

	free(reply);

	GEN_ALL_OK;
}

GEN_INTERNAL_ERRORABLE gen_internal_window_system_get_atom(gen_window_system_t* const restrict window_system, const char* const restrict atom_name, xcb_atom_t* const restrict out_atom) {
	GEN_FRAME_BEGIN(gen_internal_window_system_get_atom);

	GEN_NULL_CHECK(window_system);
	GEN_NULL_CHECK(atom_name);

	gen_error_t error = gen_internal_window_system_check(window_system);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_check` failed");

	size_t atom_name_length = 0;
	error = gen_string_length(atom_name, GEN_STRING_NO_BOUND, GEN_STRING_NO_BOUND, &atom_name_length);
	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

	xcb_intern_atom_cookie_t atom_cookie = xcb_intern_atom(window_system->internal_connection, true, (uint16_t) atom_name_length, atom_name);
	xcb_intern_atom_reply_t* atom_reply = xcb_intern_atom_reply(window_system->internal_connection, atom_cookie, NULL);
	if(!atom_reply) GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "The requested atom does not exist");
	*out_atom = atom_reply->atom;
	free(atom_reply);

	GEN_ALL_OK;
}

gen_error_t gen_window_system_create(gen_window_system_t* const restrict out_window_system) {
	GEN_FRAME_BEGIN(gen_window_system_create);
	GEN_NULL_CHECK(out_window_system);

	out_window_system->internal_connection = xcb_connect(NULL, NULL);
	gen_error_t error = gen_internal_window_system_check(out_window_system);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_check` failed");

	const xcb_setup_t* const setup = xcb_get_setup(out_window_system->internal_connection);
	out_window_system->internal_screen = xcb_setup_roots_iterator(setup).data;

	const xcb_query_extension_reply_t* query_reply = xcb_get_extension_data(out_window_system->internal_connection, &xcb_xkb_id);
	if(!query_reply) GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "The X server does not implement XKB");
	xcb_xkb_use_extension_cookie_t extension_cookie = xcb_xkb_use_extension(out_window_system->internal_connection, XCB_XKB_MAJOR_VERSION, XCB_XKB_MINOR_VERSION);
	xcb_xkb_use_extension_reply_t* extension_reply = xcb_xkb_use_extension_reply(out_window_system->internal_connection, extension_cookie, NULL);
	if(!extension_reply) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to use XKB");
	if(!extension_reply->supported) GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "The X server does not implement XKB or does not support the minimum required verson");
	free(extension_reply);

	error = gen_internal_window_system_get_atom(out_window_system, "_MOTIF_WM_HINTS", &out_window_system->internal_motif_wm_hints_atom);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_get_atom` failed");

	error = gen_internal_window_system_get_atom(out_window_system, "_NET_SUPPORTED", &out_window_system->internal_net_supported_atom);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_get_atom` failed");
	glog(WARNING, "Proceeding with minimal `_NET_SUPPORTED` feature check. Errors may ensue for partially-supported X server implementations");

	error = gen_internal_window_system_get_atom(out_window_system, "_NET_WM_ICON", &out_window_system->internal_net_wm_icon_atom);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_get_atom` failed");
	error = gen_internal_window_system_get_atom(out_window_system, "_NET_WM_STATE_FULLSCREEN", &out_window_system->internal_net_wm_state_fullsreen_atom);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_get_atom` failed");

	out_window_system->internal_event_queue_length = 0;

	int flushed = xcb_flush(out_window_system->internal_connection);
	if(flushed <= 0) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to flush the connection");

	error = gen_internal_generate_keymap(out_window_system);
	GEN_ERROR_OUT_IF(error, "`gen_internal_generate_keymap` failed");

	GEN_ALL_OK;
}

gen_error_t gen_window_system_destroy(gen_window_system_t* const restrict window_system) {
	GEN_FRAME_BEGIN(gen_window_system_destroy);
	GEN_NULL_CHECK(window_system);

	gen_error_t error = gen_internal_window_system_check(window_system);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_check` failed");

	int flushed = xcb_flush(window_system->internal_connection);
	if(flushed <= 0) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to flush the connection");

	xcb_disconnect(window_system->internal_connection);

	if(window_system->internal_event_queue) {
		error = gfree(window_system->internal_event_queue);
		GEN_ERROR_OUT_IF(error, "`gfree` failed");
	}

	GEN_ALL_OK;
}

gen_error_t gen_window_system_poll(gen_window_system_t* const restrict window_system, gen_window_system_event_t* const restrict out_event) {
	GEN_FRAME_BEGIN(gen_window_system_poll);
	GEN_NULL_CHECK(window_system);
	GEN_NULL_CHECK(out_event);

	gen_error_t error = gen_internal_window_system_check(window_system);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_check` failed");

	int flushed = xcb_flush(window_system->internal_connection);
	if(flushed <= 0) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to flush the connection");

	if(window_system->internal_event_queue_length) {
		*out_event = window_system->internal_event_queue[window_system->internal_event_queue_length - 1];

		window_system->internal_event_queue_length--;
		if(window_system->internal_event_queue_length) {
			error = grealloc((void**) &window_system->internal_event_queue, window_system->internal_event_queue_length + 1, window_system->internal_event_queue_length, sizeof(gen_window_system_event_t));
			GEN_ERROR_OUT_IF(error, "`grealloc` failed");
		}
		else {
			error = gfree(window_system->internal_event_queue);
			GEN_ERROR_OUT_IF(error, "`gfree` failed");
			window_system->internal_event_queue = NULL;
		}

		GEN_ALL_OK;
	}

	out_event->type = GEN_WINDOW_SYSTEM_EVENT_NONE;
	out_event->window_id = 0;

	xcb_generic_event_t* event = xcb_poll_for_event(window_system->internal_connection);

	if(event) {
		// glogf(DEBUG, "Event %s", gen_internal_get_event_name(event));

		switch(event->response_type & ~0x80) {
			case XCB_KEY_PRESS: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED;

				xcb_key_press_event_t* e = (xcb_key_press_event_t*) event;
				out_event->window_id = e->event;

				out_event->keycode = window_system->internal_key_mapping[e->detail];
				out_event->pressed = true;

				break;
			}
			case XCB_KEY_RELEASE: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED;

				xcb_key_release_event_t* e = (xcb_key_release_event_t*) event;
				out_event->window_id = e->event;

				out_event->keycode = window_system->internal_key_mapping[e->detail];
				out_event->pressed = false;

				break;
			}
			case XCB_BUTTON_PRESS: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED;

				xcb_button_press_event_t* e = (xcb_button_press_event_t*) event;
				out_event->window_id = e->event;

				if(e->detail > GEN_MOUSE_BUTTON_MAX) GEN_ERROR_OUT(GEN_BAD_CONTENT, "An invalid mouse button event was recieved");
				out_event->button = (gen_mouse_button_t) e->detail;

				out_event->pressed = true;

				break;
			}
			case XCB_BUTTON_RELEASE: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED;

				xcb_button_release_event_t* e = (xcb_button_release_event_t*) event;
				out_event->window_id = e->event;

				if(e->detail > GEN_MOUSE_BUTTON_MAX) GEN_ERROR_OUT(GEN_BAD_CONTENT, "An invalid mouse button event was recieved");
				out_event->button = (gen_mouse_button_t) e->detail;

				out_event->pressed = false;

				break;
			}
			case XCB_MOTION_NOTIFY: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_MOUSE_CURSOR_MOVED;

				xcb_motion_notify_event_t* e = (xcb_motion_notify_event_t*) event;
				out_event->window_id = e->event;

				out_event->position = (gint2){e->event_x, e->event_y};

				break;
			}
			case XCB_ENTER_NOTIFY: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_HOVER_CHANGED;

				xcb_enter_notify_event_t* e = (xcb_enter_notify_event_t*) event;
				out_event->window_id = e->event;

				out_event->hovered = true;

				break;
			}
			case XCB_LEAVE_NOTIFY: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_HOVER_CHANGED;

				xcb_leave_notify_event_t* e = (xcb_leave_notify_event_t*) event;
				out_event->window_id = e->event;

				out_event->hovered = false;

				break;
			}
			case XCB_FOCUS_IN: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_FOCUS_CHANGED;

				xcb_focus_in_event_t* e = (xcb_focus_in_event_t*) event;
				out_event->window_id = e->event;

				out_event->focused = true;

				break;
			}
			case XCB_FOCUS_OUT: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_FOCUS_CHANGED;

				xcb_focus_out_event_t* e = (xcb_focus_out_event_t*) event;
				out_event->window_id = e->event;

				out_event->focused = true;

				break;
			}
			case XCB_KEYMAP_NOTIFY: {
				// glogf(WARNING, "%s: Modify internal window system state at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_keymap_notify_event_t* e = (xcb_keymap_notify_event_t*) event;

				error = gen_internal_generate_keymap(window_system);
				GEN_ERROR_OUT_IF(error, "`gen_internal_generate_keymap` failed");

				break;
			}
			case XCB_EXPOSE: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED;

				xcb_expose_event_t* e = (xcb_expose_event_t*) event;
				out_event->window_id = e->window;

				break;
			}
			case XCB_GRAPHICS_EXPOSURE: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED;

				xcb_graphics_exposure_event_t* e = (xcb_graphics_exposure_event_t*) event;
				out_event->window_id = e->drawable;

				break;
			}
			case XCB_NO_EXPOSURE: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED;

				xcb_no_exposure_event_t* e = (xcb_no_exposure_event_t*) event;
				out_event->window_id = e->drawable;

				break;
			}
			case XCB_VISIBILITY_NOTIFY: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED;

				xcb_visibility_notify_event_t* e = (xcb_visibility_notify_event_t*) event;
				out_event->window_id = e->window;

				break;
			}
			case XCB_CREATE_NOTIFY: {
				// glogf(WARNING, "%s: Modify internal window system state at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_create_notify_event_t* e = (xcb_create_notify_event_t*) event;

				break;
			}
			case XCB_DESTROY_NOTIFY: {
				// glogf(WARNING, "%s: Modify internal window system state at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_destroy_notify_event_t* e = (xcb_destroy_notify_event_t*) event;

				break;
			}
			case XCB_UNMAP_NOTIFY: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED;

				xcb_unmap_notify_event_t* e = (xcb_unmap_notify_event_t*) event;
				out_event->window_id = e->window;

				out_event->attribute = (gen_window_attribute_t){.type = GEN_WINDOW_ATTRIBUTE_VISIBILITY, .visible = false};

				break;
			}
			case XCB_MAP_NOTIFY: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED;

				xcb_map_notify_event_t* e = (xcb_map_notify_event_t*) event;
				out_event->window_id = e->window;

				out_event->attribute = (gen_window_attribute_t){.type = GEN_WINDOW_ATTRIBUTE_VISIBILITY, .visible = true};

				break;
			}
			case XCB_MAP_REQUEST: {
				// glogf(WARNING, "%s: Responding to requests is unsupported at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_map_request_event_t* e = (xcb_map_request_event_t*) event;

				break;
			}
			case XCB_REPARENT_NOTIFY: {
				// glogf(WARNING, "%s: Reparenting is unsupported at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_reparent_notify_event_t* e = (xcb_reparent_notify_event_t*) event;

				break;
			}
			case XCB_CONFIGURE_NOTIFY: {
				xcb_configure_notify_event_t* e = (xcb_configure_notify_event_t*) event;
				out_event->window_id = e->window;

				window_system->internal_event_queue_length += 2;
				error = grealloc((void**) &window_system->internal_event_queue, window_system->internal_event_queue_length - 2, window_system->internal_event_queue_length, sizeof(gen_window_system_event_t));
				GEN_ERROR_OUT_IF(error, "`grealloc` failed");

				window_system->internal_event_queue[window_system->internal_event_queue_length - 2].type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED;
				window_system->internal_event_queue[window_system->internal_event_queue_length - 2].attribute = (gen_window_attribute_t){.type = GEN_WINDOW_ATTRIBUTE_POSITION, .position = (gint2){e->x, e->y}};
				window_system->internal_event_queue[window_system->internal_event_queue_length - 1].type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED;
				window_system->internal_event_queue[window_system->internal_event_queue_length - 1].attribute = (gen_window_attribute_t){.type = GEN_WINDOW_ATTRIBUTE_EXTENT, .extent = (gint2){e->width, e->height}};

				break;
			}
			case XCB_CONFIGURE_REQUEST: {
				// glogf(WARNING, "%s: Responding to requests is unsupported at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_configure_request_event_t* e = (xcb_configure_request_event_t*) event;

				break;
			}
			case XCB_GRAVITY_NOTIFY: {
				// glogf(WARNING, "%s: Window gravity is unsupported at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_gravity_notify_event_t* e = (xcb_gravity_notify_event_t*) event;

				break;
			}
			case XCB_RESIZE_REQUEST: {
				// glogf(WARNING, "%s: Responding to requests is unsupported at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_resize_request_event_t* e = (xcb_resize_request_event_t*) event;

				break;
			}
			case XCB_CIRCULATE_NOTIFY: {
				// glogf(WARNING, "%s: Event circulation is unsupported at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_circulate_notify_event_t* e = (xcb_circulate_notify_event_t*) event;

				break;
			}
			case XCB_CIRCULATE_REQUEST: {
				// glogf(WARNING, "%s: Event circulation is unsupported at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_circulate_request_event_t* e = (xcb_circulate_request_event_t*) event;

				break;
			}
			case XCB_PROPERTY_NOTIFY: {
				xcb_property_notify_event_t* e = (xcb_property_notify_event_t*) event;

				out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED;

				if(e->atom == XCB_ATOM_WM_NAME) {
					// GEN_WINDOW_ATTRIBUTE_NAME
				}
				else if(e->atom == window_system->internal_motif_wm_hints_atom) {
					// GEN_WINDOW_ATTRIBUTE_DECORATION
				}
				else if(e->atom == window_system->internal_net_wm_state_fullsreen_atom) {
					// GEN_WINDOW_ATTRIBUTE_FULLSCREEN
				}
				else if(e->atom == window_system->internal_net_wm_icon_atom) {
					// GEN_WINDOW_ATTRIBUTE_ICON
				}
				else {
					out_event->type = GEN_WINDOW_SYSTEM_EVENT_NONE;

					// char* atom_name = NULL;
					// error = gen_internal_get_atom_name(window_system, e->atom, &atom_name);
					// GEN_ERROR_OUT_IF(error, "`gen_internal_get_atom_name` failed");
					// glogf(DEBUG, "Recieved atomic property change for atom %s", atom_name);
					// error = gfree(atom_name);
					// GEN_ERROR_OUT_IF(error, "`gfree` failed");
				}

				break;
			}
			case XCB_SELECTION_CLEAR: {
				// glogf(WARNING, "%s: Selection is unsupported at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_selection_clear_event_t* e = (xcb_selection_clear_event_t*) event;

				break;
			}
			case XCB_SELECTION_REQUEST: {
				// glogf(WARNING, "%s: Selection is unsupported at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_selection_request_event_t* e = (xcb_selection_request_event_t*) event;

				break;
			}
			case XCB_SELECTION_NOTIFY: {
				// glogf(WARNING, "%s: Selection is unsupported at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_selection_notify_event_t* e = (xcb_selection_notify_event_t*) event;

				break;
			}
			case XCB_COLORMAP_NOTIFY: {
				// glogf(WARNING, "%s: Modify internal window system state at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_colormap_notify_event_t* e = (xcb_colormap_notify_event_t*) event;

				break;
			}
			case XCB_CLIENT_MESSAGE: {
				// glogf(WARNING, "%s: Client events are unsupported at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_client_message_event_t* e = (xcb_client_message_event_t*) event;

				break;
			}
			case XCB_MAPPING_NOTIFY: {
				// glogf(WARNING, "%s: Modify internal window system state at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_mapping_notify_event_t* e = (xcb_mapping_notify_event_t*) event;

				break;
			}
			case XCB_GE_GENERIC: {
				// glogf(WARNING, "%s: Generic events are unsupported at %s:%i", gen_error_name(GEN_NOT_IMPLEMENTED), __FILE__, __LINE__);

				__unused xcb_ge_generic_event_t* e = (xcb_ge_generic_event_t*) event;

				break;
			}
			default: {
				GEN_ERROR_OUT(GEN_UNKNOWN, "An unknown event occurred");
				break;
			}
		}
		free(event);
	}

	flushed = xcb_flush(window_system->internal_connection);
	if(flushed <= 0) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to flush the connection");

	GEN_ALL_OK;
}

gen_error_t gen_window_create(gen_window_system_t* const restrict window_system, gen_window_t* const restrict out_window, const gen_gfx_bitdepth_t bitdepth, const gint2 position, const gint2 extent) {
	GEN_FRAME_BEGIN(gen_window_create);
	GEN_NULL_CHECK(window_system);
	GEN_NULL_CHECK(out_window);

	gen_error_t error = gen_internal_window_system_check(window_system);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_check` failed");

	int flushed = xcb_flush(window_system->internal_connection);
	if(flushed <= 0) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to flush the connection");

	out_window->window = xcb_generate_id(window_system->internal_connection);
	xcb_create_window_value_list_t values = {
		.event_mask = XCB_EVENT_MASK_NO_EVENT | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_POINTER_MOTION_HINT | XCB_EVENT_MASK_BUTTON_1_MOTION | XCB_EVENT_MASK_BUTTON_2_MOTION | XCB_EVENT_MASK_BUTTON_3_MOTION | XCB_EVENT_MASK_BUTTON_4_MOTION | XCB_EVENT_MASK_BUTTON_5_MOTION | XCB_EVENT_MASK_BUTTON_MOTION | XCB_EVENT_MASK_KEYMAP_STATE | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_VISIBILITY_CHANGE | XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_RESIZE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_FOCUS_CHANGE | XCB_EVENT_MASK_PROPERTY_CHANGE | XCB_EVENT_MASK_COLOR_MAP_CHANGE | XCB_EVENT_MASK_OWNER_GRAB_BUTTON};
	xcb_void_cookie_t cookie = xcb_create_window_aux_checked(window_system->internal_connection, bitdepth == GEN_GFX_BITDEPTH_INHERIT ? XCB_COPY_FROM_PARENT : (uint8_t) bitdepth, out_window->window, window_system->internal_screen->root, (int16_t) position.x, (int16_t) position.y, (uint16_t) extent.x, (uint16_t) extent.y, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, window_system->internal_screen->root_visual, XCB_CW_EVENT_MASK, &values);
	error = gen_internal_window_system_validate_request_cookie(window_system, cookie);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_validate_request_cookie` failed");

	cookie = xcb_map_window_checked(window_system->internal_connection, out_window->window);
	error = gen_internal_window_system_validate_request_cookie(window_system, cookie);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_validate_request_cookie` failed");

	flushed = xcb_flush(window_system->internal_connection);
	if(flushed <= 0) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to flush the connection");

	GEN_ALL_OK;
}

gen_error_t gen_window_destroy(gen_window_system_t* const restrict window_system, gen_window_t* const restrict window) {
	GEN_FRAME_BEGIN(gen_window_destroy);
	GEN_NULL_CHECK(window_system);
	GEN_NULL_CHECK(window);

	gen_error_t error = gen_internal_window_system_check(window_system);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_check` failed");

	int flushed = xcb_flush(window_system->internal_connection);
	if(flushed <= 0) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to flush the connection");

	xcb_void_cookie_t cookie = xcb_destroy_window_checked(window_system->internal_connection, window->window);
	error = gen_internal_window_system_validate_request_cookie(window_system, cookie);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_validate_request_cookie` failed");

	flushed = xcb_flush(window_system->internal_connection);
	if(flushed <= 0) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to flush the connection");

	GEN_ALL_OK;
}

gen_error_t gen_window_modify(gen_window_system_t* const restrict window_system, const gen_window_t* const restrict window, const gen_window_attribute_t* const restrict attribute) {
	GEN_FRAME_BEGIN(gen_window_modify);
	GEN_NULL_CHECK(window_system);
	GEN_NULL_CHECK(window);

	gen_error_t error = gen_internal_window_system_check(window_system);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_check` failed");

	int flushed = xcb_flush(window_system->internal_connection);
	if(flushed <= 0) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to flush the connection");

	xcb_void_cookie_t cookie = {0};

	switch(attribute->type) {
		case GEN_WINDOW_ATTRIBUTE_VISIBILITY: {
			if(attribute->visible) {
				cookie = xcb_map_window_checked(window_system->internal_connection, window->window);
				error = gen_internal_window_system_validate_request_cookie(window_system, cookie);
				GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_validate_request_cookie` failed");
			}
			else {
				cookie = xcb_unmap_window_checked(window_system->internal_connection, window->window);
				error = gen_internal_window_system_validate_request_cookie(window_system, cookie);
				GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_validate_request_cookie` failed");
			}
			break;
		}
		case GEN_WINDOW_ATTRIBUTE_FULLSCREEN: {
			// https://gist.github.com/0xd61/43d2fa8b93c9656cca92956b70a5216f
			GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Not yet implemented!");
			break;
		}
		case GEN_WINDOW_ATTRIBUTE_DECORATION: {
			// https://stackoverflow.com/questions/28366896/how-to-remove-window-decorations-with-xcb
			GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Not yet implemented!");
			break;
		}
		case GEN_WINDOW_ATTRIBUTE_POSITION: {
			xcb_configure_window_value_list_t config = {0};
			config.x = attribute->position.x;
			config.y = attribute->position.y;
			cookie = xcb_configure_window_aux_checked(window_system->internal_connection, window->window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, &config);
			error = gen_internal_window_system_validate_request_cookie(window_system, cookie);
			GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_validate_request_cookie` failed");
			break;
		}
		case GEN_WINDOW_ATTRIBUTE_EXTENT: {
			xcb_configure_window_value_list_t config = {0};
			config.x = attribute->extent.x;
			config.y = attribute->extent.y;
			cookie = xcb_configure_window_aux_checked(window_system->internal_connection, window->window, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, &config);
			error = gen_internal_window_system_validate_request_cookie(window_system, cookie);
			GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_validate_request_cookie` failed");
			break;
		}
		case GEN_WINDOW_ATTRIBUTE_NAME: {
			gen_window_attribute_t visibility = {.type = GEN_WINDOW_ATTRIBUTE_VISIBILITY, .visible = false};
			error = gen_window_modify(window_system, window, &visibility);
			GEN_ERROR_OUT_IF(error, "`gen_window_modify` failed");

			size_t length = 0;
			error = gen_string_length(attribute->name, GEN_STRING_NO_BOUND, GEN_STRING_NO_BOUND, &length);
			GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

			cookie = xcb_change_property_checked(window_system->internal_connection, XCB_PROP_MODE_REPLACE, window->window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, GEN_INTERNAL_DATA_FORMAT_CHAR, (uint32_t) length, attribute->name);
			error = gen_internal_window_system_validate_request_cookie(window_system, cookie);
			GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_validate_request_cookie` failed");

			visibility.visible = true;
			error = gen_window_modify(window_system, window, &visibility);
			GEN_ERROR_OUT_IF(error, "`gen_window_modify` failed");
			break;
		}
		case GEN_WINDOW_ATTRIBUTE_ICON: {
			GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Not yet implemented!");

			// cookie = xcb_change_property_checked(window_system->internal_connection, XCB_PROP_MODE_REPLACE, window->window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, GEN_INTERNAL_DATA_FORMAT_CHAR, (uint32_t) length, attribute->name);
			// error = gen_internal_window_system_validate_request_cookie(window_system, cookie);
			// GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_validate_request_cookie` failed");

			break;
		}
	}

	flushed = xcb_flush(window_system->internal_connection);
	if(flushed <= 0) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to flush the connection");

	GEN_ALL_OK;
}

gen_error_t gen_window_matches_id(const gen_window_t* const restrict window, const gen_window_id_t window_id, bool* const restrict out_match) {
	GEN_FRAME_BEGIN(gen_window_matches_id);
	GEN_NULL_CHECK(window);
	GEN_NULL_CHECK(out_match);

	*out_match = window->window == window_id;

	GEN_ALL_OK;
}
