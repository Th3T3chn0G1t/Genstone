#include "include/genuine.h"

#include <gentooling.h>

gen_error_t gen_ui_draw_ninepatch_direct(void* const restrict ninepatch, const gen_ui_draw_handler_t draw_handler, const gen_ui_rect_t extent, const gen_ui_extent_t src_scale, const gen_ui_extent_t dest_scale, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_ui_draw_ninepatch_direct);

	if(!draw_handler) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`draw_handler` was NULL");
	if(!extent.w || !extent.h || !src_scale || !dest_scale) GEN_ALL_OK; // No work can be done but its not technically an error

	// Top Left
	draw_handler(ninepatch, (gen_ui_rect_t){0, 0, src_scale, src_scale}, (gen_ui_rect_t){extent.x, extent.y, dest_scale, dest_scale}, passthrough);

	// Top Right
	draw_handler(ninepatch, (gen_ui_rect_t){2 * src_scale, 0, src_scale, src_scale}, (gen_ui_rect_t){extent.x + (extent.w - dest_scale) + dest_scale, extent.y, dest_scale, dest_scale}, passthrough);

	// Bottom Left
	draw_handler(ninepatch, (gen_ui_rect_t){0, 2 * src_scale, src_scale, src_scale}, (gen_ui_rect_t){extent.x, extent.y + (extent.h - dest_scale) + dest_scale, dest_scale, dest_scale}, passthrough);

	// Bottom Right
	draw_handler(ninepatch, (gen_ui_rect_t){2 * src_scale, 2 * src_scale, src_scale, src_scale}, (gen_ui_rect_t){extent.x + (extent.w - dest_scale) + dest_scale, extent.y + (extent.h - dest_scale) + dest_scale, dest_scale, dest_scale}, passthrough);

	// Top
	for(register gen_ui_extent_t i = 0; i < (extent.w - dest_scale) / dest_scale; ++i) {
		draw_handler(ninepatch, (gen_ui_rect_t){src_scale, 0, src_scale, src_scale}, (gen_ui_rect_t){extent.x + (i * dest_scale) + dest_scale, extent.y, dest_scale, dest_scale}, passthrough);
	}
	draw_handler(ninepatch, (gen_ui_rect_t){src_scale, 0, (gen_ui_extent_t) ((double) ((extent.w - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale)), src_scale}, (gen_ui_rect_t){extent.x + (((extent.w - dest_scale) / dest_scale) * dest_scale) + dest_scale, extent.y, (extent.w - dest_scale) % dest_scale, dest_scale}, passthrough);

	// Bottom
	for(register gen_ui_extent_t i = 0; i < (extent.w - dest_scale) / dest_scale; ++i) {
		draw_handler(ninepatch, (gen_ui_rect_t){src_scale, 2 * src_scale, src_scale, src_scale}, (gen_ui_rect_t){extent.x + (i * dest_scale) + dest_scale, extent.y + (extent.h - dest_scale) + dest_scale, dest_scale, dest_scale}, passthrough);
	}
	draw_handler(ninepatch, (gen_ui_rect_t){src_scale, 2 * src_scale, (gen_ui_extent_t) ((double) ((extent.w - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale)), src_scale}, (gen_ui_rect_t){extent.x + (((extent.w - dest_scale) / dest_scale) * dest_scale) + dest_scale, extent.y + (extent.h - dest_scale) + dest_scale, (extent.w - dest_scale) % dest_scale, dest_scale}, passthrough);

	// Left
	for(register gen_ui_extent_t i = 0; i < (extent.h - dest_scale) / dest_scale; ++i) {
		draw_handler(ninepatch, (gen_ui_rect_t){0, src_scale, src_scale, src_scale}, (gen_ui_rect_t){extent.x, extent.y + (i * dest_scale) + dest_scale, dest_scale, dest_scale}, passthrough);
	}
	draw_handler(ninepatch, (gen_ui_rect_t){0, src_scale, src_scale, (gen_ui_extent_t) ((double) ((extent.h - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale))}, (gen_ui_rect_t){extent.x, extent.y + (((extent.h - dest_scale) / dest_scale) * dest_scale) + dest_scale, dest_scale, (extent.h - dest_scale) % dest_scale}, passthrough);

	// Right
	for(register gen_ui_extent_t i = 0; i < (extent.h - dest_scale) / dest_scale; ++i) {
		draw_handler(ninepatch, (gen_ui_rect_t){2 * src_scale, src_scale, src_scale, src_scale}, (gen_ui_rect_t){extent.x + (extent.w - dest_scale) + dest_scale, extent.y + (i * dest_scale) + dest_scale, dest_scale, dest_scale}, passthrough);
	}
	draw_handler(ninepatch, (gen_ui_rect_t){2 * src_scale, src_scale, src_scale, (gen_ui_extent_t) ((double) ((extent.h - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale))}, (gen_ui_rect_t){extent.x + (extent.w - dest_scale) + dest_scale, extent.y + (((extent.h - dest_scale) / dest_scale) * dest_scale) + dest_scale, dest_scale, (extent.h - dest_scale) % dest_scale}, passthrough);

	// Centre
	for(register gen_ui_extent_t i = 0; i < (extent.w - dest_scale) / dest_scale; ++i) {
		for(register gen_ui_extent_t j = 0; j < (extent.h - dest_scale) / dest_scale; ++j) {
			draw_handler(ninepatch, (gen_ui_rect_t){src_scale, src_scale, src_scale, src_scale}, (gen_ui_rect_t){extent.x + (i * dest_scale) + dest_scale, extent.y + (j * dest_scale) + dest_scale, dest_scale, dest_scale}, passthrough);
			draw_handler(ninepatch, (gen_ui_rect_t){src_scale, src_scale, (gen_ui_extent_t) ((double) ((extent.w - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale)), src_scale}, (gen_ui_rect_t){extent.x + (extent.w / dest_scale) * dest_scale, extent.y + (j * dest_scale) + dest_scale, (extent.w - dest_scale) % dest_scale, dest_scale}, passthrough);
		}
		draw_handler(ninepatch, (gen_ui_rect_t){src_scale, src_scale, src_scale, (gen_ui_extent_t) ((double) ((extent.h - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale))}, (gen_ui_rect_t){extent.x + (i * dest_scale) + dest_scale, extent.y + (extent.h / dest_scale) * dest_scale, dest_scale, (extent.h - dest_scale) % dest_scale}, passthrough);
	}
	draw_handler(ninepatch, (gen_ui_rect_t){src_scale, src_scale, (gen_ui_extent_t) ((double) ((extent.w - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale)), (gen_ui_extent_t) ((double) ((extent.h - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale))}, (gen_ui_rect_t){extent.x + (extent.w / dest_scale) * dest_scale, extent.y + (extent.h / dest_scale) * dest_scale, (extent.w - dest_scale) % dest_scale, (extent.h - dest_scale) % dest_scale}, passthrough);

	GEN_ALL_OK;
}
