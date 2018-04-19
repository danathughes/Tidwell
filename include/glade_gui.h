#ifndef __GLADE_GUI_H__
#define __GLADE_GUI_H__

#include "chip8.h"
#include "computer.h"

#include <gtk/gtk.h>

class GladeGui
{
	private:
		// Components of the computer
		Chip8* chip8;

		// Important widgets
		GtkWidget* register_values[16];
		GtkWidget* program_counter_value;
		GtkWidget* stack_pointer_value;
		GtkWidget* address_register_value;

		GtkWidget* memory_display;


		void link_widgets(GtkBuilder*);
		void link_keyboard(GtkBuilder*);

		const char* byte_to_string(unsigned char);
		const char* short_to_string(unsigned short);
		const char* byte_to_string(unsigned char, bool);
		const char* short_to_string(unsigned short, bool);

	public:
		GladeGui(Chip8*, int, char**);
		GladeGui(Chip8*, Computer*, int, char**);
		~GladeGui();

		Computer* computer;

		void build();
		void run();

		// Chip-8 Listener
		void update_register(unsigned char, unsigned char);
		void update_program_counter(unsigned short);
		void update_stack_pointer(unsigned short);
		void update_address_register(unsigned short);


		bool need_refresh();
		void fill_memory_display();

		bool running;
		GtkWidget* display;
};

#endif