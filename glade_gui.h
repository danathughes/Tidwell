#include "chip8.h"

#include <gtk/gtk.h>


#ifndef __GLADE_GUI_H__
#define __GLADE_GUI_H__

class GladeGui
{
	private:
		Chip8* chip8;

		// Is the gui running the chip


		// Important widgets
		GtkWidget* register_values[16];
		GtkWidget* program_counter_value;
		GtkWidget* stack_pointer_value;
		GtkWidget* address_register_value;


		void link_widgets(GtkBuilder*);
		const char* byte_to_string(unsigned char);
		const char* short_to_string(unsigned short);
		const char* byte_to_string(unsigned char, bool);
		const char* short_to_string(unsigned short, bool);

	public:
		GladeGui(Chip8*, int, char**);
		~GladeGui();

		void cycle_chip();
		void build();
		void run();

		void update_registers();
		bool get_pixel(unsigned char, unsigned char);

		bool need_refresh();

		bool running;
		GtkWidget* display;
};

#endif