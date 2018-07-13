/*******************
* chip8.cpp

*/

#include "memory.h"
#include "keyboard.h"
#include "display.h"
#include "chip8.h"

#include "computer.h"

#include "clock.h"

#include "gtkmm_gui.h"

#include <iostream>

/*****
* main()
*
* Create and test a CHIP-8 emulator
*****/
int main(int argc, char** argv)
{
	// Create a keyboard, memory and display
	Keyboard* keyboard = new Keyboard();
	Display* display = new Display();
	Memory* memory = new Memory();
	Chip8* chip8 = new Chip8(memory, display, keyboard);
	Clock* clock = new Clock(chip8);

	Computer* computer = new Computer(chip8, clock, memory, display, keyboard);

	computer->soft_reset();

	// Build the GUI, and start it up!
	GtkmmGui* gui = new GtkmmGui(computer, argc, argv);
	gui->build();

	chip8->add_listener(gui);
	
	clock->start();
//	clock->run();

	gui->run();

	delete clock;

	return 0;
}