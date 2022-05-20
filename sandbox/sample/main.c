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

	gen_gfx_context_t context = {0};
	error = gen_gfx_context_create(&context);
	GEN_REQUIRE_NO_ERROR(error);

	const struct {
		gfloat4 position;
		gfloat4 color;
	} triangle_vertices[] = {
		{{0.0f, -0.5f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
		{{0.5f, 0.5f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}};

	gen_gfx_draw_buffer_t triangle = {0};
	error = gen_gfx_vertex_buffer_create(&context, (uint8_t*) triangle_vertices, sizeof(triangle_vertices), &triangle);
	GEN_REQUIRE_NO_ERROR(error);

	gen_gfx_shader_t vertex = {0};
	gen_gfx_shader_t fragment = {0};
	const gen_gfx_shader_t* shaders[] = {&vertex, &fragment};

	gen_filesystem_handle_t vertex_handle = {0};
	error = gen_filesystem_handle_open(&vertex_handle, "sandbox/sample/vertex.spv");
	GEN_REQUIRE_NO_ERROR(error);
	size_t vertex_size = 0;
	error = gen_filesystem_handle_size(&vertex_size, &vertex_handle);
	GEN_REQUIRE_NO_ERROR(error);
	uint8_t* vertex_shader_data = NULL;
	error = gzalloc((void**) &vertex_shader_data, vertex_size, sizeof(uint8_t));
	GEN_REQUIRE_NO_ERROR(error);
	error = gen_filesystem_handle_read(vertex_shader_data, &vertex_handle, 0, vertex_size);
	GEN_REQUIRE_NO_ERROR(error);
	error = gen_gfx_shader_create(&context, &vertex, vertex_shader_data, vertex_size);
	GEN_REQUIRE_NO_ERROR(error);
	vertex.type = GEN_GFX_SHADER_TYPE_VERTEX;
	error = gfree(vertex_shader_data);
	GEN_REQUIRE_NO_ERROR(error);
	error = gen_filesystem_handle_close(&vertex_handle);
	GEN_REQUIRE_NO_ERROR(error);

	gen_filesystem_handle_t fragment_handle = {0};
	error = gen_filesystem_handle_open(&fragment_handle, "sandbox/sample/fragment.spv");
	GEN_REQUIRE_NO_ERROR(error);
	size_t fragment_size = 0;
	error = gen_filesystem_handle_size(&fragment_size, &fragment_handle);
	GEN_REQUIRE_NO_ERROR(error);
	uint8_t* fragment_shader_data = NULL;
	error = gzalloc((void**) &fragment_shader_data, fragment_size, sizeof(uint8_t));
	GEN_REQUIRE_NO_ERROR(error);
	error = gen_filesystem_handle_read(fragment_shader_data, &fragment_handle, 0, fragment_size);
	GEN_REQUIRE_NO_ERROR(error);
	error = gen_gfx_shader_create(&context, &fragment, fragment_shader_data, fragment_size);
	fragment.type = GEN_GFX_SHADER_TYPE_FRAGMENT;
	GEN_REQUIRE_NO_ERROR(error);
	error = gfree(fragment_shader_data);
	GEN_REQUIRE_NO_ERROR(error);
	error = gen_filesystem_handle_close(&fragment_handle);
	GEN_REQUIRE_NO_ERROR(error);

	gen_gfx_targetable_t targetable = {0};
	error = gen_gfx_targetable_create(&context, &window_system, &window, &targetable);
	GEN_REQUIRE_NO_ERROR(error);

	gen_gfx_pipeline_t pipeline = {0};
	error = gen_gfx_pipeline_create(&context, &targetable, &pipeline, shaders, sizeof(shaders) / sizeof(shaders[0]));
	GEN_REQUIRE_NO_ERROR(error);

	error = gen_gfx_shader_destroy(&context, &vertex);
	GEN_REQUIRE_NO_ERROR(error);
	error = gen_gfx_shader_destroy(&context, &fragment);
	GEN_REQUIRE_NO_ERROR(error);

	bool run = true;
	while(run) {
		gen_window_system_event_t event = {0};
		do {
			error = gen_window_system_poll(&window_system, &event);
			GEN_REQUIRE_NO_ERROR(error);

			switch(event.type) {
				case GEN_WINDOW_SYSTEM_EVENT_NONE: break;
				case GEN_WINDOW_SYSTEM_EVENT_KEY_STATE_CHANGED: break;
				case GEN_WINDOW_SYSTEM_EVENT_MOUSE_BUTTON_STATE_CHANGED: {
					run = false;
					break;
				}
				case GEN_WINDOW_SYSTEM_EVENT_WINDOW_HOVER_CHANGED: break;
				case GEN_WINDOW_SYSTEM_EVENT_MOUSE_CURSOR_MOVED: break;
				case GEN_WINDOW_SYSTEM_EVENT_WINDOW_CONTENT_DAMAGED: break;
				case GEN_WINDOW_SYSTEM_EVENT_WINDOW_ATTRIBUTE_CHANGED: {
					switch(event.attribute.type) {
						case GEN_WINDOW_ATTRIBUTE_VISIBILITY: break;
						case GEN_WINDOW_ATTRIBUTE_FULLSCREEN: break;
						case GEN_WINDOW_ATTRIBUTE_DECORATION: break;
						case GEN_WINDOW_ATTRIBUTE_POSITION: break;
						case GEN_WINDOW_ATTRIBUTE_EXTENT: {
							error = gen_gfx_targetable_geometry_update(&context, &window_system, &window, &targetable, event.attribute.extent);
							GEN_REQUIRE_NO_ERROR(error);
							break;
						}
						case GEN_WINDOW_ATTRIBUTE_NAME: break;
						case GEN_WINDOW_ATTRIBUTE_ICON: break;
					}
					break;
				}
				case GEN_WINDOW_SYSTEM_EVENT_FOCUS_CHANGED: break;
			}
		} while(event.type != GEN_WINDOW_SYSTEM_EVENT_NONE);

		gen_gfx_frame_t frame = {0};

		error = gen_gfx_pipeline_frame_begin(&context, &targetable, &pipeline, (gfloat4){0.8f, 0.3f, 0.2f, 1.0f}, &frame);
		GEN_REQUIRE_NO_ERROR(error);

		error = gen_gfx_pipeline_frame_draw_vertex_buffer(&context, &targetable, &pipeline, &triangle, &frame);
		GEN_REQUIRE_NO_ERROR(error);

		error = gen_gfx_pipeline_frame_end(&context, &targetable, &pipeline, &frame);
		GEN_REQUIRE_NO_ERROR(error);
	}

	error = gen_gfx_context_await(&context);
	GEN_REQUIRE_NO_ERROR(error);

	error = gen_gfx_vertex_buffer_destroy(&context, &triangle);
	GEN_REQUIRE_NO_ERROR(error);

	error = gen_gfx_pipeline_destroy(&context, &targetable, &pipeline);
	GEN_REQUIRE_NO_ERROR(error);

	error = gen_gfx_targetable_destroy(&context, &window_system, &window, &targetable);
	GEN_REQUIRE_NO_ERROR(error);

	error = gen_gfx_context_destroy(&context);
	GEN_REQUIRE_NO_ERROR(error);

	error = gen_window_destroy(&window_system, &window);
	GEN_REQUIRE_NO_ERROR(error);

	error = gen_window_system_destroy(&window_system);
	GEN_REQUIRE_NO_ERROR(error);
}
