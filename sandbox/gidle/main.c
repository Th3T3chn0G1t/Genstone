#include <gencommon.h>
#include <genfs.h>

GEN_DIAG_REGION_BEGIN
GEN_DIAG_IGNORE_ALL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
GEN_DIAG_REGION_END

#define MILLIS_PER_SECOND 1000
#define FPS 60

static const int screen_width = 640;
static const int screen_height = 480;

static uint8_t r = 0, g = 0, b = 0;

static SDL_Window* window;
static SDL_Renderer* renderer;

static TTF_Font* font;

static void render_textv(const int16_t x, const int16_t y, const char* text, va_list lista) {
	GEN_FRAME_BEGIN(render_textv);

	GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wformat-nonliteral"
	va_list listb;
	va_copy(listb, lista);
	char* buffer = NULL;
	gen_error_t error = gzalloc((void**) &buffer, (size_t) (vsnprintf(NULL, 0, text, lista) + 1), sizeof(char));
	GEN_REQUIRE_NO_ERROR(error);
	vsprintf(buffer, text, listb);
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, buffer, (SDL_Color){r, g, b, 255});
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_RenderCopy(renderer, text_texture, NULL, &(SDL_Rect){x, y, text_surface->w, text_surface->h});
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	error = gfree(buffer);
	GEN_REQUIRE_NO_ERROR(error);
	GEN_DIAG_REGION_END
}

static void render_text(const int16_t x, const int16_t y, const char* text, ...) {
	GEN_FRAME_BEGIN(render_text);

	va_list list;
	va_start(list, text);
	render_textv(x, y, text, list);
}

void update_game(void);
uint32_t update_per_second(uint32_t interval, void* param);

static noreturn void sighandler(__unused int signo) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(0);
}

static bool mouse_down = false;
static bool mouse_clicked = false;

static int16_t ui_scale;

int main(void) {
	GEN_FRAME_BEGIN(main);

	signal(SIGINT, sighandler);

	glog(INFO, "Hello, Gidle!");

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
	TTF_Init();

	window = SDL_CreateWindow("Gidle", 0, 0, screen_width, screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	font = TTF_OpenFont("sandbox/uitest/8-bit-operator/8bitOperatorPlusSC-Bold.ttf", 24);

	SDL_Surface* text_surface = TTF_RenderText_Blended(font, "a", (SDL_Color){r, g, b, 255});
	ui_scale = (int16_t) text_surface->h;
	SDL_FreeSurface(text_surface);

	SDL_AddTimer(MILLIS_PER_SECOND, update_per_second, NULL);

	while(true) {
		SDL_Event e;
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT) {
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				TTF_Quit();
				IMG_Quit();
				SDL_Quit();
				exit(0);
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		if(mouse_clicked) mouse_clicked = false;
		uint32_t buttons = SDL_GetMouseState(NULL, NULL);
		bool ml = buttons & SDL_BUTTON_LMASK;
		if(ml && !mouse_down) mouse_clicked = true;
		mouse_down = ml;
		update_game();

		SDL_RenderPresent(renderer);
		SDL_Delay(MILLIS_PER_SECOND / FPS);
	}
}

__unused static bool do_button(const int16_t x, const int16_t y, const char* label, ...) {
	GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wformat-nonliteral"
	va_list lista;
	va_start(lista, label);
	va_list listb;
	va_copy(listb, lista);
	char* buffer = NULL;
	gen_error_t error = gzalloc((void**) &buffer, (size_t) (vsnprintf(NULL, 0, label, lista) + 1), sizeof(char));
	GEN_REQUIRE_NO_ERROR(error);
	vsprintf(buffer, label, listb);
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, buffer, (SDL_Color){r, g, b, 255});
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_RenderCopy(renderer, text_texture, NULL, &(SDL_Rect){(x / 2), y, text_surface->w, text_surface->h});
	SDL_RenderDrawRect(renderer, &(SDL_Rect){x, y, text_surface->w, text_surface->h});
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	SDL_bool in = SDL_PointInRect(&(SDL_Point){mx, my}, &(SDL_Rect){x, y, text_surface->w, text_surface->h});
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	error = gfree(buffer);
	GEN_REQUIRE_NO_ERROR(error);
	GEN_DIAG_REGION_END
	return in && mouse_clicked;
}

static bool do_button_ow(uint16_t* const ow, const int16_t x, const int16_t y, const char* label, ...) {
	GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wformat-nonliteral"
	va_list lista;
	va_start(lista, label);
	va_list listb;
	va_copy(listb, lista);
	char* buffer = NULL;
	gen_error_t error = gzalloc((void**) &buffer, (size_t) (vsnprintf(NULL, 0, label, lista) + 1), sizeof(char));
	GEN_REQUIRE_NO_ERROR(error);
	vsprintf(buffer, label, listb);
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, buffer, (SDL_Color){r, g, b, 255});
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_RenderCopy(renderer, text_texture, NULL, &(SDL_Rect){(x / 2), y, text_surface->w, text_surface->h});
	SDL_RenderDrawRect(renderer, &(SDL_Rect){x, y, text_surface->w, text_surface->h});
	int mx, my;
	*ow = (uint16_t) text_surface->w;
	SDL_GetMouseState(&mx, &my);
	SDL_bool in = SDL_PointInRect(&(SDL_Point){mx, my}, &(SDL_Rect){x, y, text_surface->w, text_surface->h});
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	error = gfree(buffer);
	GEN_REQUIRE_NO_ERROR(error);
	GEN_DIAG_REGION_END
	return in && mouse_clicked;
}

static bool do_button_w(const int16_t x, const int16_t y, const uint16_t w, const char* label, ...) {
	GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wformat-nonliteral"
	va_list lista;
	va_start(lista, label);
	va_list listb;
	va_copy(listb, lista);
	char* buffer = NULL;
	gen_error_t error = gzalloc((void**) &buffer, (size_t) (vsnprintf(NULL, 0, label, lista) + 1), sizeof(char));
	GEN_REQUIRE_NO_ERROR(error);
	vsprintf(buffer, label, listb);
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, buffer, (SDL_Color){r, g, b, 255});
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_RenderCopy(renderer, text_texture, NULL, &(SDL_Rect){x + (w / 2) - (text_surface->w / 2), y, text_surface->w, text_surface->h});
	SDL_RenderDrawRect(renderer, &(SDL_Rect){x, y, w, text_surface->h});
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	SDL_bool in = SDL_PointInRect(&(SDL_Point){mx, my}, &(SDL_Rect){x, y, w, text_surface->h});
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
	error = gfree(buffer);
	GEN_REQUIRE_NO_ERROR(error);
	GEN_DIAG_REGION_END
	return in && mouse_clicked;
}

#define white 255, 255, 255
#define red 255, 0, 0
static void set_color(uint8_t ra, uint8_t ga, uint8_t ba) {
	r = ra;
	g = ga;
	b = ba;
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

static const size_t starting_points = 50;
static size_t points = starting_points;
static size_t points_per_second = 0;

static size_t n_clicks = 0;
static size_t clicks_this_second = 0;
static size_t points_per_click = 1;
static double machine_clicks_per_second = 0;
static double machines_per_second = 0;
static double machines_running = 0;

static size_t machine_pps = 1;
static size_t machine_cost = 10;
static size_t n_machines = 0;

static void click_upgrade_a(void) {
	points_per_click++;
}
static void machine_upgrade_a(void) {
	machine_pps++;
}
static void machine_upgrade_b(void) {
	machine_cost /= 2;
}
static void machine_upgrade_c(void) {
	machines_per_second += 0.1;
}
static void machine_upgrade_d(void) {
	n_machines += 10;
}
static void machine_click_upgrade_a(void) {
	machine_clicks_per_second += 0.1;
}
static void machine_click_upgrade_b(void) {
	machine_pps *= 2;
	points_per_click *= 2;
}
static struct {
	enum {
		clicks,
		machine,
		clicks_and_machine
	} target;
	size_t req;
	size_t req2;
	size_t cost;
	void (*cb)(void);
	const char* name;
} upgrades[] = {
	{clicks, 100, 0, 75, click_upgrade_a, "Increase Click Efficiency"},
	{machine, 5, 0, 25, machine_upgrade_a, "Increase Machine Efficiency"},
	{machine, 10, 0, 100, machine_upgrade_b, "Reduce Machine Price"},
	{clicks_and_machine, 150, 10, 250, machine_click_upgrade_a, "Machine-Click Integration"},
	{machine, 25, 0, 2000, machine_upgrade_c, "Automatic Machine Purchase"},
	{clicks_and_machine, 500, 30, 10000, machine_click_upgrade_b, "Efficiency Revamp"},
	{machine, 50, 0, 250000, machine_upgrade_d, "Buy Super Machine"}};

uint32_t update_per_second(uint32_t interval, __unused void* param) {
	machines_running += machines_per_second;

	n_clicks += (size_t) (machine_clicks_per_second * (double) n_machines);
	points += (size_t) (machine_clicks_per_second * (double) points_per_click * (double) n_machines);

	points += points_per_second;

	clicks_this_second = 0;

	return interval;
}

void update_game(void) {
	GEN_FRAME_BEGIN(update_game);

	set_color(white);
	render_text(0, 0, "%zu$ @ %zu$/s + %zuc/s @ %zu$/c", points, points_per_second, (size_t) (machine_clicks_per_second * (double) n_machines) + clicks_this_second, points_per_click);
	if(do_button_w(screen_width - ui_scale, 0, (uint16_t) ui_scale, "+")) {
		n_clicks++;
		clicks_this_second++;
		points += points_per_click;
	}

	while(points >= machine_cost && machines_running >= 1) {
		points -= machine_cost;
		machine_cost += machine_cost / 4;
		n_machines++;
		machines_running -= 1;
	}
	if(points < machine_cost) set_color(red);
	uint16_t machine_bw;
	if(do_button_ow(&machine_bw, 0, ui_scale, "Buy Machine %zu$", machine_cost) && points >= machine_cost) {
		points -= machine_cost;
		machine_cost += machine_cost / 4;
		n_machines++;
	}
	set_color(white);
	SDL_RenderDrawLine(renderer, 0, ui_scale + 1, (int) ((machines_running > 1 ? 1 : machines_running) * (double) machine_bw), ui_scale + 1);

	set_color(white);
	for(size_t i = 0; i < sizeof(upgrades) / sizeof(upgrades[0]); i++) {
		bool reached_target = false;

		if(upgrades[i].target == machine)
			reached_target = n_machines >= upgrades[i].req;
		else if(upgrades[i].target == clicks)
			reached_target = n_clicks >= upgrades[i].req;
		else if(upgrades[i].target == clicks_and_machine)
			reached_target = n_clicks >= upgrades[i].req && n_machines >= upgrades[i].req2;

		if(points < upgrades[i].cost || !reached_target)
			set_color(red);
		else
			set_color(white);

		uint16_t bw;
		if(do_button_ow(&bw, 0, (int16_t) (i + 2) * ui_scale, "%s %zu$", upgrades[i].name, upgrades[i].cost) && points >= upgrades[i].cost && reached_target) {
			upgrades[i].cb();
			upgrades[i].req += upgrades[i].req / 2;
			upgrades[i].req2 += upgrades[i].req2 / 2;
			upgrades[i].cost += upgrades[i].cost / 2;
		}

		set_color(white);
		if(!reached_target) {
			if(upgrades[i].target == machine)
				SDL_RenderDrawLine(renderer, 0, ((int) (i + 2) * ui_scale) + ui_scale - 2, (int) (((double) n_machines / (double) upgrades[i].req) * (double) bw), ((int) (i + 2) * ui_scale) + ui_scale - 2);
			else if(upgrades[i].target == clicks)
				SDL_RenderDrawLine(renderer, 0, ((int) (i + 2) * ui_scale) + ui_scale - 2, (int) (((double) n_clicks / (double) upgrades[i].req) * (double) bw), ((int) (i + 2) * ui_scale) + ui_scale - 2);
			else if(upgrades[i].target == clicks_and_machine) {
				if(n_clicks >= upgrades[i].req)
					SDL_RenderDrawLine(renderer, 0, ((int) (i + 2) * ui_scale) + ui_scale - 3, (int) bw, ((int) (i + 2) * ui_scale) + ui_scale - 3);
				else
					SDL_RenderDrawLine(renderer, 0, ((int) (i + 2) * ui_scale) + ui_scale - 3, (int) (((double) n_clicks / (double) upgrades[i].req) * (double) bw), ((int) (i + 2) * ui_scale) + ui_scale - 3);
				if(n_machines >= upgrades[i].req2)
					SDL_RenderDrawLine(renderer, 0, ((int) (i + 2) * ui_scale) + ui_scale - 2, (int) bw, ((int) (i + 2) * ui_scale) + ui_scale - 2);
				else
					SDL_RenderDrawLine(renderer, 0, ((int) (i + 2) * ui_scale) + ui_scale - 2, (int) (((double) n_machines / (double) upgrades[i].req2) * (double) bw), ((int) (i + 2) * ui_scale) + ui_scale - 2);
			}
		}
		else {
			if(upgrades[i].target == machine)
				SDL_RenderDrawLine(renderer, 0, ((int) (i + 2) * ui_scale) + ui_scale - 2, (int) bw, ((int) (i + 2) * ui_scale) + ui_scale - 2);
			else if(upgrades[i].target == clicks)
				SDL_RenderDrawLine(renderer, 0, ((int) (i + 2) * ui_scale) + ui_scale - 2, (int) bw, ((int) (i + 2) * ui_scale) + ui_scale - 2);
			else if(upgrades[i].target == clicks_and_machine) {
				SDL_RenderDrawLine(renderer, 0, ((int) (i + 2) * ui_scale) + ui_scale - 2, (int) bw, ((int) (i + 2) * ui_scale) + ui_scale - 2);
				SDL_RenderDrawLine(renderer, 0, ((int) (i + 2) * ui_scale) + ui_scale - 3, (int) bw, ((int) (i + 2) * ui_scale) + ui_scale - 3);
			}
		}
	}

	points_per_second = 0;
	points_per_second = (n_machines * machine_pps);
}
