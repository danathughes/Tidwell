#include "core/schip8.h"
#include <iostream>

/************
*
* Create a SChip-8 with default setup
************/
SChip8::SChip8() 
	: Chip8()
{
	create_operation_map();
	graphicMode = LORES;
}


/************
*
* Create a SChip-8 with provided components
************/
SChip8::SChip8(Memory* _memory, Display* _display, Keyboard* _keyboard)
	: Chip8(_memory, _display, _keyboard)
{
	create_operation_map();
	graphicMode = LORES;
}


/************
*
* Create a SChip-8 with provided components, allowing user to override default call stack size
************/
SChip8::SChip8(Memory* _memory, Display* _display, Keyboard* _keyboard, unsigned char call_stack_size)
	: Chip8(_memory, _display, _keyboard, call_stack_size)
{
	create_operation_map();
	graphicMode = LORES;
}


void SChip8::create_operation_map()
{
	// Add the SCHIP operations
	operation_map.insert(std::make_pair(0x00C0, (void (Chip8::*) (unsigned short, unsigned char, unsigned char, unsigned char)) &SChip8::_scroll_down));
	operation_map.insert(std::make_pair(0x00FB, (void (Chip8::*) (unsigned short, unsigned char, unsigned char, unsigned char)) &SChip8::_scroll_right));
	operation_map.insert(std::make_pair(0x00FC, (void (Chip8::*) (unsigned short, unsigned char, unsigned char, unsigned char)) &SChip8::_scroll_left));
	operation_map.insert(std::make_pair(0x00FD, (void (Chip8::*) (unsigned short, unsigned char, unsigned char, unsigned char)) &SChip8::_exit));
	operation_map.insert(std::make_pair(0x00FE, (void (Chip8::*) (unsigned short, unsigned char, unsigned char, unsigned char)) &SChip8::_disable_extended_screen));
	operation_map.insert(std::make_pair(0x00FF, (void (Chip8::*) (unsigned short, unsigned char, unsigned char, unsigned char)) &SChip8::_enable_extended_screen));
	operation_map.insert(std::make_pair(0xF030, (void (Chip8::*) (unsigned short, unsigned char, unsigned char, unsigned char)) &SChip8::_set_address_big_sprite));
	operation_map.insert(std::make_pair(0xF075, (void (Chip8::*) (unsigned short, unsigned char, unsigned char, unsigned char)) &SChip8::_dump_register_rpl));
	operation_map.insert(std::make_pair(0xF085, (void (Chip8::*) (unsigned short, unsigned char, unsigned char, unsigned char)) &SChip8::_load_register_rpl));

	// Override the _draw method, since it's handled a bit differently (need to explicitly cast to the subclass method)
	operation_map[0xD000] = (void (Chip8::*) (unsigned short, unsigned char, unsigned char, unsigned char)) &SChip8::_draw;
}


/*************
* cycle()
*
* Emulate a single clock cycle.
************/
void SChip8::cycle()
{
//	std::cout << "PC: " << program_counter << std::endl;

	// Get the opcode from memory, and increment the program counter
	// The opcode takes two bytes in memory, stored big-endian
	unsigned short opcode = (memory->fetch(program_counter) << 8) | (memory->fetch(program_counter + 1));


//	std::cout << "0x" << std::hex << program_counter << ":\t" << std::hex << opcode << std::endl;

	program_counter += 2;

	gui->update_program_counter(program_counter);
	
	// Pull out all possible variables from the opcode
	unsigned short address = 	(unsigned short) (opcode & 0x0FFF);			// 12-bit value
	unsigned char register_x = 	(unsigned char) ((opcode & 0x0F00) >> 8);	// 2nd nybble
	unsigned char register_y = 	(unsigned char) ((opcode & 0x00F0) >> 4);	// 3rd nybble
	unsigned char value =		(unsigned char) (opcode & 0x00FF);			// Last byte used as value

	// Method to be called
	void (Chip8::* operation) (unsigned short, unsigned char, unsigned char, unsigned char) = NULL;

	// Decode the opcode and act accordingly
	// opcodes are organized roughly by first nybble
	unsigned short opnum = opcode & 0xF000;

	// Opcode prefix 0x0000 may also refer to 0x00Cn, 0x00E0, 0x00EE, 0x00FB, 0x00FC, 0x00FD, 0x00FE, or 0x00FF check if this
	// is the case
	if(opnum == 0x0000)
	{
		if((opcode & 0x00F0) == 0x00C0)
		{
			opnum = 0x00C0;
		}
		if((opcode & 0x00FF) == 0x00E0 || (opcode & 0x00FF) == 0x00EE || (opcode & 0x00FF) >= 0x00FB)
		{
			opnum = opcode & 0xF0FF;
		}
	}

	// Opcode prefix 0x8000 also relies on the last nybble
	// NOTE:  Check if this is critical for 0x5000 and 0x9000 as well.
	//        Do these operations necessarily need to end in 0?
	if((opnum == 0x5000) || (opnum == 0x8000) || (opnum==0x9000))
	{
		opnum = opcode & 0xF00F;
	}

	// Opcode prefix 0xE000 and 0xF000 rely on the last byte as well
	if((opnum == 0xE000) || (opnum == 0xF000))
	{
		opnum = opcode & 0xF0FF;
	}

	// Get the operation from the operation map, and execute
	// If the operation isn't in the map, inform that this is an invalid opcode
	if(operation_map.find(opnum) != operation_map.end())
	{
		operation = operation_map[opnum];
		(this->*operation) (address, register_x, register_y, value);
	}
	else		// Operation wasn't found, need to throw invalid opcode
	{
		_invalid_opcode(opcode);
	}

}

void SChip8::_scroll_down(unsigned short address, unsigned char register_x, unsigned char register_y, unsigned char value)
{
	unsigned char num_rows = value & 0x0F;
	display->scroll_down(num_rows);
}

void SChip8::_scroll_right(unsigned short address, unsigned char register_x, unsigned char register_y, unsigned char value)
{
	display->scroll_right(4);
}

void SChip8::_scroll_left(unsigned short address, unsigned char register_x, unsigned char register_y, unsigned char value)
{
	display->scroll_left(4);
}

void SChip8::_exit(unsigned short address, unsigned char register_x, unsigned char register_y, unsigned char value)
{
	// Not sure what to do here, may need to implement another abstraction layer...
	std::cout << "EXIT" << std::endl;
	while(1);
}

void SChip8::_enable_extended_screen(unsigned short address, unsigned char register_x, unsigned char register_y, unsigned char value)
{
	std::cout << "Extended screen mode" << std::endl;
	display->resize(128,64);
	graphicMode = HIRES;
}

void SChip8::_disable_extended_screen(unsigned short address, unsigned char register_x, unsigned char register_y, unsigned char value)
{
	std::cout << "Normal screen mode" << std::endl;
	display->resize(64,32);
	graphicMode = LORES;
}


/******
*
******/
void SChip8::_draw(unsigned short address, unsigned char register_x, unsigned char register_y, unsigned char value)
{
	bool collision = false;
	unsigned char num_lines = value & 0x0F;

	// If the number of rows given is 0, then draw a 16x16 sprite.  Otherwise, draw an Nx8 sprite
	if(num_lines == 0) 		// Need to draw 16 lines of 16 pixels
	{

		unsigned char x = registers[register_x];
		unsigned char y = registers[register_y];

		// Is the display 128x64, or 64x32??
		if(graphicMode == HIRES)
		{
			for(int i=0; i<32; i=i+2)
			{
				unsigned char line = memory->fetch(address_register + i);
				collision = display->write_line(x, y + (i/2),  line) || collision;

				line = memory->fetch(address_register + i + 1);
				collision = display->write_line(x+8, y + (i/2), line) || collision;
			}
		}
		else
		{
			for(int i=0; i<16; i++)
			{
				unsigned char line = memory->fetch(address_register + i);
				collision = display->write_line(x, y + i,  line) || collision;
			}			
		}
		
	}
	else 				// Draw using Chip-8 method
	{
		Chip8::_draw(address, register_x, register_y, value);
	}

	if(collision)
	{
		registers[0x0F] = 0x01;
	}
	else
	{
		registers[0x0F] = 0x00;
	}

	gui->refresh_display();
	gui->update_register(0x0F, registers[0x0F]);
}


void SChip8::_set_address_big_sprite(unsigned short address, unsigned char register_x, unsigned char register_y, unsigned char value)
{
	address_register = memory->get_big_sprite_address(registers[register_x]);

	gui->update_address_register(address_register);
}

void SChip8::_dump_register_rpl(unsigned short address, unsigned char register_x, unsigned char register_y, unsigned char value)
{
	// NOTE: Not sure what to do here.  May need to incorporate another hardware abstraction
	for(int i=0; i<=register_x; i++)
	{
		hp_registers[i] = registers[i];
	}
}

void SChip8::_load_register_rpl(unsigned short address, unsigned char register_x, unsigned char register_y, unsigned char value)
{
	// NOTE: Not sure what to do here.  May need to incorporate another hardware abstraction
	for(int i=0; i<=register_x; i++)
	{
		registers[i] = hp_registers[i];
	}
}

