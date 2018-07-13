#include "clock.h"
#include "chip8.h"

#include <thread>
#include <iostream>

Clock::Clock(Chip8* _chip)
{
	clock_period = 10000;
	delay_period = 16667;
	sound_period = 16667;

	running = false;
	exists = true;

	chip = _chip;
}

Clock::~Clock()
{
	exists = false;

	clock_thread.join();
	delay_thread.join();
	sound_thread.join();
}

void Clock::start()
{
	clock_thread = std::thread(&Clock::runChipClock, this);
	delay_thread = std::thread(&Clock::runDelayClock, this);
	sound_thread = std::thread(&Clock::runSoundClock, this);
}

void Clock::runChipClock()
{
	std::cout << "Starting Chip Clock" << std::endl;

	while(exists)
	{
		usleep(clock_period);
		if(running)
		{
			chip->cycle();
		}
	}
}

void Clock::runDelayClock()
{
	std::cout << "Starting Delay Clock" << std::endl;

	while(exists)
	{
		usleep(delay_period);
		if(running)
		{
			chip->cycle_delay();
		}
	}
}

void Clock::runSoundClock()
{
	std::cout << "Starting Sound Clock" << std::endl;

	while(exists)
	{
		usleep(sound_period);
		if(running)
		{
			chip->cycle_sound();
		}
	}
}

void Clock::run()
{
	running = true;
}

void Clock::pause()
{
	running = false;
}