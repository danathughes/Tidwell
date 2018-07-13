#ifndef __COMPUTER_H__
#define __COMPUTER_H__

#include "chip8.h"
#include "memory.h"
#include "display.h"
#include "keyboard.h"
#include "clock.h"

class Computer
{
	private:
		Chip8* chip;
		Memory* memory;
		Keyboard* keyboard;
		Display* display;
		Clock* clock;

	public:
		Computer();
		Computer(Chip8*, Clock*, Memory*, Display*, Keyboard*);
		~Computer();

		void press_key(unsigned char);
		void release_key(unsigned char);

		bool get_pixel(unsigned char, unsigned char);

		void cycle();
		void run();
		void pause();

		void load(const char*);
		void soft_reset();

		std::string get_memory_string();
};

#endif