// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genuine.h"

#include <gentooling.h>

gen_error_t gen_ui_draw_ninepatch_direct(void* const restrict ninepatch, const gen_ui_draw_handler_t draw_handler, const gen_ui_rect_t extent, const gen_ui_extent_t src_scale, const gen_ui_extent_t dest_scale, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_ui_draw_ninepatch_direct);

	GEN_NULL_CHECK(draw_handler);
	if(!extent.z || !extent.w || !src_scale || !dest_scale) GEN_ALL_OK; // No work can be done but its not technically an error

	gen_error_t error = GEN_OK;

	// Top Left
	error = draw_handler(ninepatch, (gen_ui_rect_t){0, 0, src_scale, src_scale}, (gen_ui_rect_t){extent.x, extent.y, dest_scale, dest_scale}, passthrough);
	GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");

	// Top Right
	error = draw_handler(ninepatch, (gen_ui_rect_t){2 * src_scale, 0, src_scale, src_scale}, (gen_ui_rect_t){extent.x + (extent.z - dest_scale) + dest_scale, extent.y, dest_scale, dest_scale}, passthrough);
	GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");

	// Bottom Left
	error = draw_handler(ninepatch, (gen_ui_rect_t){0, 2 * src_scale, src_scale, src_scale}, (gen_ui_rect_t){extent.x, extent.y + (extent.w - dest_scale) + dest_scale, dest_scale, dest_scale}, passthrough);
	GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");

	// Bottom Right
	error = draw_handler(ninepatch, (gen_ui_rect_t){2 * src_scale, 2 * src_scale, src_scale, src_scale}, (gen_ui_rect_t){extent.x + (extent.z - dest_scale) + dest_scale, extent.y + (extent.w - dest_scale) + dest_scale, dest_scale, dest_scale}, passthrough);
	GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");

	// Top
	for(gen_ui_extent_t i = 0; i < (extent.z - dest_scale) / dest_scale; ++i) {
		error = draw_handler(ninepatch, (gen_ui_rect_t){src_scale, 0, src_scale, src_scale}, (gen_ui_rect_t){extent.x + (i * dest_scale) + dest_scale, extent.y, dest_scale, dest_scale}, passthrough);
		GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");
	}
	error = draw_handler(ninepatch, (gen_ui_rect_t){src_scale, 0, (gen_ui_extent_t) ((double) ((extent.z - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale)), src_scale}, (gen_ui_rect_t){extent.x + (((extent.z - dest_scale) / dest_scale) * dest_scale) + dest_scale, extent.y, (extent.z - dest_scale) % dest_scale, dest_scale}, passthrough);
	GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");

	// Bottom
	for(gen_ui_extent_t i = 0; i < (extent.z - dest_scale) / dest_scale; ++i) {
		error = draw_handler(ninepatch, (gen_ui_rect_t){src_scale, 2 * src_scale, src_scale, src_scale}, (gen_ui_rect_t){extent.x + (i * dest_scale) + dest_scale, extent.y + (extent.w - dest_scale) + dest_scale, dest_scale, dest_scale}, passthrough);
		GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");
	}
	error = draw_handler(ninepatch, (gen_ui_rect_t){src_scale, 2 * src_scale, (gen_ui_extent_t) ((double) ((extent.z - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale)), src_scale}, (gen_ui_rect_t){extent.x + (((extent.z - dest_scale) / dest_scale) * dest_scale) + dest_scale, extent.y + (extent.w - dest_scale) + dest_scale, (extent.z - dest_scale) % dest_scale, dest_scale}, passthrough);
	GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");

	// Left
	for(gen_ui_extent_t i = 0; i < (extent.w - dest_scale) / dest_scale; ++i) {
		error = draw_handler(ninepatch, (gen_ui_rect_t){0, src_scale, src_scale, src_scale}, (gen_ui_rect_t){extent.x, extent.y + (i * dest_scale) + dest_scale, dest_scale, dest_scale}, passthrough);
		GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");
	}
	error = draw_handler(ninepatch, (gen_ui_rect_t){0, src_scale, src_scale, (gen_ui_extent_t) ((double) ((extent.w - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale))}, (gen_ui_rect_t){extent.x, extent.y + (((extent.w - dest_scale) / dest_scale) * dest_scale) + dest_scale, dest_scale, (extent.w - dest_scale) % dest_scale}, passthrough);
	GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");

	// Right
	for(gen_ui_extent_t i = 0; i < (extent.w - dest_scale) / dest_scale; ++i) {
		error = draw_handler(ninepatch, (gen_ui_rect_t){2 * src_scale, src_scale, src_scale, src_scale}, (gen_ui_rect_t){extent.x + (extent.z - dest_scale) + dest_scale, extent.y + (i * dest_scale) + dest_scale, dest_scale, dest_scale}, passthrough);
		GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");
	}
	error = draw_handler(ninepatch, (gen_ui_rect_t){2 * src_scale, src_scale, src_scale, (gen_ui_extent_t) ((double) ((extent.w - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale))}, (gen_ui_rect_t){extent.x + (extent.z - dest_scale) + dest_scale, extent.y + (((extent.w - dest_scale) / dest_scale) * dest_scale) + dest_scale, dest_scale, (extent.w - dest_scale) % dest_scale}, passthrough);
	GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");

	// Centre
	for(gen_ui_extent_t i = 0; i < (extent.z - dest_scale) / dest_scale; ++i) {
		for(gen_ui_extent_t j = 0; j < (extent.w - dest_scale) / dest_scale; ++j) {
			error = draw_handler(ninepatch, (gen_ui_rect_t){src_scale, src_scale, src_scale, src_scale}, (gen_ui_rect_t){extent.x + (i * dest_scale) + dest_scale, extent.y + (j * dest_scale) + dest_scale, dest_scale, dest_scale}, passthrough);
			GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");
			error = draw_handler(ninepatch, (gen_ui_rect_t){src_scale, src_scale, (gen_ui_extent_t) ((double) ((extent.z - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale)), src_scale}, (gen_ui_rect_t){extent.x + (extent.z / dest_scale) * dest_scale, extent.y + (j * dest_scale) + dest_scale, (extent.z - dest_scale) % dest_scale, dest_scale}, passthrough);
			GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");
		}
		error = draw_handler(ninepatch, (gen_ui_rect_t){src_scale, src_scale, src_scale, (gen_ui_extent_t) ((double) ((extent.w - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale))}, (gen_ui_rect_t){extent.x + (i * dest_scale) + dest_scale, extent.y + (extent.w / dest_scale) * dest_scale, dest_scale, (extent.w - dest_scale) % dest_scale}, passthrough);
		GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");
	}
	error = draw_handler(ninepatch, (gen_ui_rect_t){src_scale, src_scale, (gen_ui_extent_t) ((double) ((extent.z - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale)), (gen_ui_extent_t) ((double) ((extent.w - dest_scale) % dest_scale) * ((double) src_scale / (double) dest_scale))}, (gen_ui_rect_t){extent.x + (extent.z / dest_scale) * dest_scale, extent.y + (extent.w / dest_scale) * dest_scale, (extent.z - dest_scale) % dest_scale, (extent.w - dest_scale) % dest_scale}, passthrough);
	GEN_ERROR_OUT_IF(error, "Call to ninepatch draw handler failed");

	GEN_ALL_OK;
}
