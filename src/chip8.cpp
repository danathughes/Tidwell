
#include "chip8.h"
#include "glade_gui.h"

#include "memory.h"
#include "display.h"
#include "keyboard.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define CALL_STACK_SIZE		64

/************
*
* Create a Chip-8 with default setup
************/
Chip8::Chip8()
{
	memory = new Memory();

	keyboard = new Keyboard();
	display = new Display();

	call_stack = new unsigned short[CALL_STACK_SIZE];

	refresh=false;

	std::cout << "Memory: " << memory << std::endl;
	std::cout << "Keyboard: " << keyboard << std::endl;
	std::cout << "Display: " << display << std::endl;
	// Seed a random number generator
	srand(time(NULL));
}


/************
*
* Create a Chip-8 with provided components
************/
Chip8::Chip8(Memory* _memory, Display* _display, Keyboard* _keyboard)
{
	memory = _memory;
	keyboard = _keyboard;
	display = _display;

	call_stack = new unsigned short[CALL_STACK_SIZE];

	refresh=false;

	std::cout << "In Chip:" << std::endl;
	std::cout << "Memory: " << memory << std::endl;
	std::cout << "Display: " << display << std::endl;
	std::cout << "Keyboard: " << keyboard << std::endl;
	// Seed a random number generator
	srand(time(NULL));
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


void Chip8::add_listener(GladeGui* _gui)
{
	gui = _gui;
}


void Chip8::load(const char* filename)
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
	unsigned short opcode = (memory->fetch(program_counter) << 8) | (memory->fetch(program_counter + 1));
	program_counter += 2;

	// Decrement the audio and delay timers, if non-zero
	if(delay_timer > 0)
	{
		delay_timer--;
	}
	if(sound_timer > 0)
	{
		sound_timer--;
	}

	// Pull out all possible variables from the opcode
	unsigned short address = 	(unsigned short) (opcode & 0x0FFF);			// 12-bit value
	unsigned char register_x = 	(unsigned char) ((opcode & 0x0F00) >> 8);	// 2nd nybble
	unsigned char register_y = 	(unsigned char) ((opcode & 0x00F0) >> 4);	// 3rd nyyble
	unsigned char value =		(unsigned char) (opcode & 0x00FF);			// Last byte used as value
	unsigned char nybble = 		(unsigned char) (opcode & 0x000F);

	/*
	std::cout << "Cycle --- Opcode: 0x" << std::hex << opcode << "  ";
	std::cout << "Address: 0x" << std::hex << address << "  ";
	std::cout << "Vx: 0x" << std::hex << (unsigned short) register_x << "  ";
	std::cout << "Vy: 0x" << std::hex << (unsigned short) register_y << "  ";
	std::cout << "Value: 0x" << std::hex << (unsigned short) value << std::endl;
	std::cout << "          Program Counter: 0x" << std::hex << program_counter << std::endl << std::endl;

//	display->show();
	*/

	// Decode the opcode and act accordingly
	// opcodes are organized roughly by first nybble
	switch(opcode & 0xF000)
	{
		case 0x0000:
			switch(opcode)
			{
				case 0x00E0:
					_clear_screen();
					break;
				case 0x00EE:
					_return();
					break;
				default:
					_system_call(address);
					break;
			}
			break;

		case 0x1000:
			_jump(address);
			break;

		case 0x2000:
			_call(address);
			break;

		case 0x3000:
			_skip_equal_register_value(register_x, value);
			break;

		case 0x4000:
			_skip_not_equal_register_value(register_x, value);
			break;

		case 0x5000:
			_skip_equal_register_register(register_x, register_y);
			break;

		case 0x6000:
			_assign_register_value(register_x, value);
			break;

		case 0x7000:
			_add_register_value(register_x, value);
			break;

		case 0x8000:
			switch(opcode & 0x000F)
			{
				case 0x0000:
					_assign_register_register(register_x, register_y);
					break;

				case 0x0001:
					_or(register_x, register_y);
					break;

				case 0x0002:
					_and(register_x, register_y);
					break;

				case 0x0003:
					_xor(register_x, register_y);
					break;

				case 0x0004:
					_add_register_register(register_x, register_y);
					break;

				case 0x0005:
					_subtract_register_register(register_x, register_y);
					break;

				case 0x0006:
					_shift_right(register_x);
					break;

				case 0x0007:
					_subtract_negative_register_register(register_x, register_y);
					break;

				case 0x000E:
					_shift_left(register_x);
					break;

				default:
					// Not a valid opcode
					_invalid_opcode(opcode);
					break;
			}
			break;

		case 0x9000:
			_skip_not_equal_register_register(register_x, register_y);
			break;

		case 0xA000:
			_set_address_register(address);
			break;

		case 0XB000:
			_jump_offset(address);
			break;

		case 0xC000:
			_random(register_x, value);
			break;

		case 0xD000:
			_draw(register_x, register_y, nybble);
			break;

		case 0xE000:
			switch(opcode & 0x00FF)
			{
				case 0x009E:
					_skip_key_pressed(register_x);
					break;

				case 0x00A1:
					_skip_key_not_pressed(register_x);
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
					_get_delay_timer(register_x);
					break;

				case 0x000A:
					_get_key(register_x);
					break;

				case 0x0015:
					_set_delay_timer(register_x);
					break;

				case 0x0018:
					_set_sound_timer(register_x);
					break;

				case 0x001E:
					_add_address_register(register_x);
					break;

				case 0x0029:
					_set_address_sprite(register_x);
					break;

				case 0x0033:
					_store_bcd(register_x);
					break;

				case 0x0055:
					_dump_register(register_x);
					break;

				case 0x0065:
					_load_register(register_x);
					break;

				default:
					_invalid_opcode(opcode);
					break;
			}
			break;

		default:
			_invalid_opcode(opcode);
			break;
	}


	for(unsigned char i=0; i<0x10; i++)
	{
		gui->update_register(i, registers[i]);
	}
	gui->update_program_counter(program_counter);
	gui->update_stack_pointer(stack_pointer);
	gui->update_address_register(address_register);

}


/**********************
* OPCODES
**********************/

/*********************
* _clear_screen
*
* Clear the display, set all bytes in the display memory to 0x00
*********************/
void Chip8::_clear_screen()
{
	// What are the bounds of the display memory?
	unsigned short display_start = memory->get_display_start();
	unsigned short display_end = display_start + memory->get_display_size();

	// Set all the bits in memory associated with the screen to 0
	for(unsigned short address=display_start; address<display_end; address++)
	{
		memory->dump(address, 0x00);
	}

	display->clear();
}


/*********************
* _return
*
* Set the program counter to the address on the top of the stack, then
* subtract 1 from the stack pointer.
*********************/
void Chip8::_return()
{
	// Is there somthing on the stack to pop?
	if(stack_pointer == 0)
	{
		std::cout << "ERROR:  No address on stack in RETURN" << std::endl;
		return;
	}

	// Set the program counter to the address on the top of the stack
	stack_pointer--;
	program_counter = call_stack[stack_pointer];
}


/*********************
* _system_call
*
* Jump to the system routine at the address provided.
*
* Parameters
*   unsigned short address - address of the routine
*********************/
void Chip8::_system_call(unsigned short address)
{
	// NOTE:  This instruction is ignored.
}


/*********************
* _jump
*
* Set the program counter to the provided address.
*
* Parameters
*   unsigned short address - address to jump to
*********************/
void Chip8::_jump(unsigned short address)
{
	program_counter = address;
}


/*********************
* _call
*
* Call the subroutine at the provided address.
* Push the stack pointer on the call stack, and increment the stack pointer.
*
* Parameters
*   unsigned short address - address of the subroutine to call
*********************/
void Chip8::_call(unsigned short address)
{
	// Is the stack full?
	if(stack_pointer == CALL_STACK_SIZE)
	{
		std::cout << "ERROR:  Stack full in CALL" << std::endl;
		return;
	}

	call_stack[stack_pointer] = program_counter;
	stack_pointer++;


	program_counter = address;
}


/*********************
* _skip_equal_register_value
*
* Skip the next instruction if the value in the register is equal to
* the given value
*
* Parameters
*   unsigned char register_x - which register to compare to
*   unsigned char value - the value to compre to
*********************/
void Chip8::_skip_equal_register_value(unsigned char register_x, unsigned char value)
{
	if(registers[register_x] == value)
	{
		program_counter += 2;
	}
}


/*********************
* _skip_not_equal_register_value
*
* Skip the next instruction if the value in the register is not equal to
* the given value
*
* Parameters
*   unsigned char register_x - which register to compare to
*   unsigned char value - the value to compre to
*********************/
void Chip8::_skip_not_equal_register_value(unsigned char register_x, unsigned char value)
{
	if(registers[register_x] != value)
	{
		program_counter += 2;
	}
}


/*********************
* _skip_equal_register_register
*
* Skip the next instruction if the value in the two registers are equal
*
* Parameters
*   unsigned char register_x - the first register to compare
*   unsigned char register_y - the second register to compare
*********************/
void Chip8::_skip_equal_register_register(unsigned char register_x, unsigned char register_y)
{
	if(registers[register_x] == registers[register_y])
	{
		program_counter += 2;
	}
}


/*********************
* _skip_not_equal_register_register
*
* Skip the next instruction if the value in the two registers are not equal
*
* Parameters
*   unsigned char register_x - the first register to compare
*   unsigned char register_y - the second register to compare
*********************/
void Chip8::_skip_not_equal_register_register(unsigned char register_x, unsigned char register_y)
{
	if(registers[register_x] != registers[register_y])
	{
		program_counter += 2;
	}

}


/*********************
* _assign_register_value
*
* Assign a value to a register
*
* Parameters
*   unsigned char register_x - the register to assign to
*   unsigned char value - the value to store in the register
*********************/
void Chip8::_assign_register_value(unsigned char register_x, unsigned char value)
{
	registers[register_x] = value;
}


/*********************
* _add_register_value
*
* Add a value to a register
*
* Parameters
*   unsigned char register_x - the register to add to
*   unsigned char value - the value to add to the register
*********************/
void Chip8::_add_register_value(unsigned char register_x, unsigned char value)
{
	registers[register_x] += value;
}


/*********************
* _assign_register_register
*
* Assign a value in a register to another register
*
* Parameters
*   unsigned char register_x - the register to assign to
*   unsigned char register_y - the register to assign from
*********************/
void Chip8::_assign_register_register(unsigned char register_x, unsigned char register_y)
{
	registers[register_x] = registers[register_y];
}


/*********************
* _or
*
* Perform a bitwise OR on the values in each register, and assign to Vx
*
* Parameters
*   unsigned char register_x - the register to assign to
*   unsigned char register_y - the other register
*********************/
void Chip8::_or(unsigned char register_x, unsigned char register_y)
{
	registers[register_x] = registers[register_x] | registers[register_y];
}


/*********************
* _and
*
* Perform a bitwise AND on the values in each register, and assign to Vx
*
* Parameters
*   unsigned char register_x - the register to assign to
*   unsigned char register_y - the other register
*********************/
void Chip8::_and(unsigned char register_x, unsigned char register_y)
{
	registers[register_x] = registers[register_x] & registers[register_y];
}


/*********************
* _xor
*
* Perform a bitwise XOR on the values in each register, and assign to Vx
*
* Parameters
*   unsigned char register_x - the register to assign to
*   unsigned char register_y - the other register
*********************/
void Chip8::_xor(unsigned char register_x, unsigned char register_y)
{
	registers[register_x] = registers[register_x] ^ registers[register_y];
}


/*********************
* _add_register_register
*
* Add the two register values, and assign to Vx.  Set Vf to the carry bit
*
* Parameters
*   unsigned char register_x - the register to assign to
*   unsigned char register_y - the other register
*********************/
void Chip8::_add_register_register(unsigned char register_x, unsigned char register_y)
{
	unsigned short total = (unsigned short) registers[register_x] + (unsigned short) registers[register_y];

	// Is there a carry bit?
	if(total > 0xFF)
	{
		registers[0x0F] = 0x01;
	}
	else
	{
		registers[0x0F] = 0x00;
	}

	// Assign the target register
	registers[register_x] = (unsigned char) (total & 0x00FF);
}


/*********************
* _subtract_register_register
*
* Subract Vy from Vx, and assign to Vx.  Set Vf to the borrow bit
*
* Parameters
*   unsigned char register_x - the register to assign to
*   unsigned char register_y - the other register
*********************/
void Chip8::_subtract_register_register(unsigned char register_x, unsigned char register_y)
{
	unsigned short difference = (unsigned short) registers[register_x];

	// Is a borrow necessary?
	if(registers[register_x] > registers[register_y])
	{
		registers[0x0F] = 0x01;		// Set the NOT BORROW bit
	}
	else
	{
		registers[0x0F] = 0x00;
		difference += 0x0100;		// Fill in the borrow bit
	}
	difference -= registers[register_y];

	registers[register_x] = (unsigned char) difference;	
}


/*********************
* _subtract_negative_register_register
*
* Subtract Vx from Vy, and assign to Vx.  Set Vf to the borrow bit
*
* Parameters
*   unsigned char register_x - the register to assign to
*   unsigned char register_y - the other register
*********************/
void Chip8::_subtract_negative_register_register(unsigned char register_x, unsigned char register_y)
{
	unsigned short difference = (unsigned short) registers[register_y];

	// Is a borrow necessary?
	if(registers[register_y] > registers[register_x])
	{
		registers[0x0F] = 0x01;		// Set the NOT BORROW bit
	}
	else
	{
		registers[0x0F] = 0x00;
		difference += 0x0100;		// Fill in the borrow bit
	}
	difference -= registers[register_x];

	registers[register_x] = (unsigned char) difference;	
}



/*********************
* _shift_right
*
* Shift the value in the register to the right, assign the value of
* the least significant bit to Vf
*
* Parameters
*   unsigned char register_x - the register to shift
*********************/
void Chip8::_shift_right(unsigned char register_x)
{
	// Assign the LSB to register F
	if((registers[register_x] & 0x01) != 0x00)
	{
		registers[0x0F] = 0x01;
	}
	else
	{
		registers[0x0F] = 0x00;
	}

	// Right shift - make sure that the shifted in bit is 0
	registers[register_x] = (registers[register_x] >> 1) | 0x7F;
}


/*********************
* _shift_left
*
* Shift the value in the register to the left, assign the value of
* the most significant bit to Vf
*
* Parameters
*   unsigned char register_x - the register to shift
*********************/
void Chip8::_shift_left(unsigned char register_x)
{
	// Assign the MSB to register F
	if((registers[register_x] & 0x80) != 0x00)
	{
		registers[0x0F] = 0x01;
	}
	else
	{
		registers[0x0F] = 0x00;
	}

	// Right shift - make sure that the shifted in bit is 0
	registers[register_x] = (registers[register_x] << 1) | 0xFE;
}


/*********************
* _set_address_register
*
* Set the address register to the provided value
*
* Parameters
*   unsigned short address - address to set the register to
*********************/
void Chip8::_set_address_register(unsigned short address)
{
	address_register = address;
}


/*********************
* _jump_offset
*
* Set the program counter to the provided address, offset by
* the value in V0.
*
* Parameters
*   unsigned short address - address to jump to
*********************/
void Chip8::_jump_offset(unsigned short address)
{
	// NOTE:  What if the program counter exceeds an allowable value?

	program_counter = address + registers[0x00];
}


/*********************
* _random
*
* Generate a random byte value, and AND it with the provided byte value
* and assign the results to register Vx
*
* Parameters
*   unsigned char register_x - the register to assign the value to
*	unsigned char value - the provided byte value
*********************/
void Chip8::_random(unsigned char register_x, unsigned char value)
{
	registers[register_x] = (rand() % 0x100) & value;
}


/*********************
* _draw
*
* Draw a sprite at the location (Vx, Vy).  The sprite is 8 pixels wide and
* N pixels high, where N is the value passed in.  The sprite is stored in the 
* current address register.
*
* Parameters
*   unsigned char register_x - the register containing the x coordinate of the sprite
*   unsigned char reigster_y - the register containing the y coordinate of the sprite
*	unsigned char value - the number of pixels in height of the sprite.
*********************/
void Chip8::_draw(unsigned char register_x, unsigned char register_y, unsigned char value)
{
	bool collision = false;

	unsigned char x = registers[register_x];
	unsigned char y = registers[register_y];

	// Read in N bytes, starting at the current address register
	for(int i=0; i<value; i++)
	{
		unsigned char line = memory->fetch(address_register + i);

		collision = collision || display->write_line(x, y+i, line);
	}
	if(collision)
	{
		registers[0x0F] = 0x01;
	}

	gui->refresh_display();
}


void Chip8::_skip_key_pressed(unsigned char register_x)
{
	unsigned char key = registers[register_x];

	if(keyboard->is_key_pressed(key))
	{
		program_counter += 2;
	}
}



void Chip8::_skip_key_not_pressed(unsigned char register_x)
{
	unsigned char key = registers[register_x];

	if(!keyboard->is_key_pressed(key))
	{
		program_counter += 2;
	}
}



void Chip8::_get_key(unsigned char register_x)
{
	bool got_key_press = false;

	// Check for a key press
	for(int i=0; i<0x0F; i++)
	{
		if(keyboard->is_key_pressed(i))
		{
			registers[register_x] = i;
			got_key_press = true;
			break;
		}
	}

	// if the key wasn't pressed, decrement the program counter by 2 to repeat this operations
	if(!got_key_press)
	{
		program_counter -= 2;
	}
	

}


/*********************
* _get_delay_timer
*
* Assign the current value of the delay timer to register x
*
* Parameters
*   unsigned char register_x - the register to assign the timer to
*********************/
void Chip8::_get_delay_timer(unsigned char register_x)
{
	registers[register_x] = delay_timer;
}


/*********************
* _set_delay_timer
*
* Assign the delay timer to the value in register x
*
* Parameters
*   unsigned char register_x - the register containing the delay timer value
*********************/
void Chip8::_set_delay_timer(unsigned char register_x)
{
	delay_timer = registers[register_x];
}


/*********************
* _set_sound_timer
*
* Assign the sound timer to the value in register x
*
* Parameters
*   unsigned char register_x - the register containing the sound timer value
*********************/
void Chip8::_set_sound_timer(unsigned char register_x)
{
	sound_timer = registers[register_x];
}


/*********************
* _add_address_register
*
* Add the value of register x to the address register
*
* Parameters
*   unsigned char register_x - the register to add to the address register
*********************/
void Chip8::_add_address_register(unsigned char register_x)
{
	address_register += registers[register_x];
}



void Chip8::_set_address_sprite(unsigned char register_x)
{
	address_register = memory->get_sprite_address(registers[register_x]);	
}



void Chip8::_store_bcd(unsigned char register_x)
{
	unsigned char value = registers[register_x];

	// What is the 100's, 10's and 1's?
	unsigned char value_100s = value / 100;
	value -= 100*(value_100s);

	unsigned char value_10s = value / 10;
	value -= 10*(value_10s);

	unsigned char value_1s = value;

	// Store the three bytes
	memory->dump(address_register, value_100s);
	memory->dump(address_register, value_10s);
	memory->dump(address_register, value_1s);
}


/*********************
* _dump_register
*
* Store registers V0 to Vx (including Vx) into memory starting at the address
* register.
*
* Parameters
*   unsigned char register_x - the upper register value to write to memory
*********************/
void Chip8::_dump_register(unsigned char register_x)
{
	// Loop through the registers and write to memory
	for(int reg_num=0; reg_num <= register_x; reg_num++)
	{
		memory->dump(address_register, registers[reg_num]);
		address_register++;
	}
}


/*********************
* _load_register
*
* Load registers V0 to Vx (including Vx) from memory starting at the address
* register.
*
* Parameters
*   unsigned char register_x - the upper register to load
*********************/
void Chip8::_load_register(unsigned char register_x)
{
	// Loop through the registers and write to memory
	for(int reg_num=0; reg_num <= register_x; reg_num++)
	{
		registers[reg_num] = memory->fetch(address_register);
		address_register++;
	}
}

void Chip8::_invalid_opcode(unsigned short opcode)
{
	std::cout << "INVALID OPCODE: 0x" << std::hex << opcode << std::endl;
}

unsigned char Chip8::get_register(unsigned char register_number)
{
	return registers[register_number];
}

unsigned short Chip8::get_address()
{
	return address_register;
}

unsigned short Chip8::get_program_counter()
{
	return program_counter;
}

unsigned char Chip8::get_stack_pointer()
{
	return stack_pointer;
}

