/*******************
* chip8.cpp

*/

#include "memory.h"
#include "chip8.h"



/*****
* main()
*
* Create and test a CHIP-8 emulator
*****/
int main()
{
	Chip8* chip8 = new Chip8();
	chip8->reset();
	chip8->test();

	chip8->load("maze.ch8");

	return 0;
}