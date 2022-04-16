#include "include/genuwin.h"

#include <gemory.h>
#include <genstring.h>

enum
{
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

// GEN_INTERNAL_ERRORABLE gen_internal_window_system_get_atom(gen_window_system_t* const restrict window_system, const char* const restrict atom_name, xcb_atom_t** const restrict out_atom) {
// 	GEN_FRAME_BEGIN(gen_internal_window_system_get_atom);

// 	GEN_NULL_CHECK(window_system);
// 	GEN_NULL_CHECK(atom_name);

// 	gen_error_t error = gen_internal_window_system_check(window_system);
// 	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_check` failed");

// 	size_t atom_name_length = 0;
// 	error = gen_string_length(atom_name, GEN_STRING_NO_BOUND, GEN_STRING_NO_BOUND, &atom_name_length);
// 	GEN_ERROR_OUT_IF(error, "`gen_string_length` failed");

// 	xcb_intern_atom_cookie_t atom_cookie = xcb_intern_atom(window_system->internal_connection, true, atom_name_length, atom_name);
// 	xcb_intern_atom_reply_t* atom_reply = xcb_intern_atom_reply(window_system->internal_connection, atom_cookie, NULL);
// 	if(!atom_reply) GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "The requested atom does not exist");
// 	*out_atom = atom_reply->atom;
// 	free(atom_reply);

// 	GEN_ALL_OK;
// }

gen_error_t gen_window_system_create(gen_window_system_t* const restrict out_window_system) {
	GEN_FRAME_BEGIN(gen_window_system_create);
	GEN_NULL_CHECK(out_window_system);

	out_window_system->internal_connection = xcb_connect(NULL, NULL);
	gen_error_t error = gen_internal_window_system_check(out_window_system);
	GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_check` failed");

	const xcb_setup_t* const setup = xcb_get_setup(out_window_system->internal_connection);
	out_window_system->internal_screen = xcb_setup_roots_iterator(setup).data;

	// error = gen_internal_window_system_get_atom(out_window_system, "_NET_SUPPORTED", &out_window_system->internal_net_supported_atom);
	// GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_get_atom` failed");

	// error = gen_internal_window_system_get_atom(out_window_system, "_NET_SUPPORTED", &out_window_system->internal_net_supported_atom);
	// GEN_ERROR_OUT_IF(error, "`gen_internal_window_system_get_atom` failed");

	// xcb_get_property_cookie_t property_cookie = xcb_get_property(out_window_system->connection, false, out_window_system->screen->root, out_window_system->internal_net_supported_atom, XCB_ATOM_ATOM, 0, 0);
	// xcb_get_property_reply_t* property_reply = xcb_get_property_reply(out_window_system->connection, property_cookie, NULL);
	// if(!property_reply) GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "The X server does not implement the minimum requirements for Genuwin to function");

	// free(property_reply);

	out_window_system->internal_event_queue_length = 0;

	int flushed = xcb_flush(out_window_system->internal_connection);
	if(flushed <= 0) GEN_ERROR_OUT(GEN_OPERATION_FAILED, "Failed to flush the connection");

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

	GEN_ALL_OK;
}

/*
GEN_WINDOW_SYSTEM_EVENT_NONE:
     XCB_EVENT_MASK_NO_EVENT

GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED:
     XCB_EVENT_MASK_KEY_PRESS
     XCB_EVENT_MASK_KEY_RELEASE

GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED:
     XCB_EVENT_MASK_BUTTON_PRESS
     XCB_EVENT_MASK_BUTTON_RELEASE

GEN_WINDOW_SYSTEM_EVENT_WINDOW_HOVER_CHANGED:
     XCB_EVENT_MASK_ENTER_WINDOW
     XCB_EVENT_MASK_LEAVE_WINDOW

GEN_WINDOW_SYSTEM_EVENT_MOUSE_CURSOR_MOVED:
     XCB_EVENT_MASK_POINTER_MOTION
     XCB_EVENT_MASK_POINTER_MOTION_HINT (?)
     XCB_EVENT_MASK_BUTTON_1_MOTION (?)
     XCB_EVENT_MASK_BUTTON_2_MOTION (?)
     XCB_EVENT_MASK_BUTTON_3_MOTION (?)
     XCB_EVENT_MASK_BUTTON_4_MOTION (?)
     XCB_EVENT_MASK_BUTTON_5_MOTION (?)
     XCB_EVENT_MASK_BUTTON_MOTION (?)

[INTERNAL]:
     XCB_EVENT_MASK_KEYMAP_STATE
     XCB_EVENT_MASK_COLOR_MAP_CHANGE
     XCB_EVENT_MASK_OWNER_GRAB_BUTTON

GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED:
     XCB_EVENT_MASK_EXPOSURE

GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED:
     XCB_EVENT_MASK_VISIBILITY_CHANGE
     XCB_EVENT_MASK_STRUCTURE_NOTIFY (?)
     XCB_EVENT_MASK_RESIZE_REDIRECT
     XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY (?)
     XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT (?)
     XCB_EVENT_MASK_PROPERTY_CHANGE

GEN_WINDOW_SYSTEM_EVENT_FOCUS_CHANGED:
     XCB_EVENT_MASK_FOCUS_CHANGE
*/

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
		error = grealloc((void**) &window_system->internal_event_queue, window_system->internal_event_queue_length + 1, window_system->internal_event_queue_length, sizeof(gen_window_system_event_t));
		GEN_ERROR_OUT_IF(error, "`grealloc` failed");

		GEN_ALL_OK;
	}

	xcb_generic_event_t* event = xcb_poll_for_event(window_system->internal_connection);

	if(event) {
		switch(event->response_type & ~0x80) {
			case XCB_KEY_PRESS: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED;

				xcb_key_press_event_t* e = (xcb_key_press_event_t*) event;
				out_event->window_id = e->event;

				out_event->keycode = e->detail;
				out_event->pressed = true;

				break;
			}
			case XCB_KEY_RELEASE: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED;

				xcb_key_release_event_t* e = (xcb_key_release_event_t*) event;
				out_event->window_id = e->event;

				out_event->keycode = e->detail;
				out_event->pressed = false;

				break;
			}
			case XCB_BUTTON_PRESS: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED;

				xcb_button_press_event_t* e = (xcb_button_press_event_t*) event;
				out_event->window_id = e->event;

				if(e->detail & XCB_BUTTON_MASK_1)
					out_event->button = GEN_MOUSE_BUTTON_1;
				else if(e->detail & XCB_BUTTON_MASK_2)
					out_event->button = GEN_MOUSE_BUTTON_2;
				else if(e->detail & XCB_BUTTON_MASK_3)
					out_event->button = GEN_MOUSE_BUTTON_3;
				else if(e->detail & XCB_BUTTON_MASK_4)
					out_event->button = GEN_MOUSE_BUTTON_4;
				else if(e->detail & XCB_BUTTON_MASK_5)
					out_event->button = GEN_MOUSE_BUTTON_5;
				else
					GEN_ERROR_OUT(GEN_BAD_CONTENT, "An invalid mouse button event was recieved");
				out_event->pressed = true;

				break;
			}
			case XCB_BUTTON_RELEASE: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED;

				xcb_button_release_event_t* e = (xcb_button_release_event_t*) event;
				out_event->window_id = e->event;

				if(e->detail & XCB_BUTTON_MASK_1)
					out_event->button = GEN_MOUSE_BUTTON_1;
				else if(e->detail & XCB_BUTTON_MASK_2)
					out_event->button = GEN_MOUSE_BUTTON_2;
				else if(e->detail & XCB_BUTTON_MASK_3)
					out_event->button = GEN_MOUSE_BUTTON_3;
				else if(e->detail & XCB_BUTTON_MASK_4)
					out_event->button = GEN_MOUSE_BUTTON_4;
				else if(e->detail & XCB_BUTTON_MASK_5)
					out_event->button = GEN_MOUSE_BUTTON_5;
				else
					GEN_ERROR_OUT(GEN_BAD_CONTENT, "An invalid mouse button event was recieved");
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
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Modify internal window system state");

				__unused xcb_keymap_notify_event_t* e = (xcb_keymap_notify_event_t*) event;

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
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Modify internal window system state");

				__unused xcb_create_notify_event_t* e = (xcb_create_notify_event_t*) event;

				break;
			}
			case XCB_DESTROY_NOTIFY: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Modify internal window system state");

				__unused xcb_destroy_notify_event_t* e = (xcb_destroy_notify_event_t*) event;

				break;
			}
			case XCB_UNMAP_NOTIFY: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED;

				xcb_unmap_notify_event_t* e = (xcb_unmap_notify_event_t*) event;
				out_event->window_id = e->window;

				out_event->attribute = (gen_window_attribute_t){.visible = false};

				break;
			}
			case XCB_MAP_NOTIFY: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED;

				xcb_map_notify_event_t* e = (xcb_map_notify_event_t*) event;
				out_event->window_id = e->window;

				out_event->attribute = (gen_window_attribute_t){.visible = true};

				break;
			}
			case XCB_MAP_REQUEST: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Responding to requests is unsupported");

				__unused xcb_map_request_event_t* e = (xcb_map_request_event_t*) event;

				break;
			}
			case XCB_REPARENT_NOTIFY: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Reparenting is unsupported");

				__unused xcb_reparent_notify_event_t* e = (xcb_reparent_notify_event_t*) event;

				break;
			}
			case XCB_CONFIGURE_NOTIFY: {
				out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED;

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
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Responding to requests is unsupported");

				__unused xcb_configure_request_event_t* e = (xcb_configure_request_event_t*) event;

				break;
			}
			case XCB_GRAVITY_NOTIFY: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Window gravity is unsupported");

				__unused xcb_gravity_notify_event_t* e = (xcb_gravity_notify_event_t*) event;

				break;
			}
			case XCB_RESIZE_REQUEST: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Responding to requests is unsupported");

				__unused xcb_resize_request_event_t* e = (xcb_resize_request_event_t*) event;

				break;
			}
			case XCB_CIRCULATE_NOTIFY: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Event circulation is unsupported");

				__unused xcb_circulate_notify_event_t* e = (xcb_circulate_notify_event_t*) event;

				break;
			}
			case XCB_CIRCULATE_REQUEST: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Event circulation is unsupported");

				__unused xcb_circulate_request_event_t* e = (xcb_circulate_request_event_t*) event;

				break;
			}
			case XCB_PROPERTY_NOTIFY: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Atom properties are not yet implemented");

				//out_event->type = GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED;
				__unused xcb_property_notify_event_t* e = (xcb_property_notify_event_t*) event;

				break;
			}
			case XCB_SELECTION_CLEAR: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Selection is unsupported");

				__unused xcb_selection_clear_event_t* e = (xcb_selection_clear_event_t*) event;

				break;
			}
			case XCB_SELECTION_REQUEST: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Selection is unsupported");

				__unused xcb_selection_request_event_t* e = (xcb_selection_request_event_t*) event;

				break;
			}
			case XCB_SELECTION_NOTIFY: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Selection is unsupported");

				__unused xcb_selection_notify_event_t* e = (xcb_selection_notify_event_t*) event;

				break;
			}
			case XCB_COLORMAP_NOTIFY: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Modify internal window system state");

				__unused xcb_colormap_notify_event_t* e = (xcb_colormap_notify_event_t*) event;

				break;
			}
			case XCB_CLIENT_MESSAGE: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Client events are unsupported");

				__unused xcb_client_message_event_t* e = (xcb_client_message_event_t*) event;

				break;
			}
			case XCB_MAPPING_NOTIFY: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Modify internal window system state");

				__unused xcb_mapping_notify_event_t* e = (xcb_mapping_notify_event_t*) event;

				break;
			}
			case XCB_GE_GENERIC: {
				GEN_ERROR_OUT(GEN_NOT_IMPLEMENTED, "Generic events are unsupported");

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

	out_event->type = GEN_WINDOW_SYSTEM_EVENT_NONE;
	out_event->window_id = 0;

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
