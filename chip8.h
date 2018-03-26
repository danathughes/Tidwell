#ifndef __CHIP8_H__
#define __CHIP8_H__

#include "memory.h"
#include "display.h"
#include "keyboard.h"

class Chip8
{
	private:
		// Since memory can change for various implementations, utilize an
		// external memory object
		Memory* memory;

		// External keyboard and display
		Keyboard* keyboard;
		Display* display;

		// Registers -- V0 - VF
		unsigned char registers[16];

		// Address register -- I
		unsigned short address_register;

		// Call stack - Size allocated upon creation
		unsigned char* call_stack;
		unsigned char stack_pointer;

		// Timers
		unsigned char delay_timer;
		unsigned char sound_timer;

		// Program counter
		unsigned short program_counter;


		// Execution of opcodes -- each opcode takes a short (the actual opcode) as an argument
		void _clear_screen(unsigned short);
		void _return(unsigned short);
		void _system_call(unsigned short);
		void _jump(unsigned short);
		void _subroutine(unsigned short);
		void _skip_not_equal_value(unsigned short);
		void _skip_equal_register(unsigned short);
		void _store_register_value(unsigned short);
		void _add_register_value(unsigned short);
		void _store_register_register(unsigned short);
		void _or_register_register(unsigned short);
		void _and_register_register(unsigned short);
		void _xor_register_register(unsigned short);
		void _add_register_register_carry(unsigned short);
		void _subtract_register_register_borrow(unsigned short);
		void _shift_right(unsigned short);
		void _shift_left(unsigned short);
		void _skip_not_equal_register(unsigned short);
		void _set_address_register(unsigned short);
		void _jump_offset(unsigned short);
		void _random(unsigned short);
		void _draw(unsigned short);
		void _skip_key_pressed(unsigned short);
		void _skip_key_not_pressed(unsigned short);
		void _get_delay_timer(unsigned short);
		void _get_key(unsigned short);
		void _set_delay_timer(unsigned short);
		void _set_sound_timer(unsigned short);
		void _add_address_register(unsigned short);
		void _set_address_sprite(unsigned short);
		void _store_bcd(unsigned short);
		void _dump_register(unsigned short);
		void _load_register(unsigned short);
		void _invalid_opcode(unsigned short);


	public:
		// Constructors and destructors
		Chip8();

		// High-level instructions to reset the chip, load a program,
		// and perform a clock cycle
		void reset();
		void load();
		void cycle();
		void test();
};

#endif