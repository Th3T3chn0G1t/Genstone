// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include <gencommon.h>
#include <genuine.h>

#define SOURCE_SCALE (uint16_t) 4
#define OUTPUT_SCALE (uint16_t) 8

#define ELEMENT_SCALE (uint16_t) 5

static void render_handler(const gen_ui_ninepatch_t* const restrict ninepatch, __unused const gen_ui_rect_t source, __unused const gen_ui_rect_t destination, __unused void* const restrict passthrough) {
	GEN_REQUIRE_EQUAL(NULL, ninepatch->texture);
	GEN_REQUIRE_EQUAL(SOURCE_SCALE, ninepatch->source_scale);
}

int main() {
	gen_ui_ninepatch_t ninepatch = {NULL, SOURCE_SCALE};

	gen_ui_element_t element = {&ninepatch, {.xywh = {0, 0, ELEMENT_SCALE, ELEMENT_SCALE}}};

	gen_ui_draw_element(render_handler, &element, OUTPUT_SCALE, NULL);
}
