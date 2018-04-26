#ifndef __GLADE_GUI_H__
#define __GLADE_GUI_H__

#include "computer.h"
#include "chip_listener.h"

#include <gtk/gtk.h>

class GladeGui : public ChipListener
{
	private:

		// Important widgets
		GtkWidget* register_values[16];
		GtkWidget* program_counter_value;
		GtkWidget* stack_pointer_value;
		GtkWidget* address_register_value;
		GtkWidget* memory_display;

		void link_widgets(GtkBuilder*);
		void link_keyboard(GtkBuilder*);

	public:
		GladeGui(Computer*, int, char**);
		~GladeGui();

		Computer* computer;

		void build();
		void run();

		// Chip-8 Listener
		void update_register(unsigned char, unsigned char);
		void update_program_counter(unsigned short);
		void update_stack_pointer(unsigned short);
		void update_address_register(unsigned short);

		void refresh_display();

		void fill_memory_display();

		bool running;
		GtkWidget* display;
};

#endif