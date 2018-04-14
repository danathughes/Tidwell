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

		// Start location of sprites
		unsigned short _sprite_memory_start;
		void load_sprites();

	public:
		Memory();
		unsigned char fetch(unsigned short);
		void dump(unsigned short, unsigned char);

		unsigned short get_ram_start();
		void print_memory(unsigned short, unsigned short);

		unsigned short get_display_start();
		unsigned short get_display_size();

		unsigned short get_sprite_address(unsigned char);

};

#endif