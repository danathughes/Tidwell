#ifndef __SIMPLE_SDL_GUI_H__
#define __SIMPLE_SDL_GUI_H__

#include "core/computer.h"
#include "core/chip_listener.h"

#include <SDL2/SDL.h>

const int SCREEN_FPS = 100;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

class SimpleSDLGui : public ChipListener
{
	private:
		// Overall screen size
		unsigned int window_width = 532;
		unsigned int window_height = 346;

		// Screen location
		unsigned int screen_x = 10;
		unsigned int screen_y = 10;
		unsigned int screen_width = 512;
		unsigned int screen_height = 256;

		// Run and step buttons
		unsigned int run_button_x = 10;
		unsigned int run_button_y = 276;
		unsigned int run_button_width = 60;
		unsigned int run_button_height = 60;

		unsigned int step_button_x = 80;
		unsigned int step_button_y = 276;
		unsigned int step_button_width = 60;
		unsigned int step_button_height = 60;

		// Game Title widget
		unsigned int game_title_x = 150;
		unsigned int game_title_y = 276;
		unsigned int game_title_width = 366;
		unsigned int game_title_height = 30;

		// SDL Screen
		SDL_Window*  window;
		SDL_Surface* screenSurface;
		SDL_Renderer* renderer;

		// Keyboard mapping
		SDL_Keycode Key0 = SDLK_x;
		SDL_Keycode Key1 = SDLK_1;
		SDL_Keycode Key2 = SDLK_2;
		SDL_Keycode Key3 = SDLK_3;
		SDL_Keycode Key4 = SDLK_q;
		SDL_Keycode Key5 = SDLK_w;
		SDL_Keycode Key6 = SDLK_e;
		SDL_Keycode Key7 = SDLK_a;
		SDL_Keycode Key8 = SDLK_s;
		SDL_Keycode Key9 = SDLK_d;
		SDL_Keycode KeyA = SDLK_z;
		SDL_Keycode KeyB = SDLK_c;
		SDL_Keycode KeyC = SDLK_4;
		SDL_Keycode KeyD = SDLK_r;
		SDL_Keycode KeyE = SDLK_f;
		SDL_Keycode KeyF = SDLK_v;

		void draw();
		void draw_screen(int, int, int, int);

	public:
		SimpleSDLGui(Computer*, int, char**);
		~SimpleSDLGui();

		Computer* computer;

		void build();
		void run();

		// Chip-8 Listener
		void update_register(unsigned char, unsigned char);
		void update_program_counter(unsigned short);
		void update_stack_pointer(unsigned short);
		void update_address_register(unsigned short);
		void update_delay_timer(unsigned short);
		void update_sound_timer(unsigned short);

		void update_memory();

		void refresh_display();

		void update_stack(unsigned short*, unsigned char, unsigned char);

		bool running;
};




#endif