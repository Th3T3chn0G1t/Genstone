#include "include/genuine.h"
#include <gentooling.h>

const static gen_ui_rect_t ninepatch_source_rects[9] =
{
    { .xywh = { 0, 0, 1, 1 } },
    { .xywh = { 1, 0, 1, 1 } },
    { .xywh = { 2, 0, 1, 1 } },
    { .xywh = { 0, 1, 1, 1 } },
    { .xywh = { 1, 1, 1, 1 } },
    { .xywh = { 2, 1, 1, 1 } },
    { .xywh = { 0, 2, 1, 1 } },
    { .xywh = { 1, 2, 1, 1 } },
    { .xywh = { 2, 2, 1, 1 } }
};

#define GEN_INTERNAL_UI_ELEMENT_RECTS(index, w_mul, h_mul, x_mod, y_mod, w_mod, h_mod, x_w_mod_mul, y_h_mod_mul) \
    do { \
        source.x = ninepatch_source_rects[index].x * element->ninepatch->source_scale; \
        source.y = ninepatch_source_rects[index].y * element->ninepatch->source_scale; \
        source.w = ninepatch_source_rects[index].w * element->ninepatch->source_scale; \
        source.h = ninepatch_source_rects[index].h * element->ninepatch->source_scale; \
        destination.x = destination_scale * (element->rect.x + (x_w_mod_mul * (element->rect.w - (uint16_t) 1)) + x_mod); \
        destination.y = destination_scale * (element->rect.y + (y_h_mod_mul * (element->rect.h - (uint16_t) 1)) + y_mod); \
        destination.w = destination_scale * ((w_mul * element->rect.w) + (w_mod ? (uint16_t) 1 : (uint16_t) -2)); \
        destination.h = destination_scale * ((h_mul * element->rect.h) + (h_mod ? (uint16_t) 1 : (uint16_t) -2)); \
        handler(element->ninepatch, source, destination, passthrough); \
    } while(0)

void gen_ui_draw_element(const gen_ui_render_handler_t handler, const gen_ui_element_t* const restrict element, const uint16_t destination_scale, void* restrict passthrough) {
    GEN_FRAME_BEGIN;

    gen_ui_rect_t source;
    gen_ui_rect_t destination;

    GEN_INTERNAL_UI_ELEMENT_RECTS(GEN_UI_SOURCE_RECT_TL, (uint16_t) 0, (uint16_t) 0, (uint16_t) 0, (uint16_t) 0, (uint16_t) 1, (uint16_t) 1, (uint16_t) 0, (uint16_t) 0);
    GEN_INTERNAL_UI_ELEMENT_RECTS(GEN_UI_SOURCE_RECT_T , (uint16_t) 1, (uint16_t) 0, (uint16_t) 1, (uint16_t) 0, (uint16_t) 0, (uint16_t) 1, (uint16_t) 0, (uint16_t) 0);
    GEN_INTERNAL_UI_ELEMENT_RECTS(GEN_UI_SOURCE_RECT_TR, (uint16_t) 0, (uint16_t) 0, (uint16_t) 0, (uint16_t) 0, (uint16_t) 1, (uint16_t) 1, (uint16_t) 1, (uint16_t) 0);
    GEN_INTERNAL_UI_ELEMENT_RECTS(GEN_UI_SOURCE_RECT_L , (uint16_t) 0, (uint16_t) 1, (uint16_t) 0, (uint16_t) 1, (uint16_t) 1, (uint16_t) 0, (uint16_t) 0, (uint16_t) 0);
    GEN_INTERNAL_UI_ELEMENT_RECTS(GEN_UI_SOURCE_RECT_C , (uint16_t) 1, (uint16_t) 1, (uint16_t) 1, (uint16_t) 1, (uint16_t) 0, (uint16_t) 0, (uint16_t) 0, (uint16_t) 0);
    GEN_INTERNAL_UI_ELEMENT_RECTS(GEN_UI_SOURCE_RECT_R , (uint16_t) 0, (uint16_t) 1, (uint16_t) 0, (uint16_t) 1, (uint16_t) 1, (uint16_t) 0, (uint16_t) 1, (uint16_t) 0);
    GEN_INTERNAL_UI_ELEMENT_RECTS(GEN_UI_SOURCE_RECT_BL, (uint16_t) 0, (uint16_t) 0, (uint16_t) 0, (uint16_t) 0, (uint16_t) 1, (uint16_t) 1, (uint16_t) 0, (uint16_t) 1);
    GEN_INTERNAL_UI_ELEMENT_RECTS(GEN_UI_SOURCE_RECT_B , (uint16_t) 1, (uint16_t) 0, (uint16_t) 1, (uint16_t) 0, (uint16_t) 0, (uint16_t) 1, (uint16_t) 0, (uint16_t) 1);
    GEN_INTERNAL_UI_ELEMENT_RECTS(GEN_UI_SOURCE_RECT_BR, (uint16_t) 0, (uint16_t) 0, (uint16_t) 0, (uint16_t) 0, (uint16_t) 1, (uint16_t) 1, (uint16_t) 1, (uint16_t) 1);

    GEN_FRAME_END;
}
