#include "core/computer.h"

#include <iostream>
#include <fstream>

Computer::Computer()
{
	chip = new Chip8();
	memory = new Memory();
	display = new Display();
	keyboard = new Keyboard();
}


Computer::Computer(Chip8* _chip8, Clock* _clock, Memory* _memory, Display* _display, Keyboard* _keyboard)
{
	chip = _chip8;
	memory = _memory;
	display = _display;
	keyboard = _keyboard;
	clock = _clock;
}

Computer::~Computer()
{

}

void Computer::press_key(unsigned char key_num)
{
	keyboard->press_key(key_num);
}


void Computer::release_key(unsigned char key_num)
{
	keyboard->release_key(key_num);
}

void Computer::cycle()
{
	chip->cycle();
}

bool Computer::get_pixel(unsigned char x, unsigned char y)
{
	return display->get_pixel(x, y);
}

void Computer::run()
{
	clock->run();
}

void Computer::pause()
{
	clock->pause();
}


/*************
* load()
*
* Load a program into memory.
************/
void Computer::load(const char* filename)
{
	// Open the file
	std::ifstream romfile;
	romfile.open(filename, std::ios::in | std::ios::binary);

	// If the file didn't open, return
	if(!romfile.is_open())
	{
		std::cout << "ERROR: File " << filename << " did not open!" << std::endl;
		return;
	}

	// Load the entirity of the file into a char array, then write to memory
	// NOTE:  Should the file just be passed to memory to load directly?

	// Figure out how long the file is
	long begin, end;
	begin = romfile.tellg();
	romfile.seekg(0, std::ios::end);
	end = romfile.tellg();
	int size = (int) (end - begin);

	// Load the file
	char* memblock = new char[size];

	romfile.seekg(0, std::ios::beg);
	romfile.read(memblock, size);
	romfile.close();

	// Dump the loaded file into memory
	unsigned short start_address = memory->get_ram_start();
	std::cout << "Start address of RAM: 0x" << std::hex << start_address << std::endl;
	for(int i=0; i<size; i++)
	{
		memory->dump(start_address + i, memblock[i]);
	}

	delete [] memblock;

	memory->print_memory(start_address, size);
}

void Computer::soft_reset()
{
	chip->reset();
	display->clear();
}

std::string Computer::get_memory_string()
{
	return (memory->to_string(16));
}