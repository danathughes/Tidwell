/*******************
* chip8.cpp

*/

#include "memory.h"
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
	Chip8* chip8 = new Chip8();
	chip8->reset();
//	chip8->test();

	chip8->load("maze.ch8");
	chip8->reset();

	chip8->press_key(3);

	std::cout << "Chip 8: " << std::hex << chip8 << std::endl;

	// Build the GUI, and start it up!
	GladeGui* gui = new GladeGui(chip8, argc, argv);
	gui->build();
	gui->run();


	return 0;
}