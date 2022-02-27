#include <gencommon.h>
#include <genfs.h>
#include <genuine.h>

GEN_DIAG_REGION_BEGIN
GEN_DIAG_IGNORE_ALL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
GEN_DIAG_REGION_END

#define MILLIS_PER_SECOND 1000
#define FPS 60

#define NINEPATCH_SCALE 16

static SDL_Renderer* renderer = NULL;

static void rect_callback(void* const restrict ninepatch, const gen_ui_rect_t src, const gen_ui_rect_t dest, __unused void* const restrict passthrough) {
	SDL_Rect src_r = {(int) src.x, (int) src.y, (int) src.w, (int) src.h};
	SDL_Rect dest_r = {(int) dest.x, (int) dest.y, (int) dest.w, (int) dest.h};

	SDL_RenderCopy(renderer, (SDL_Texture*) ninepatch, &src_r, &dest_r);
}

static const SDL_Color color = {255, 176, 223, 255};
static gen_ui_extent_t dir_list_height = 0;
static gen_ui_extent_t ui_scale = 16;

static void dir_list_callback(const char* const restrict path, void* const restrict passthrough) {
	SDL_Surface* text_r = TTF_RenderText_Blended((TTF_Font*) passthrough, path, color);
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, text_r);
	SDL_Rect dest = {(int) ui_scale, (int) dir_list_height++ * (int) (ui_scale + ui_scale / 2) + 4 * (int) ui_scale, text_r->w, text_r->h};
	SDL_FreeSurface(text_r);
	SDL_RenderCopy(renderer, text, NULL, &dest);
	SDL_DestroyTexture(text);
}

int main(void) {
	GEN_FRAME_BEGIN(main);

	glog(INFO, "Hello, Genuine!");

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
	TTF_Init();

	gen_ui_extent_t screen_width = 1280;
	gen_ui_extent_t screen_height = 780;

	SDL_Window* window = SDL_CreateWindow("Genuine Test", 0, 0, (int) screen_width, (int) screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface* texture_r = IMG_Load("sandbox/uitest/ninepatch.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, texture_r);
	SDL_FreeSurface(texture_r);
	SDL_Surface* viewport_texture_r = IMG_Load("sandbox/uitest/ninepatch_viewport.png");
	SDL_Texture* viewport_texture = SDL_CreateTextureFromSurface(renderer, viewport_texture_r);
	SDL_FreeSurface(viewport_texture_r);

	gen_filesystem_handle_t dir;
	(void) gen_handle_open(&dir, ".");

	while(true) {
		SDL_Event e;
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) goto cleanup;
		}

		int screen_width_r = 0;
		int screen_height_r = 0;
		SDL_GetWindowSize(window, &screen_width_r, &screen_height_r);
		SDL_Rect viewport = {0, 0, screen_width_r, screen_height_r};
		SDL_RenderSetViewport(renderer, &viewport);
		ui_scale = (gen_ui_extent_t) screen_width_r / 80;
		screen_width = (gen_ui_extent_t) screen_width_r;
		screen_height = (gen_ui_extent_t) screen_height_r;

		TTF_Font* font = TTF_OpenFont("sandbox/uitest/8-bit-operator/8bitOperatorPlusSC-Bold.ttf", (int) ui_scale);
		TTF_Font* font_x2 = TTF_OpenFont("sandbox/uitest/8-bit-operator/8bitOperatorPlusSC-Bold.ttf", 2 * (int) ui_scale);

		SDL_RenderClear(renderer);

		static const char* const labels[] = {"File", "Edit", "View", "Run", "Help"};

		gen_ui_extent_t TOOLBAR_BUTTONS_LENGTH = sizeof(labels) / sizeof(labels[0]);
		gen_ui_extent_t TOOLBAR_HEIGHT = ui_scale + ui_scale / 2;
		gen_ui_extent_t TOOLBAR_BUTTON_WIDTH = 4 * ui_scale;
		gen_ui_extent_t TOOLBAR_BUTTON_STRIDE = 5 * ui_scale;

		gen_ui_extent_t SIDEBAR_Y = TOOLBAR_HEIGHT + ui_scale;
		gen_ui_extent_t SIDEBAR_WIDTH = 16 * ui_scale;

		gen_ui_extent_t EXPLORER_Y = (screen_height - ui_scale) - (8 * ui_scale);
		gen_ui_extent_t EXPLORER_HEIGHT = (screen_height - ui_scale) - EXPLORER_Y;

		gen_ui_extent_t HIERARCHY_WIDTH = (12 * ui_scale);
		gen_ui_extent_t HIERARCHY_X = (screen_width - ui_scale) - HIERARCHY_WIDTH;

		gen_ui_extent_t VIEWPORT_X = SIDEBAR_WIDTH + ui_scale;

		for(gen_ui_extent_t i = 0; i < TOOLBAR_BUTTONS_LENGTH; ++i) {
			(void) gen_ui_draw_ninepatch_direct(texture, rect_callback, (gen_ui_rect_t){i * TOOLBAR_BUTTON_STRIDE, 0, TOOLBAR_BUTTON_WIDTH, TOOLBAR_HEIGHT}, NINEPATCH_SCALE, ui_scale, NULL);

			SDL_Surface* text_r = TTF_RenderText_Blended(font, labels[i], color);
			SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, text_r);
			SDL_Rect dest = {(int) (i * TOOLBAR_BUTTON_STRIDE) + ((int) TOOLBAR_BUTTON_WIDTH / 2) - (text_r->w / 2) + (int) ui_scale / 2, (int) (TOOLBAR_HEIGHT / 2) - (text_r->h / 2) + (int) (ui_scale / 2), text_r->w, text_r->h};
			SDL_FreeSurface(text_r);
			SDL_RenderCopy(renderer, text, NULL, &dest);
			SDL_DestroyTexture(text);
		}

		(void) gen_ui_draw_ninepatch_direct(texture, rect_callback, (gen_ui_rect_t){(TOOLBAR_BUTTON_STRIDE * TOOLBAR_BUTTONS_LENGTH), 0, (screen_width - ui_scale) - (TOOLBAR_BUTTON_STRIDE * TOOLBAR_BUTTONS_LENGTH), TOOLBAR_HEIGHT}, NINEPATCH_SCALE, ui_scale, NULL);
		(void) gen_ui_draw_ninepatch_direct(texture, rect_callback, (gen_ui_rect_t){0, SIDEBAR_Y, SIDEBAR_WIDTH, (screen_height - ui_scale) - SIDEBAR_Y}, NINEPATCH_SCALE, ui_scale, NULL);
		(void) gen_ui_draw_ninepatch_direct(texture, rect_callback, (gen_ui_rect_t){SIDEBAR_WIDTH + ui_scale, EXPLORER_Y, (screen_width - ui_scale) - (SIDEBAR_WIDTH + ui_scale), EXPLORER_HEIGHT}, NINEPATCH_SCALE, ui_scale, NULL);
		(void) gen_ui_draw_ninepatch_direct(texture, rect_callback, (gen_ui_rect_t){HIERARCHY_X, SIDEBAR_Y, HIERARCHY_WIDTH, (screen_height - ui_scale) - (SIDEBAR_Y + EXPLORER_HEIGHT + ui_scale)}, NINEPATCH_SCALE, ui_scale, NULL);
		(void) gen_ui_draw_ninepatch_direct(viewport_texture, rect_callback, (gen_ui_rect_t){VIEWPORT_X, SIDEBAR_Y, (screen_width - ui_scale) - (VIEWPORT_X + HIERARCHY_WIDTH + ui_scale), (screen_height - ui_scale) - (SIDEBAR_Y + EXPLORER_HEIGHT + ui_scale)}, NINEPATCH_SCALE, ui_scale, NULL);

		dir_list_height = 0;
		(void) gen_directory_list(&dir, dir_list_callback, font);

		SDL_RenderPresent(renderer);
		TTF_CloseFont(font);
		TTF_CloseFont(font_x2);
		SDL_Delay(MILLIS_PER_SECOND / FPS);
	}
cleanup:
	(void) gen_handle_close(&dir);

	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(viewport_texture);
	SDL_DestroyWindow(window);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
