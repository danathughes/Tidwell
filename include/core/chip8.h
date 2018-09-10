#ifndef __CHIP8_H__
#define __CHIP8_H__

class GladeGui;

#include "core/chip_listener.h"

#include "core/memory.h"
#include "core/display.h"
#include "core/keyboard.h"

#include <map>

class Chip8
{
	private:
		// Since memory can change for various implementations, utilize an
		// external memory object
		Memory* memory;

		// External keyboard and display
		Keyboard* keyboard;
		Display* display;

		ChipListener* gui;

		// A map used to convert operation codes to methods
		std::map <unsigned short, void (Chip8::* ) (unsigned short, unsigned char, unsigned char, unsigned char)> operation_map;

		// Registers -- V0 - VF
		unsigned char registers[16];

		// Address register -- I
		unsigned short address_register;

		// Call stack - Size allocated upon creation
		unsigned short* call_stack;
		unsigned char stack_pointer;

		// Timers
		unsigned short delay_timer;
		unsigned short sound_timer;

		// Program counter
		unsigned short program_counter;


		// Execution of opcodes -- each opcode takes a short (the actual opcode) as an argument
		void _clear_screen(unsigned short, unsigned char, unsigned char, unsigned char);

		void _return(unsigned short, unsigned char, unsigned char, unsigned char);
		void _system_call(unsigned short, unsigned char, unsigned char, unsigned char);
		void _jump(unsigned short, unsigned char, unsigned char, unsigned char);
		void _jump_offset(unsigned short, unsigned char, unsigned char, unsigned char);
		void _call(unsigned short, unsigned char, unsigned char, unsigned char);
		void _skip_equal_register_value(unsigned short, unsigned char, unsigned char, unsigned char);
		void _skip_not_equal_register_value(unsigned short, unsigned char, unsigned char, unsigned char);
		void _skip_equal_register_register(unsigned short, unsigned char, unsigned char, unsigned char);
		void _skip_not_equal_register_register(unsigned short, unsigned char, unsigned char, unsigned char);
		void _assign_register_value(unsigned short, unsigned char, unsigned char, unsigned char);
		void _add_register_value(unsigned short, unsigned char, unsigned char, unsigned char);
		void _assign_register_register(unsigned short, unsigned char, unsigned char, unsigned char);
		void _or(unsigned short, unsigned char, unsigned char, unsigned char);
		void _and(unsigned short, unsigned char, unsigned char, unsigned char);
		void _xor(unsigned short, unsigned char, unsigned char, unsigned char);
		void _shift_right(unsigned short, unsigned char, unsigned char, unsigned char);
		void _shift_left(unsigned short, unsigned char, unsigned char, unsigned char);
		void _add_register_register(unsigned short, unsigned char, unsigned char, unsigned char);
		void _subtract_register_register(unsigned short, unsigned char, unsigned char, unsigned char);
		void _subtract_negative_register_register(unsigned short, unsigned char, unsigned char, unsigned char);
		void _set_address_register(unsigned short, unsigned char, unsigned char, unsigned char);
		void _random(unsigned short, unsigned char, unsigned char, unsigned char);
		void _draw(unsigned short, unsigned char, unsigned char, unsigned char);
		void _get_delay_timer(unsigned short, unsigned char, unsigned char, unsigned char);
		void _set_delay_timer(unsigned short, unsigned char, unsigned char, unsigned char);
		void _set_sound_timer(unsigned short, unsigned char, unsigned char, unsigned char);
		void _skip_key_pressed(unsigned short, unsigned char, unsigned char, unsigned char);
		void _skip_key_not_pressed(unsigned short, unsigned char, unsigned char, unsigned char);
		void _get_key(unsigned short, unsigned char, unsigned char, unsigned char);
		void _add_address_register(unsigned short, unsigned char, unsigned char, unsigned char);
		void _set_address_sprite(unsigned short, unsigned char, unsigned char, unsigned char);
		void _store_bcd(unsigned short, unsigned char, unsigned char, unsigned char);
		void _dump_register(unsigned short, unsigned char, unsigned char, unsigned char);
		void _load_register(unsigned short, unsigned char, unsigned char, unsigned char);

		void _invalid_opcode(unsigned short);

		void create_operation_map();

	public:
		// Constructors and destructors
		Chip8();
		Chip8(Memory*, Display*, Keyboard*);

		// High-level instructions to reset the chip, load a program,
		// and perform a clock cycle
		void reset();
		void cycle();
		void cycle_delay();
		void cycle_sound();
		void test();

		// Listeners
		void add_listener(ChipListener*);

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