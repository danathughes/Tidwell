/*******************
* chip8.cpp

*/

#include "memory.h"
#include "keyboard.h"
#include "display.h"
#include "chip8.h"

#include "computer.h"

#include "glade_gui.h"

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

	Computer* computer = new Computer(chip8, memory, display, keyboard);

	computer->soft_reset();

	// Build the GUI, and start it up!
	GladeGui* gui = new GladeGui(computer, argc, argv);

	chip8->add_listener(gui);
	
	gui->build();
	gui->run();


	return 0;
}