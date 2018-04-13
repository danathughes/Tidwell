/*******************
* chip8.cpp

*/

#include "memory.h"
#include "keyboard.h"
#include "display.h"
#include "chip8.h"

//#include "gtk_gui.h"
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

	std::cout << "Memory: " << std::hex << memory << std::endl;
	std::cout << "Display: " << std::hex << display << std::endl;
	std::cout << "Keyboard: " << std::hex << keyboard << std::endl;

	Chip8* chip8 = new Chip8(memory, display, keyboard);
	chip8->reset();
//	chip8->test();

	chip8->load("minimal.ch8");
	chip8->reset();

	// Build the GUI, and start it up!
	GladeGui* gui = new GladeGui(chip8, memory, display, keyboard, argc, argv);
	gui->build();
	gui->run();


	return 0;
}