#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <thread>
#include <unistd.h>

#include "core/chip8.h"

class Clock
{
	private:
		useconds_t clock_period;
		useconds_t delay_period;
		useconds_t sound_period;

		std::thread clock_thread;
		std::thread delay_thread;
		std::thread sound_thread;

		bool running;
		bool exists;

		Chip8* chip;

		void runChipClock();
		void runDelayClock();
		void runSoundClock();

	public:
		Clock(Chip8* );
		~Clock();
		void start();
		void run();
		void pause();
};

#endif