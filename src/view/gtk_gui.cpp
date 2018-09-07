#include "view/gtk_gui.h"

#include <gtk/gtk.h>
#include <iostream>
#include <sstream>



static void _click_cycle(GtkWidget* widget, gpointer data)
{
//	Chip8* chip = (Chip8*) data;

//	chip->cycle();

	GtkGui* gui = (GtkGui*) data;

	gui->cycle_chip();
	gui->update_registers();
}



GtkGui::GtkGui(Chip8* chip, int argc, char** argv)
{
	// Initialize Gtk
	gtk_init(&argc, &argv);

	chip8 = chip;
}


GtkGui::~GtkGui()
{

}

void GtkGui::cycle_chip()
{
	chip8->cycle();
}


void GtkGui::update_registers()
{
	std::stringstream ss;

	// Update the register_values
	for(unsigned char i=0; i<16; i++)
	{
		ss.clear();
		ss.str(std::string());
		ss << "0x" << std::hex << (unsigned short) chip8->get_register(i);
		gtk_label_set_text(GTK_LABEL (register_values[i]), ss.str().c_str());
	}

	// Update the program counter value and stack pointer value
	ss.clear();
	ss.str(std::string());
	ss << "0x" << std::hex << chip8->get_program_counter();
	gtk_label_set_text(GTK_LABEL (program_counter_value), ss.str().c_str());

	ss.clear();
	ss.str(std::string());
	ss << "0x" << std::hex << (unsigned short) chip8->get_stack_pointer();
	gtk_label_set_text(GTK_LABEL (stack_pointer_value), ss.str().c_str());
}


void GtkGui::build()
{
	std::stringstream ss;

	/* Create the window, set the title, etc. */
	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Chip-8");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);	// Quick when closed
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	/* Set up a grid to add all the stuff to */
	GtkWidget* grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID (grid), 10);

	/* Create labels for registers, program counter, stack pointer, etc. */


	for(int i=0; i<16; i++)
	{
		ss.clear();
		ss.str(std::string());
		ss << "V" << std::hex << i;
		register_labels[i] = gtk_label_new(ss.str().c_str());
		register_values[i] = gtk_label_new("0x00");

		// Add this to the grid
		gtk_grid_attach(GTK_GRID (grid), register_labels[i], 2*(i/4), i%4, 1, 1);
		gtk_grid_attach(GTK_GRID (grid), register_values[i], 2*(i/4)+1, i%4, 1, 1);
	}

	// Program counter
	GtkWidget* program_counter_label = gtk_label_new("Program Counter: ");
	GtkWidget* stack_pointer_label = gtk_label_new("Stack Pointer: ");
	program_counter_value = gtk_label_new("0x0000");
	stack_pointer_value = gtk_label_new("0x0000");


	gtk_grid_attach(GTK_GRID (grid), program_counter_label, 9, 0, 1, 1);
	gtk_grid_attach(GTK_GRID (grid), program_counter_value, 10, 0, 1, 1);
	gtk_grid_attach(GTK_GRID (grid), stack_pointer_label, 9, 1, 1, 1);
	gtk_grid_attach(GTK_GRID (grid), stack_pointer_value, 10, 1, 1, 1);

	/* Create a button to cycle */
	GtkWidget* cycle_button = gtk_button_new_with_label("CYCLE");

	// Add this to the grid
	gtk_grid_attach(GTK_GRID (grid), cycle_button, 0, 5, 4, 1);

	g_signal_connect(cycle_button, "clicked", G_CALLBACK(_click_cycle), this);

	gtk_container_add(GTK_CONTAINER(window), grid);

	gtk_widget_show_all(window);
}


void GtkGui::run()
{
	gtk_main();
}