#ifndef __SCHIP8_H__
#define __SCHIP8_H__

#include "core/chip8.h"

class SChip8 : public Chip8
{
	protected:
		// HP Registers
		unsigned char hp_registers[8];

		void _scroll_down(unsigned short, unsigned char, unsigned char, unsigned char);
		void _scroll_right(unsigned short, unsigned char, unsigned char, unsigned char);
		void _scroll_left(unsigned short, unsigned char, unsigned char, unsigned char);
		void _exit(unsigned short, unsigned char, unsigned char, unsigned char);
		void _enable_extended_screen(unsigned short, unsigned char, unsigned char, unsigned char);
		void _disable_extended_screen(unsigned short, unsigned char, unsigned char, unsigned char);

		void _draw(unsigned short, unsigned char, unsigned char, unsigned char);

		void _set_address_big_sprite(unsigned short, unsigned char, unsigned char, unsigned char);
		void _dump_register_rpl(unsigned short, unsigned char, unsigned char, unsigned char);
		void _load_register_rpl(unsigned short, unsigned char, unsigned char, unsigned char);

		void create_operation_map();

	public:
		// Constructors and destructors
		SChip8();
		SChip8(Memory*, Display*, Keyboard*);
		SChip8(Memory*, Display*, Keyboard*, unsigned char);

		void cycle();
};

#endif