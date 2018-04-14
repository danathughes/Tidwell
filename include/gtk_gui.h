#include "chip8.h"

#include <gtk/gtk.h>


#ifndef __GTK_GUI_H__
#define __GTK_GUI_H__

class GtkGui
{
	private:
		Chip8* chip8;

		// Important widgets
		GtkWidget* register_labels[16];
		GtkWidget* register_values[16];

		GtkWidget* program_counter_value;
		GtkWidget* stack_pointer_value;

	public:
		GtkGui(Chip8*, int, char**);
		~GtkGui();

		void cycle_chip();
		void build();
		void run();

		void update_registers();
};

#endif