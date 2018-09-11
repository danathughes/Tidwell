#include "view/simple_sdl_gui.h"
//#include "glade_gui.h"
#include <SDL2/SDL.h>

#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <map>

extern "C" {

/* Callback functions */



SimpleSDLGui::SimpleSDLGui(Computer* _computer, int argc, char** argv)
{
	// Initialize SDL -- return if it cannot
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize!  SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}

	computer = _computer;

	running = false;
}


SimpleSDLGui::~SimpleSDLGui()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}




void SimpleSDLGui::build()
{
	// Initialize the pointers, just in case
	window = NULL;
	renderer = NULL;
	screenSurface = NULL;

	// Create the main window and a renderer
	window = SDL_CreateWindow("Chip-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if(window == NULL)
	{
		std::cout << "Window could not be created!  SDL_ERROR: " << SDL_GetError() << std::endl;
		return;
	}

	// Create the renderer for the window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if(renderer == NULL)
	{
		std::cout << "Renderer could not be created!  SDL_ERROR: " << SDL_GetError();
	}

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
}


void SimpleSDLGui::draw()
{
	draw_screen(10, 10, 320, 160);
}


void SimpleSDLGui::draw_screen(int _x, int _y, int screen_width, int screen_height)
{
	// Clear out the screen (white background)
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	// How big to make each pixel?
	int pixel_width = (screen_width - _x) / computer->get_display_width();
	int pixel_height = (screen_height - _y) / computer->get_display_height();

	// Loop through the display and draw rectangles where the bits are set
	for(int x=0; x < computer->get_display_width(); x++)
	{
		for(int y=0; y < computer->get_display_height(); y++)
		{
			if(computer->get_pixel(x,y))
			{
				// Draw this pixel
				SDL_Rect pixel = {x*pixel_width + _x, y*pixel_height + _y, pixel_width, pixel_height};
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(renderer, &pixel);
			}
		}
	}

	// Update the window
	SDL_RenderPresent(renderer);
}


void SimpleSDLGui::run()
{
	// Timer to cap the number of frames per second to 60
	unsigned int startTime = 0;

	// Main execution flag
	bool quit = false;

	// Events to handle
	SDL_Event event;

	// Start the computer
	computer->run();

	// Run the application until quit
	while(!quit)
	{
		// Get the timer
		startTime = SDL_GetTicks();

		// Handle events
		while(SDL_PollEvent(&event) != 0)
		{
			// User requests a quit
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}

			// Handle key presses
			if(event.type == SDL_KEYDOWN)
			{
				// Handle each key
				if(event.key.keysym.sym == Key0)	computer->press_key(0x00);
				if(event.key.keysym.sym == Key1)	computer->press_key(0x01);			
				if(event.key.keysym.sym == Key2)	computer->press_key(0x02);
				if(event.key.keysym.sym == Key3)	computer->press_key(0x03);
				if(event.key.keysym.sym == Key4)	computer->press_key(0x04);
				if(event.key.keysym.sym == Key5)	computer->press_key(0x05);
				if(event.key.keysym.sym == Key6)	computer->press_key(0x06);
				if(event.key.keysym.sym == Key7)	computer->press_key(0x07);
				if(event.key.keysym.sym == Key8)	computer->press_key(0x08);
				if(event.key.keysym.sym == Key9)	computer->press_key(0x09);
				if(event.key.keysym.sym == KeyA)	computer->press_key(0x0A);
				if(event.key.keysym.sym == KeyB)	computer->press_key(0x0B);
				if(event.key.keysym.sym == KeyC)	computer->press_key(0x0C);
				if(event.key.keysym.sym == KeyD)	computer->press_key(0x0D);
				if(event.key.keysym.sym == KeyE)	computer->press_key(0x0E);
				if(event.key.keysym.sym == KeyF)	computer->press_key(0x0F);

				// Resize the display?
				if(event.key.keysym.sym == SDLK_UP)		computer->resize_display(128,64);
				if(event.key.keysym.sym == SDLK_DOWN)	computer->resize_display(64,32);
			}

			// Handle key releases
			if(event.type == SDL_KEYUP)
			{
				// Handle each key
				if(event.key.keysym.sym == Key0)	computer->release_key(0x00);
				if(event.key.keysym.sym == Key1)	computer->release_key(0x01);
				if(event.key.keysym.sym == Key2)	computer->release_key(0x02);
				if(event.key.keysym.sym == Key3)	computer->release_key(0x03);
				if(event.key.keysym.sym == Key4)	computer->release_key(0x04);
				if(event.key.keysym.sym == Key5)	computer->release_key(0x05);
				if(event.key.keysym.sym == Key6)	computer->release_key(0x06);
				if(event.key.keysym.sym == Key7)	computer->release_key(0x07);
				if(event.key.keysym.sym == Key8)	computer->release_key(0x08);
				if(event.key.keysym.sym == Key9)	computer->release_key(0x09);
				if(event.key.keysym.sym == KeyA)	computer->release_key(0x0A);
				if(event.key.keysym.sym == KeyB)	computer->release_key(0x0B);
				if(event.key.keysym.sym == KeyC)	computer->release_key(0x0C);
				if(event.key.keysym.sym == KeyD)	computer->release_key(0x0D);
				if(event.key.keysym.sym == KeyE)	computer->release_key(0x0E);
				if(event.key.keysym.sym == KeyF)	computer->release_key(0x0F);
			}
		}

		draw();

		// Wait until enough ticks have passed
		while(SDL_GetTicks() - startTime < SCREEN_TICKS_PER_FRAME);
//		SDL_Delay(50);

	}
}



/* Listener Functions */

void SimpleSDLGui::update_register(unsigned char register_number, unsigned char value)
{

}

void SimpleSDLGui::update_program_counter(unsigned short value)
{

}

void SimpleSDLGui::update_stack_pointer(unsigned short value)
{

}

void SimpleSDLGui::update_address_register(unsigned short value)
{

}


void SimpleSDLGui::update_delay_timer(unsigned short value)
{

}


void SimpleSDLGui::update_sound_timer(unsigned short value)
{

}


void SimpleSDLGui::update_memory()
{

}


void SimpleSDLGui::update_stack(unsigned short* stack, unsigned char num_values, unsigned char stack_size)
{

}


void SimpleSDLGui::refresh_display()
{

}






}	// extern "C"