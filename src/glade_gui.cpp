#include "glade_gui.h"

#include <gtk/gtk.h>
#include <iostream>
#include <sstream>
#include <string>

extern "C" {

/* Callback functions */
void _click_cycle(GtkWidget* widget, gpointer data)
{
	// Extract the GUI from the user data
	GladeGui* gui = (GladeGui*) data;


	// Cycle the chip
	gui->computer->cycle();

	// Update registers, display, etc.
	gui->update_registers();

	// Indicate that the display widget needs updating
	gtk_widget_queue_draw(gui->display);
}


void _draw_screen(GtkWidget* widget, cairo_t* cr, gpointer data)
{
	guint width, height;
	GdkRGBA fg_color, bg_color;
	GtkStyleContext* context;

	// Extract the GUI from the user data
	GladeGui* gui = (GladeGui*) data;

	context = gtk_widget_get_style_context(widget);

	width = gtk_widget_get_allocated_width(widget);
	height = gtk_widget_get_allocated_height(widget);

	gtk_render_background(context, cr, 0, 0, width, height);

	// Grab pixels from the gui display, and draw accordingly
	for(unsigned char i=0; i<64; i++)
	{
		for(unsigned char j=0; j<32; j++)
		{
			if(gui->computer->get_pixel(i,j))
			{
				int _x = 5 * (int) i;
				int _y = 5 * (int) j;

				cairo_rectangle(cr, _x, _y, 5, 5);

				gtk_style_context_get_color(context, gtk_style_context_get_state(context), &fg_color);
				gdk_cairo_set_source_rgba(cr, &fg_color);

				cairo_fill(cr);
			}
		}
	}	
}


void on_window_main_destroy(GtkWidget* widget, gpointer data)
{
	GladeGui* gui = (GladeGui*) data;

	gui->running = false;

	gtk_main_quit();
}


gboolean _run(gpointer data)
{
	GladeGui* gui = (GladeGui*) data;

	if(gui->running)
	{
		// Cycle the chip
		gui->computer->cycle();

		// Update registers, display, etc.
		gui->update_registers();

		// Indicate that the display widget needs updating
		if(gui->need_refresh())
		{
			gtk_widget_queue_draw(gui->display);
		}
	}

	return true;
}


void on_run_button_toggled(GtkWidget* widget, gpointer data)
{
	GladeGui* gui = (GladeGui*) data;

	gui->running = !gui->running;

	if(gui->running)
	{
		std::cout << "Running..." << std::endl;
	}
	else
	{
		std::cout << "Stopped" << std::endl;
	}
}


void on_button_pressed(GtkWidget* widget, GdkEventExpose* event, gpointer data)
{
	GladeGui* gui = (GladeGui*) data;

	unsigned char button_num = (unsigned char) std::stoi(gtk_widget_get_name(widget), NULL, 16);

	gui->computer->press_key(button_num);
}


void on_button_released(GtkWidget* widget, GdkEventExpose* event, gpointer data)
{
	GladeGui* gui = (GladeGui*) data;

	unsigned char button_num = (unsigned char) std::stoi(gtk_widget_get_name(widget), NULL, 16);

	gui->computer->release_key(button_num);
}


GladeGui::GladeGui(Chip8* chip, int argc, char** argv)
{
	// Initialize Gtk
	gtk_init(&argc, &argv);

	chip8 = chip;

	running = false;
}


GladeGui::GladeGui(Chip8* chip, Computer* _computer, int argc, char** argv)
{
	// Initialize Gtk
	gtk_init(&argc, &argv);

	chip8 = chip;
	computer = _computer;

	running = false;
}


GladeGui::~GladeGui()
{

}

bool GladeGui::need_refresh()
{
	bool refresh = chip8->refresh;
	chip8->refresh = false;

	return refresh;
}
/*
bool GladeGui::get_pixel(unsigned char x, unsigned char y)
{
	return computer->get_pixel(x,y);
}

void GladeGui::press_key(unsigned char key_num)
{
	computer->press_key(key_num);
}

void GladeGui::release_key(unsigned char key_num)
{
	computer->release_key(key_num);
}


void GladeGui::cycle_chip()
{
	computer->cycle();
}
*/

const char* GladeGui::byte_to_string(unsigned char value)
{
	return byte_to_string(value, true);
}


const char* GladeGui::byte_to_string(unsigned char value, bool prepend_0x)
{
	std::stringstream ss;

	// Clear the string stream
	ss.clear();
	ss.str(std::string());

	// Prepend with 0x, if desired
	if(prepend_0x)		ss << "0x";

	// Does there need to be a zero padded to the string?
	if(value < 0x0F)	ss << "0";

	// Convert the value to hex and append to the string
	ss << std::hex << (unsigned short) value;

	// Return as a char*
	return ss.str().c_str();
}


const char* GladeGui::short_to_string(unsigned short value)
{
	return short_to_string(value, true);
}


const char* GladeGui::short_to_string(unsigned short value, bool prepend_0x)
{
	std::stringstream ss;

	// Clear the string stream
	ss.clear();
	ss.str(std::string());

	// Prepend with 0x, if desired
	if(prepend_0x)		ss << "0x";

	// Does there need to be a zero padded to the string?
	if(value < 0x0FFF)	ss << "0";
	if(value < 0x00FF)	ss << "0";
	if(value < 0x000F)	ss << "0";
	

	// Convert the value to hex and append to the string
	ss << std::hex << value;

	// Return as a char*
	return ss.str().c_str();
}


void GladeGui::update_registers()
{
	// Update the register_values
	for(unsigned char i=0; i<16; i++)
	{
		gtk_label_set_text(GTK_LABEL (register_values[i]), byte_to_string(chip8->get_register(i)));
	}

	// Update the program counter value and stack pointer value
	gtk_label_set_text(GTK_LABEL (program_counter_value), short_to_string(chip8->get_program_counter()));
	gtk_label_set_text(GTK_LABEL (stack_pointer_value), short_to_string(chip8->get_stack_pointer()));
	gtk_label_set_text(GTK_LABEL (address_register_value), short_to_string(chip8->get_address()));	
}


void GladeGui::link_widgets(GtkBuilder* builder)
{
	// Populate the register values array
	register_values[0] = GTK_WIDGET(gtk_builder_get_object(builder, "label_v0_value"));
	register_values[1] = GTK_WIDGET(gtk_builder_get_object(builder, "label_v1_value"));
	register_values[2] = GTK_WIDGET(gtk_builder_get_object(builder, "label_v2_value"));
	register_values[3] = GTK_WIDGET(gtk_builder_get_object(builder, "label_v3_value"));
	register_values[4] = GTK_WIDGET(gtk_builder_get_object(builder, "label_v4_value"));
	register_values[5] = GTK_WIDGET(gtk_builder_get_object(builder, "label_v5_value"));
	register_values[6] = GTK_WIDGET(gtk_builder_get_object(builder, "label_v6_value"));
	register_values[7] = GTK_WIDGET(gtk_builder_get_object(builder, "label_v7_value"));
	register_values[8] = GTK_WIDGET(gtk_builder_get_object(builder, "label_v8_value"));
	register_values[9] = GTK_WIDGET(gtk_builder_get_object(builder, "label_v9_value"));
	register_values[10] = GTK_WIDGET(gtk_builder_get_object(builder, "label_va_value"));
	register_values[11] = GTK_WIDGET(gtk_builder_get_object(builder, "label_vb_value"));
	register_values[12] = GTK_WIDGET(gtk_builder_get_object(builder, "label_vc_value"));
	register_values[13] = GTK_WIDGET(gtk_builder_get_object(builder, "label_vd_value"));
	register_values[14] = GTK_WIDGET(gtk_builder_get_object(builder, "label_ve_value"));
	register_values[15] = GTK_WIDGET(gtk_builder_get_object(builder, "label_vf_value"));

	program_counter_value = GTK_WIDGET(gtk_builder_get_object(builder, "label_program_counter_value"));
	stack_pointer_value = GTK_WIDGET(gtk_builder_get_object(builder, "label_stack_pointer_value"));
	address_register_value = GTK_WIDGET(gtk_builder_get_object(builder, "label_address_register_value"));
}


void GladeGui::link_keyboard(GtkBuilder* builder)
{
	std::stringstream ss;
	GtkWidget* key;

	for(int i=0; i<16; i++)
	{
		ss.clear();
		ss.str(std::string());
		ss << "button_" << std::hex << i;

		key = GTK_WIDGET(gtk_builder_get_object(builder, ss.str().c_str()));

		ss.clear();
		ss.str(std::string());
		ss << std::hex << i;

		gtk_widget_set_name(key, ss.str().c_str());
	}
}


void GladeGui::build()
{
	GtkBuilder* builder;
	GtkWidget*  window;

	// Load the glade file containing the gui
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "chip8_gui.glade", NULL);

	// Build the window
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

	// Collect references to needed widgets--register value text areas, display, etc.
	link_widgets(builder);
	link_keyboard(builder);
	display = GTK_WIDGET(gtk_builder_get_object(builder, "display"));
	memory_display = GTK_WIDGET(gtk_builder_get_object(builder, "memory_display"));

	// Connect the signals.  All signals are going to pass the instance of this object as
	// user data, so that callback functions can access appropriate information.
	gtk_builder_connect_signals(builder, this);


	g_object_unref(builder);

	gtk_widget_show(window);

	update_registers();

	fill_memory_display();
}


void GladeGui::run()
{
	// Start the timeout to call the run function
	gint run_ref = g_timeout_add(16, _run, this);

	gtk_main();
}


void GladeGui::fill_memory_display()
{
	gtk_label_set_text(GTK_LABEL (memory_display), computer->get_memory_string());
}

}	// extern "C"