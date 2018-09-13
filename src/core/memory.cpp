#include "core/memory.h"

#include <iostream>
#include <iomanip>

#include <string>
#include <sstream>

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

	for(int i=0; i<memory_size; i++)
	{
		memory[i] = 0x00;
	}

	_system_memory_start = 0x000;
	_ram_start = 0x200;
	_call_stack_start = 0xEA0;
	_display_refresh_start = 0xF00;

	// Allocate the sprite memory
	_sprite_memory_start = 0x000;
	_big_sprite_memory_start = 0x050;
	load_sprites();	
	load_big_sprites();
}


void Memory::load_sprites()
{
	unsigned char sprite_list[80] = {0xF0,0x90,0x90,0x90,0xF0,		// 0
									 0x20,0x60,0x20,0x20,0x70,		// 1
									 0xF0,0x10,0xF0,0x80,0xF0,		// 2
									 0xF0,0x10,0xF0,0x10,0xF0,		// 3
									 0x90,0x90,0xF0,0x10,0x10,		// 4
									 0xF0,0x80,0xF0,0x10,0xF0,		// 5
									 0xF0,0x80,0xF0,0x90,0xF0,		// 6
									 0xF0,0x10,0x20,0x40,0x40,		// 7
									 0xF0,0x90,0xF0,0x90,0xF0,		// 8
									 0xF0,0x90,0xF0,0x10,0xF0,		// 9
									 0xF0,0x90,0xF0,0x90,0x90,		// A
									 0xE0,0x90,0xE0,0x90,0xE0,		// B
									 0xF0,0x80,0x80,0x80,0xF0,		// C
									 0xE0,0x90,0x90,0x90,0xE0,		// D
									 0xF0,0x80,0xF0,0x80,0xF0,		// E
									 0xF0,0x80,0xF0,0x80,0x80};		// F
	for(int i=0; i<80; i++)
	{
		memory[_sprite_memory_start + i] = sprite_list[i];
	}
}


void Memory::load_big_sprites()
{
	unsigned char sprite_list[100] = {0x3C,0x7E,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0x7E,0x3C,	// 0
									  0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x58,0x38,0x18,	// 1
									  0xFF,0xFF,0x60,0x30,0x18,0x0C,0x06,0xC3,0x7F,0x3E,	// 2
									  0x3C,0x7E,0xC3,0x03,0x0E,0x0E,0x03,0xC3,0x7E,0x3C,	// 3
									  0x06,0x06,0xFF,0xFF,0xC6,0x66,0x36,0x1E,0x0E,0x06,	// 4
									  0x3C,0x7E,0xC3,0x03,0xFE,0xFC,0xC0,0xC0,0xFF,0xFF,	// 5
									  0x3C,0x7E,0xC3,0xC3,0xFE,0xFC,0xC0,0xC0,0x7C,0x3E,	// 6
									  0x60,0x60,0x60,0x30,0x18,0x0C,0x06,0x03,0xFF,0xFF,	// 7
									  0x3C,0x7E,0xC3,0xC3,0x7E,0x7E,0xC3,0xC3,0x7E,0x3C,	// 8
									  0x7C,0x3E,0x03,0x03,0x3F,0x7F,0xC3,0xC3,0x7E,0x3C,	// 9
									  };

	for(int i=0; i<100; i++)
	{
		memory[_big_sprite_memory_start + i] = sprite_list[i];
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
		std::cout << "MEMORY ERROR: Attemping to access memory address " << address << ", memory size " << (unsigned short) memory_size << std::endl;
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


unsigned short Memory::get_big_sprite_address(unsigned char value)
{
	return _big_sprite_memory_start + 10*value;
}

/*******************
* string to_string(unsigned int bytes_per_line)
*
* Create a string consisting of the bytes in memory.  Each line in the string
* is formatted as follows:
*
* 0xXXXX: XX XX XX XX XX XX ...
* 
* The address of the first byte on the line is displayed, followed by the sequence of bytes
* in memory.
*******************/
std::string Memory::to_string(unsigned int bytes_per_line)
{
	std::stringstream ss;

	// Clear the string stream
	ss.clear();
	ss.str(std::string());

	// Set the current address
	unsigned short current_address = 0x0000;

	// Loop through the address until all bytes are written to the string
	while(current_address < memory_size)
	{
		// Should the address be written (i.e., new line?)
		if((current_address % bytes_per_line) == 0)
		{
			ss << "0x";
			// Prepend the address appropriately
			if(current_address < 0x0FFF)	ss << "0";
			if(current_address < 0x00FF)	ss << "0";
			if(current_address < 0x000F)	ss << "0";

			ss << std::hex << current_address << ": ";
		}

		// Write the current byte -- prepend a 0 if necessary
		if(memory[current_address] < 0x0F)	ss << "0";

		ss << std::hex << (unsigned short) memory[current_address];

		// Increment the address, then write a space or newline as appropriate
		current_address++;
		if((current_address % bytes_per_line) == 0)
		{
			ss << "\n";
		}
		else
		{
			ss << " ";
		}
	}

	// All done!
	return ss.str();
}