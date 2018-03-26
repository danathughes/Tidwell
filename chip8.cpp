
#include "chip8.h"

#include "memory.h"
#include "display.h"
#include "keyboard.h"

#include <iostream>

/************
*
* Create a Chip-8 with default setup
************/
Chip8::Chip8()
{
	memory = new Memory();

	keyboard = new Keyboard();
	display = new Display();

	call_stack = new unsigned char[64];
}


/*************
* reset()
*
* Set the registers, stack pointer, timers and program counter to
* initial values
************/
void Chip8::reset()
{
	// Set all the registers to 0x00
	for(int i=0; i<16; i++)
	{
		registers[i] = 0x00;
	}

	// Set the timers to 0
	delay_timer = 0x00;
	sound_timer = 0x00;

	// Set the stack pointer to 0x00
	// NOTE: Should the call stack be flushed / set to zero?
	stack_pointer = 0x00;

	// Set the address register to 0x00
	address_register = 0x00;

	// Set the program counter to the start of the program memory
	program_counter = 0x200;
}



/*************
* load()
*
* Load a program into memory.
************/
void Chip8::load()
{

}

/*************
* cycle()
*
* Emulate a single clock cycle.
************/
void Chip8::cycle()
{
	// Get the opcode from memory, and increment the program counter
	// The opcode takes two bytes in memory, stored big-endian
	short opcode = (memory->fetch(program_counter) << 8) | (memory->fetch(program_counter + 1));
	program_counter += 2;


	// Decode the opcode and act accordingly
	// opcodes are organized roughly by first nybble
	switch(opcode & 0xF000)
	{
		case 0x0000:
			switch(opcode)
			{
				case 0x00E0:
					_clear_screen(opcode);
					break;
				case 0x00EE:
					_return(opcode);
					break;
				default:
					_system_call(opcode);
					break;

			}
			break;

		case 0x1000:
			_jump(opcode);
			break;

		case 0x2000:
			_subroutine(opcode);
			break;

		case 0x3000:
			_skip_not_equal_value(opcode);
			break;

		case 0x4000:
			_skip_not_equal_value(opcode);
			break;

		case 0x5000:
			_skip_equal_register(opcode);
			break;

		case 0x6000:
			_store_register_value(opcode);
			break;

		case 0x7000:
			_add_register_value(opcode);
			break;

		case 0x8000:
			switch(opcode & 0x000F)
			{
				case 0x0000:
					_store_register_register(opcode);
					break;

				case 0x0001:
					_or_register_register(opcode);
					break;

				case 0x0002:
					_and_register_register(opcode);
					break;

				case 0x0003:
					_xor_register_register(opcode);
					break;

				case 0x0004:
					_add_register_register_carry(opcode);
					break;

				case 0x0005:
					_subtract_register_register_borrow(opcode);
					break;

				case 0x0006:
					_shift_right(opcode);
					break;

				case 0x0007:
					_subtract_register_register_borrow(opcode);
					break;

				case 0x000E:
					_shift_left(opcode);
					break;

				default:
					// Not a valid opcode
					_invalid_opcode(opcode);
					break;
			}
			break;

		case 0x9000:
			_skip_not_equal_register(opcode);
			break;

		case 0xA000:
			_set_address_register(opcode);
			break;

		case 0XB000:
			_jump_offset(opcode);
			break;

		case 0xC000:
			_random(opcode);
			break;

		case 0xD000:
			_draw(opcode);
			break;

		case 0xE000:
			switch(opcode & 0x00FF)
			{
				case 0x009E:
					_skip_key_pressed(opcode);
					break;

				case 0x00A1:
					_skip_key_not_pressed(opcode);
					break;

				default:
					// Not a valid opcode
					_invalid_opcode(opcode);
					break;
			}
			break;

		case 0xF000:
			switch(opcode & 0x00FF)
			{
				case 0x0007:
					_get_delay_timer(opcode);
					break;

				case 0x000A:
					_get_key(opcode);
					break;

				case 0x0015:
					_set_delay_timer(opcode);
					break;

				case 0x0018:
					_set_sound_timer(opcode);
					break;

				case 0x001E:
					_add_address_register(opcode);
					break;

				case 0x0029:
					_set_address_sprite(opcode);
					break;

				case 0x0033:
					_store_bcd(opcode);
					break;

				case 0x0055:
					_dump_register(opcode);
					break;

				case 0x0065:
					_load_register(opcode);
					break;

				default:
					_invalid_opcode(opcode);
					break;
			}

		default:
			_invalid_opcode(opcode);
			break;
	}

}


/**********************
* OPCODES
**********************/

void Chip8::_clear_screen(unsigned short opcode)
{

}

void Chip8::_return(unsigned short opcode)
{

}

void Chip8::_system_call(unsigned short opcode)
{

}

void Chip8::_jump(unsigned short opcode)
{

}

void Chip8::_subroutine(unsigned short opcode)
{

}

void Chip8::_skip_not_equal_value(unsigned short opcode)
{

}

void Chip8::_skip_equal_register(unsigned short opcode)
{

}

void Chip8::_store_register_value(unsigned short opcode)
{

}

void Chip8::_add_register_value(unsigned short opcode)
{

}

void Chip8::_store_register_register(unsigned short opcode)
{

}

void Chip8::_or_register_register(unsigned short opcode)
{

}

void Chip8::_and_register_register(unsigned short opcode)
{

}

void Chip8::_xor_register_register(unsigned short opcode)
{

}

void Chip8::_add_register_register_carry(unsigned short opcode)
{

}

void Chip8::_subtract_register_register_borrow(unsigned short opcode)
{

}

void Chip8::_shift_right(unsigned short opcode)
{

}

void Chip8::_shift_left(unsigned short opcode)
{

}

void Chip8::_skip_not_equal_register(unsigned short opcode)
{

}

void Chip8::_set_address_register(unsigned short opcode)
{

}

void Chip8::_jump_offset(unsigned short opcode)
{

}

void Chip8::_random(unsigned short opcode)
{

}

void Chip8::_draw(unsigned short opcode)
{

}

void Chip8::_skip_key_pressed(unsigned short opcode)
{

}

void Chip8::_skip_key_not_pressed(unsigned short opcode)
{

}

void Chip8::_get_delay_timer(unsigned short opcode)
{

}

void Chip8::_get_key(unsigned short opcode)
{

}

void Chip8::_set_delay_timer(unsigned short opcode)
{

}

void Chip8::_set_sound_timer(unsigned short opcode)
{

}

void Chip8::_add_address_register(unsigned short opcode)
{

}

void Chip8::_set_address_sprite(unsigned short opcode)
{

}

void Chip8::_store_bcd(unsigned short opcode)
{

}

void Chip8::_dump_register(unsigned short opcode)
{

}

void Chip8::_load_register(unsigned short opcode)
{

}

void Chip8::_invalid_opcode(unsigned short opcode)
{

}



void Chip8::test()
{
	std::cout << "Fetching memory at location 0x456: " << memory->fetch(0x456) << std::endl;
	std::cout << "Attempting memory fetch at location 0x2345: "  << memory->fetch(0x2345) << std::endl;
}