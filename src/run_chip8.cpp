/*******************
* chip8.cpp

*/

#include "core/memory.h"
#include "core/keyboard.h"
#include "core/display.h"
//#include "core/chip8.h"
#include "core/schip8.h"

#include "core/computer.h"

#include "core/clock.h"

#include "view/gtkmm_gui.h"
#include "view/simple_sdl_gui.h"

#include <iostream>

/*****
* main()
*
* Create and test a CHIP-8 emulator
*****/
int main(int argc, char** argv)
{
	// Did the user provide a file to run?
	if(argc < 2)
	{
		std::cout << "Program file not provided!  USAGE:  RunChip8 <program.ch8>" << std::endl;
		return 0;
	}

	// Create a keyboard, memory and display
	Keyboard* keyboard = new Keyboard();
	Display* display = new Display();
	Memory* memory = new Memory();
	SChip8* chip8 = new SChip8(memory, display, keyboard);
	Clock* clock = new Clock(chip8);

	Computer* computer = new Computer((Chip8*)chip8, clock, memory, display, keyboard);

	computer->soft_reset();

	// Build the GUI, and start it up!
//	GtkmmGui* gui = new GtkmmGui(computer, argc, argv);
	SimpleSDLGui* gui = new SimpleSDLGui(computer, argc, argv);
	gui->build();

	chip8->add_listener(gui);
	
	clock->start();

	computer->load(argv[1]);

	gui->run();


	delete clock;

	return 0;
}