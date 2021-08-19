#include <gencommon.h>
#include <genuine.h>

#define SOURCE_SCALE (uint16_t) 4
#define OUTPUT_SCALE (uint16_t) 8

#define ELEMENT_SCALE (uint16_t) 5

static void render_handler(const gen_ui_ninepatch_t* const restrict ninepatch, const gen_ui_rect_t source, const gen_ui_rect_t destination, void* restrict passthrough) {
    (void) source;
    (void) destination;
    (void) passthrough;

    GEN_REQUIRE_EQUAL(NULL, ninepatch->texture);
    GEN_REQUIRE_EQUAL(SOURCE_SCALE, ninepatch->source_scale);
}

int main() {
    gen_ui_ninepatch_t ninepatch = { NULL, SOURCE_SCALE };

    gen_ui_element_t element = { &ninepatch, { .xywh = { 0, 0, ELEMENT_SCALE, ELEMENT_SCALE } } };

    gen_ui_draw_element(render_handler, &element, OUTPUT_SCALE, NULL);
}
