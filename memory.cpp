#include "memory.h"

#include <iostream>

/*****************
* Memory()
*
* Create a Memory object with the default Chip-8 settings:
*   0x000-0x1FF:  Interpreter / ROM (fonts, etc)
*   0x200-0xE9F:  RAM
*   0xEA0-0XEFF:  Call Stack
*   0xF00-0XFFF:  Display refresh  
*/
Memory::Memory()
{
	// Allocate 4K of memory to the heap
	memory_size = 0x1000;
	memory = new unsigned char[memory_size];

	_system_memory_start = 0x000;
	_ram_start = 0x200;
	_call_stack_start = 0xEA0;
	_display_refresh_start = 0xF00;
}


/*******************
* unsigned char fetch(short address)
*
* Return the byte at the give address in memory
*
* Parameters:
*   address - memory location to retrieve a byte from
*
* Return:
*   value at the provided memory address
******************/
unsigned char Memory::fetch(unsigned short address)
{
	// NOTE: May need to worry about illegal access?

	// Check to see if the address is within the memory size
	if(address >= memory_size)
	{
		std::cout << "MEMORY ERROR: Attemping to access memory address " << address << ", memory size " << memory_size << std::endl;
		return 0;			// What else to do?
	}

	return memory[address];

}