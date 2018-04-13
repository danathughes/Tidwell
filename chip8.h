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
		void _clear_screen();

		void _return();
		void _system_call(unsigned short);
		void _jump(unsigned short);
		void _jump_offset(unsigned short);
		void _call(unsigned short);

		void _skip_equal_register_value(unsigned char, unsigned char);
		void _skip_not_equal_register_value(unsigned char, unsigned char);
		void _skip_equal_register_register(unsigned char, unsigned char);
		void _skip_not_equal_register_register(unsigned char, unsigned char);

		void _assign_register_value(unsigned char, unsigned char);
		void _add_register_value(unsigned char, unsigned char);
		void _assign_register_register(unsigned char, unsigned char);

		void _or(unsigned char, unsigned char);
		void _and(unsigned char, unsigned char);
		void _xor(unsigned char, unsigned char);

		void _shift_right(unsigned char);
		void _shift_left(unsigned char);

		void _add_register_register(unsigned char, unsigned char);
		void _subtract_register_register(unsigned char, unsigned char);
		void _subtract_negative_register_register(unsigned char, unsigned char);

		void _set_address_register(unsigned short);

		void _random(unsigned char, unsigned char);

		void _draw(unsigned char, unsigned char, unsigned char);

		void _get_delay_timer(unsigned char);
		void _set_delay_timer(unsigned char);
		void _set_sound_timer(unsigned char);


		void _skip_key_pressed(unsigned char);
		void _skip_key_not_pressed(unsigned char);
		void _get_key(unsigned char);


		void _add_address_register(unsigned char);

		void _set_address_sprite(unsigned char);
		void _store_bcd(unsigned char);

		void _dump_register(unsigned char);
		void _load_register(unsigned char);


		void _invalid_opcode(unsigned short);


	public:
		// Constructors and destructors
		Chip8();

		// High-level instructions to reset the chip, load a program,
		// and perform a clock cycle
		void reset();
		void load(const char*);
		void cycle();
		void test();

		// Access to program counter, stack pointer, registers, etc.
		unsigned char get_register(unsigned char);
		unsigned short get_address();
		unsigned short get_program_counter();
		unsigned char get_stack_pointer();

		// Access to the display
		bool get_pixel(unsigned char, unsigned char);
		void press_key(unsigned char);
		void release_key(unsigned char);

		bool refresh;
};

#endif