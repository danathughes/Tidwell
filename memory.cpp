#include "memory.h"

#include <iostream>
#include <iomanip>

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

	// Allocate the sprite memory
	_sprite_memory_start = 0x000;
	load_sprites();	
}


void Memory::load_sprites()
{
	unsigned char sprite_list[80] = {0xF0,0x90,0x90,0x90,0xF0,
									 0x20,0x60,0x20,0x20,0x70,
									 0xF0,0x10,0xF0,0x80,0xF0,
									 0xF0,0x10,0xF0,0x10,0xF0,
									 0x90,0x90,0xF0,0x10,0x10,
									 0xF0,0x80,0xF0,0x10,0xF0,
									 0xF0,0x80,0xF0,0x90,0xF0,
									 0xF0,0x10,0x20,0x40,0x40,
									 0xF0,0x90,0xF0,0x90,0xF0,
									 0xF0,0x90,0xF0,0x10,0xF0,
									 0xF0,0x90,0xF0,0x90,0x90,
									 0xE0,0x90,0xE0,0x90,0xE0,
									 0xF0,0x80,0x80,0x80,0xF0,
									 0xE0,0x90,0x90,0x90,0xE0,
									 0xF0,0x80,0xF0,0x80,0xF0,
									 0xF0,0x80,0xF0,0x80,0x80};
	for(int i=0; i<80; i++)
	{
		memory[_sprite_memory_start + i] = sprite_list[i];
	}
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

/*******************
* void dump(short address, char value)
*
* Write the byte to the provided address
*
* Parameters:
*   address - memory location to write to
*   value   - byte value to write
*******************/
void Memory::dump(unsigned short address, unsigned char value)
{
	// Check to see if the address is a writable location.
	if(address < _ram_start)
	{
		std::cout << "MEMORY ERROR: Attempting to write to memory address " << address << std::endl;
		return;
	}

	memory[address] = value;
}


unsigned short Memory::get_ram_start()
{
	return _ram_start;
}

unsigned short Memory::get_display_start()
{
	return _display_refresh_start;
}

unsigned short Memory::get_display_size()
{
	return 0xFF;
}


void Memory::print_memory(unsigned short address, unsigned short num_bytes)
{
	for(unsigned short i=address; i<address+num_bytes; i++)
	{
		std::cout << std::setw(2) << std::hex << (unsigned short) memory[i] << " ";
	}
	std::cout << std::endl;
}

/*******************
* unsigned short get_sprite_address(unsigned char value)
*
* Get the address for the sprite in ROM for the provided character
*
* Parameters:
*   value   - the hexadecimal value of the sprite in ROM
*******************/
unsigned short Memory::get_sprite_address(unsigned char value)
{
	// Each sprite if 5 bytes in size
	return _sprite_memory_start + 5*value;
}