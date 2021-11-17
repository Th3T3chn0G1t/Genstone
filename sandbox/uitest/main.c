#include <gencommon.h>

typedef ssize_t gen_ui_extent_t;

typedef struct {
	gen_ui_extent_t x;
	gen_ui_extent_t y;
	gen_ui_extent_t w;
	gen_ui_extent_t h;
} const gen_ui_rect_t;

typedef void (*gen_ui_draw_handler_t)(void* const restrict, const gen_ui_rect_t, const gen_ui_rect_t, void* const restrict);

static GEN_ERRORABLE_RETURN gen_ui_draw_ninepatch_direct(void* const restrict ninepatch, const gen_ui_draw_handler_t draw_handler, const gen_ui_rect_t extent, const gen_ui_extent_t src_scale, const gen_ui_extent_t dest_scale, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_ui_draw_ninepatch_direct);

	if(!draw_handler) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`draw_handler` was NULL");
	if(!extent.w || !extent.h || !src_scale || !dest_scale) GEN_ERROR_OUT(GEN_OK, ""); // No work can be done but its not technically an error

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

	GEN_ERROR_OUT(GEN_OK, "");
}

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

#define MILLIS_PER_SECOND 1000
#define FPS 60

static void rect_callback(void* const restrict ninepatch, const gen_ui_rect_t src, const gen_ui_rect_t dest, void* const restrict passthrough) {
	SDL_Rect src_r = {(int) src.x, (int) src.y, (int) src.w, (int) src.h};
	SDL_Rect dest_r = {(int) dest.x, (int) dest.y, (int) dest.w, (int) dest.h};
	SDL_BlitScaled((SDL_Surface*) ninepatch, &src_r, (SDL_Surface*) passthrough, &dest_r);
}

int main(void) {
	GEN_FRAME_BEGIN(main);

	glog(INFO, "Hello, Genuine!");

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);

	SDL_Window* window = SDL_CreateWindow("Genuine Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Surface* screen_surface = SDL_GetWindowSurface(window);

	SDL_Surface* texture = IMG_Load("sandbox/uitest/ninepatch.png");

	while(true) {
		SDL_Event e;
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) goto cleanup;
		}

		SDL_Rect screen_dimensions = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		SDL_FillRect(screen_surface, &screen_dimensions, 0xFF000000);

		(void) gen_ui_draw_ninepatch_direct(texture, rect_callback, (gen_ui_rect_t){0, 0, 256, SCREEN_HEIGHT - 32}, 16, 32, screen_surface);

		SDL_UpdateWindowSurface(window);
		SDL_Delay(MILLIS_PER_SECOND / FPS);
	}
cleanup:

	SDL_FreeSurface(texture);
	SDL_DestroyWindow(window);

	SDL_Quit();
}
