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
#include <SDL2/SDL_ttf.h>
#include <genfs.h>

#define SCREEN_WIDTH 1900
#define SCREEN_HEIGHT 1000

#define MILLIS_PER_SECOND 1000
#define FPS 60

#define NINEPATCH_SCALE 16
#define UI_SCALE 16

static SDL_Surface* screen_surface;

static void rect_callback(void* const restrict ninepatch, const gen_ui_rect_t src, const gen_ui_rect_t dest, __unused void* const restrict passthrough) {
	SDL_Rect src_r = {(int) src.x, (int) src.y, (int) src.w, (int) src.h};
	SDL_Rect dest_r = {(int) dest.x, (int) dest.y, (int) dest.w, (int) dest.h};
	SDL_BlitScaled((SDL_Surface*) ninepatch, &src_r, screen_surface, &dest_r);
}

static const SDL_Color color = {255, 176, 223, 255};
static gen_ui_extent_t dir_list_height = 0;

static void dir_list_callback(const char* const restrict path, void* const restrict passthrough) {
	SDL_Surface* text = TTF_RenderText_Blended((TTF_Font*) passthrough, path, color);
	SDL_Rect dest = {UI_SCALE, (int) dir_list_height++ * 2 * UI_SCALE + 4 * UI_SCALE, 0, 0};
	SDL_BlitSurface(text, NULL, screen_surface, &dest);
}

int main(void) {
	GEN_FRAME_BEGIN(main);

	glog(INFO, "Hello, Genuine!");

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
	TTF_Init();

	SDL_Window* window = SDL_CreateWindow("Genuine Test", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	screen_surface = SDL_GetWindowSurface(window);

	SDL_Surface* texture = IMG_Load("sandbox/uitest/ninepatch.png");
	SDL_Surface* viewport_texture = IMG_Load("sandbox/uitest/ninepatch_viewport.png");
	TTF_Font* font_32 = TTF_OpenFont("sandbox/uitest/8-bit-operator/8bitOperatorPlusSC-Bold.ttf", 2 * UI_SCALE);
	TTF_Font* font_16 = TTF_OpenFont("sandbox/uitest/8-bit-operator/8bitOperatorPlusSC-Bold.ttf", UI_SCALE);

	SDL_Surface* text = NULL;

	while(true) {
		SDL_Event e;
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) goto cleanup;
		}

		const SDL_Rect screen_dimensions = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		SDL_FillRect(screen_surface, &screen_dimensions, 0xFF000000);

		const char* labels[] = {"File", "Edit", "View", "Run", "Help"};

		for(register gen_ui_extent_t i = 0; i < (gen_ui_extent_t) (sizeof(labels) / sizeof(labels[0])); ++i) {
			(void) gen_ui_draw_ninepatch_direct(texture, rect_callback, (gen_ui_rect_t){5 * i * UI_SCALE, 0, 4 * UI_SCALE, UI_SCALE + UI_SCALE / 2}, NINEPATCH_SCALE, UI_SCALE, screen_surface);

			text = TTF_RenderText_Blended(font_16, labels[i], color);
			SDL_Rect dest = {(int) (UI_SCALE + 5 * i * UI_SCALE), UI_SCALE / 2, 0, 0};
			SDL_BlitSurface(text, NULL, screen_surface, &dest);
		}
		(void) gen_ui_draw_ninepatch_direct(texture, rect_callback, (gen_ui_rect_t){(5 * UI_SCALE * (sizeof(labels) / sizeof(labels[0]))), 0, (SCREEN_WIDTH - UI_SCALE) - (5 * UI_SCALE * (sizeof(labels) / sizeof(labels[0]))), UI_SCALE + UI_SCALE / 2}, NINEPATCH_SCALE, UI_SCALE, screen_surface);

		(void) gen_ui_draw_ninepatch_direct(texture, rect_callback, (gen_ui_rect_t){0, 2 * UI_SCALE + UI_SCALE / 2, 16 * UI_SCALE, (SCREEN_HEIGHT - UI_SCALE) - (2 * UI_SCALE + UI_SCALE / 2)}, NINEPATCH_SCALE, UI_SCALE, screen_surface);
		(void) gen_ui_draw_ninepatch_direct(texture, rect_callback, (gen_ui_rect_t){16 * UI_SCALE + UI_SCALE, 44 * UI_SCALE + UI_SCALE, (SCREEN_WIDTH - UI_SCALE) - (16 * UI_SCALE + UI_SCALE), (SCREEN_HEIGHT - UI_SCALE) - (44 * UI_SCALE + UI_SCALE)}, NINEPATCH_SCALE, UI_SCALE, screen_surface);
		(void) gen_ui_draw_ninepatch_direct(texture, rect_callback, (gen_ui_rect_t){(SCREEN_WIDTH - UI_SCALE) - (16 * UI_SCALE), 2 * UI_SCALE + UI_SCALE / 2, 16 * UI_SCALE, (SCREEN_HEIGHT - UI_SCALE) - (2 * UI_SCALE + UI_SCALE / 2) - ((SCREEN_HEIGHT - UI_SCALE) - (44 * UI_SCALE + UI_SCALE) + UI_SCALE)}, NINEPATCH_SCALE, UI_SCALE, screen_surface);

		(void) gen_ui_draw_ninepatch_direct(viewport_texture, rect_callback, (gen_ui_rect_t){16 * UI_SCALE + UI_SCALE, 2 * UI_SCALE + UI_SCALE / 2, (SCREEN_WIDTH - UI_SCALE) - 2 * (16 * UI_SCALE + UI_SCALE), (SCREEN_HEIGHT - UI_SCALE) - (2 * UI_SCALE + UI_SCALE / 2) - ((SCREEN_HEIGHT - UI_SCALE) - (44 * UI_SCALE + UI_SCALE)) - UI_SCALE}, NINEPATCH_SCALE, UI_SCALE, screen_surface);

		dir_list_height = 0;
		gen_filesystem_handle_t dir;
		(void) gzalloc((void**) &dir.path, GEN_PATH_MAX, sizeof(char));
		(void) gen_handle_open(&dir, ".");
		(void) gen_directory_list(&dir, dir_list_callback, font_16);
		(void) gen_handle_close(&dir);

		SDL_UpdateWindowSurface(window);
		SDL_Delay(MILLIS_PER_SECOND / FPS);
	}
cleanup:
	SDL_FreeSurface(text);
	TTF_CloseFont(font_32);
	TTF_CloseFont(font_16);
	SDL_FreeSurface(texture);
	SDL_DestroyWindow(window);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
