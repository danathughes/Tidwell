#ifndef __MEMORY_H__
#define __MEMORY_H__

/******************
* Memory
*
* Storage and mapping to RAM, interpreter, and video refresh
******************/
class Memory
{
	private:
		// Actual location of memory on the heap
		unsigned short memory_size;
		unsigned char* memory;

		// Boundaries of each section of memory
		unsigned short _system_memory_start;
		unsigned short _ram_start;
		unsigned short _call_stack_start;
		unsigned short _display_refresh_start;

	public:
		Memory();
		unsigned char fetch(unsigned short);
		
};

#endif