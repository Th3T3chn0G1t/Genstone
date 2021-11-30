// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <genuine.h>

static void my_handler(void* const restrict texture, const gen_ui_rect_t src, const gen_ui_rect_t dest, void* const restrict passthrough) {
    my_draw_function(passthrough /* your renderer */, texture, src, dest);
}

int main(void) {
    // Set up your renderer etc.

    gen_error_t error = gen_ui_draw_ninepatch_direct(my_texture, my_handler, (gen_ui_rect_t) {20,40,600,400}, 16, 32, my_renderer);
    GEN_REQUIRE_NO_ERROR(error);
}
