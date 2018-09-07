#include "view/gtkmm_gui.h"
//#include "glade_gui.h"
#include <gtkmm.h>
#include <gtk/gtk.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <map>

extern "C" {

/* Callback functions */



//void xon_window_main_destroy(GtkWidget* widget, gpointer data)
//{
//	GtkmmGui* gui = (GtkmmGui*) data;

//	gui->running = false;

//	gtk_main_quit();
//}


GtkmmGui::GtkmmGui(Computer* _computer, int argc, char** argv)
{
	// Initialize Gtk
	gtkApp = Gtk::Application::create(argc, argv, "com.danathughes.chip8");

	computer = _computer;

	running = false;
}


GtkmmGui::~GtkmmGui()
{

}


std::string _byte_to_string(unsigned char value, bool prepend_0x)
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
	return ss.str();
}


std::string _short_to_string(unsigned short value, bool prepend_0x)
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
	return ss.str();
}



bool GtkmmGui::on_key_press(GdkEventButton* event, int button_num)
{
	computer->press_key((unsigned char) button_num);

	// Let other handlers do their thing
	return false;
}


bool GtkmmGui::on_key_release(GdkEventButton* event, int button_num)
{
	computer->release_key((unsigned char) button_num);

	// Let other handlers do their thing
	return false;
}


void GtkmmGui::on_click_cycle()
{
	computer->cycle();
}


void GtkmmGui::on_reset()
{
	computer->soft_reset();
}


void GtkmmGui::on_run_toggled()
{
	running = !running;
	if(running)		computer->run();
	if(!running)	computer->pause();
}


void GtkmmGui::on_load_rom_activate()
{
	// Open up the Load Rom Dialog
	int result = load_rom_dialog->run();

	switch(result)
	{
		case(1):
		case(Gtk::RESPONSE_OK):
		{
			// A file was selected. Try to load it.
			std::string filename = load_rom_dialog->get_filename();
			std::cout << "Loading " << filename << "..." <<  std::endl;

			// Stop any executing programs prior to running
			if(running)
			{
				run_button->set_active(false);
				running = false;
				computer->pause();
			}

			// Load the program, reset the computer, and refresh the memory display
			computer->load(filename.c_str());
			computer->soft_reset();
			update_memory();

			break;
		}
		case(2):
		case(Gtk::RESPONSE_CANCEL):
		{
			// User clicked cancel -- nothing to load
			break;
		}
		default:
		{
			// Something went wrong...
			break;
		}
	}

	// Hide the load_rom_dialog...
	load_rom_dialog->close();
}


bool GtkmmGui::update_timeout()
{
	// Update the stale data
	std::map<Gtk::Label*, std::string>::iterator iter;

	for(iter = stale_values.begin(); iter != stale_values.end(); ++iter)
	{
		Gtk::Label* widget = iter->first;
		std::string value = iter->second;
		widget->set_text(value);
	}

	return true;
}


bool GtkmmGui::draw_screen(Cairo::RefPtr<Cairo::Context> cr)
{
	// Scale the drawing area so that a 1x1 rectange corresponds to a single pixel
	Gtk::Allocation allocation = screen_area->get_allocation();
	const int screen_width = allocation.get_width();
	const int screen_height = allocation.get_height();

	double width_scale = (double) screen_width / 64;
	double height_scale = (double) screen_height / 32;


	cr->scale(width_scale, height_scale);			// NOTE:  This is hardcoded!  Grab it from the computer display!



	// Draw all the rectangles in the display
	for(unsigned char i=0; i<64; i++)
	{
		for(unsigned char j=0; j<32; j++)
		{
			// Draw the rectange
			cr->rectangle((double) i, (double) j, 1.0, 1.0);
			// Figure out if the color should be foreground or background
			if(computer->get_pixel(i,j))
			{
				cr->set_source_rgb(0.0,0.0,0.0);
			}
			else
			{
				cr->set_source_rgb(1.0,1.0,1.0);
			}
			// Fill the rectangle
			cr->fill();
		}
	}
}



void GtkmmGui::setup_menu(Glib::RefPtr<Gtk::Builder> builder)
{
	Gtk::MenuItem* load_rom_menu_item = nullptr;

	builder->get_widget("load_rom_menu_item", load_rom_menu_item);
	load_rom_menu_item->signal_activate().connect(sigc::mem_fun(*this, &GtkmmGui::on_load_rom_activate));
}


void GtkmmGui::setup_labels(Glib::RefPtr<Gtk::Builder> builder)
{
	builder->get_widget("label_v0_value", register_values[0]);
	builder->get_widget("label_v1_value", register_values[1]);
	builder->get_widget("label_v2_value", register_values[2]);
	builder->get_widget("label_v3_value", register_values[3]);
	builder->get_widget("label_v4_value", register_values[4]);
	builder->get_widget("label_v5_value", register_values[5]);
	builder->get_widget("label_v6_value", register_values[6]);
	builder->get_widget("label_v7_value", register_values[7]);
	builder->get_widget("label_v8_value", register_values[8]);
	builder->get_widget("label_v9_value", register_values[9]);
	builder->get_widget("label_va_value", register_values[10]);
	builder->get_widget("label_vb_value", register_values[11]);
	builder->get_widget("label_vc_value", register_values[12]);
	builder->get_widget("label_vd_value", register_values[13]);
	builder->get_widget("label_ve_value", register_values[14]);
	builder->get_widget("label_vf_value", register_values[15]);
	
	builder->get_widget("label_program_counter_value", program_counter_value);
	builder->get_widget("label_stack_pointer_value", stack_pointer_value);
	builder->get_widget("label_address_register_value", address_register_value);

	builder->get_widget("label_sound_timer_value", sound_timer_value);
	builder->get_widget("label_delay_timer_value", delay_timer_value);
	builder->get_widget("label_sound_value", sound_status);
}


void GtkmmGui::setup_keyboard(Glib::RefPtr<Gtk::Builder> builder)
{
	std::stringstream ss;
	Gtk::Button* key;

	for(int i=0; i<16; i++)
	{
		ss.clear();
		ss.str(std::string());
		ss << "button_" << std::hex << i;

		builder->get_widget(ss.str().c_str(), key);

		ss.clear();
		ss.str(std::string());
		ss << std::hex << i;

		// Set the key to accept PRESS and RELEASE, and link to the callback methods
		// The 'false' parameter ensures that the signal is propagated by any previous handlers, and reaches
		// the desired callback.
		key->signal_button_press_event().connect(sigc::bind<int>(sigc::mem_fun(*this, &GtkmmGui::on_key_press), i), false);
		key->signal_button_release_event().connect(sigc::bind<int>(sigc::mem_fun(*this, &GtkmmGui::on_key_release), i), false);
	}
}


void GtkmmGui::build()
{
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
	Gtk::Button* cycle_button = nullptr;
	Gtk::Button* reset_button = nullptr;

	// Load the glade file containing the gui
	try
	{
		builder->add_from_file("chip8_gui.glade");
	}
	catch(const Glib::FileError& error)
	{
		std::cerr << "FileError: " << error.what() << std::endl;
		return;
	}
	catch(const Glib::MarkupError& error)
	{
		std::cerr << "MarkupError: " << error.what() << std::endl;
		return;
	}
	catch(const Gtk::BuilderError& error)
	{
		std::cerr << "BuilderError: " << error.what() << std::endl;
		return;
	}

	// Build the window
	builder->get_widget("window_main", window);

	// Collect references to needed widgets--register value text areas, display, etc.
	setup_labels(builder);
	setup_keyboard(builder);
	setup_menu(builder);

	// Set up the display.  Connect to a callback to draw when the screen needs updating	
	builder->get_widget("display", screen_area);
	screen_area->signal_draw().connect(sigc::mem_fun(*this, &GtkmmGui::draw_screen), false);

	builder->get_widget("memory_display", memory_display);
	builder->get_widget("stack_display", stack_display);

	memory_display->set_justify(Gtk::JUSTIFY_FILL);
	stack_display->set_justify(Gtk::JUSTIFY_FILL);

	// Setup the signal handlers for the cycle and run buttons
	builder->get_widget("step_button", cycle_button);
	builder->get_widget("run_button", run_button);
	builder->get_widget("reset_button", reset_button);

	cycle_button->signal_clicked().connect(sigc::mem_fun(*this, &GtkmmGui::on_click_cycle));
	run_button->signal_clicked().connect(sigc::mem_fun(*this, &GtkmmGui::on_run_toggled));
	reset_button->signal_clicked().connect(sigc::mem_fun(*this, &GtkmmGui::on_reset));

	// References to the various dialog boxes (loading a rom, about, etc.)
	builder->get_widget("load_rom_dialog", load_rom_dialog);

	update_memory();
	stale_values.clear();


	// Start a timer for updaing values in the GUI
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &GtkmmGui::update_timeout), 100);
}


void GtkmmGui::run()
{
	gtkApp->run(*window);
}



/* Listener Functions */

void GtkmmGui::update_register(unsigned char register_number, unsigned char value)
{
//	register_values[register_number]->set_text(_byte_to_string(value, true));
	stale_values[register_values[register_number]] = _byte_to_string(value, true);
}

void GtkmmGui::update_program_counter(unsigned short value)
{
//	program_counter_value->set_text(_short_to_string(value, true));
	stale_values[program_counter_value] = _short_to_string(value, true);
}

void GtkmmGui::update_stack_pointer(unsigned short value)
{
//	stack_pointer_value->set_text(_short_to_string(value, true));
	stale_values[stack_pointer_value] = _short_to_string(value, true);
}

void GtkmmGui::update_address_register(unsigned short value)
{
//	address_register_value->set_text(_short_to_string(value, true));
	stale_values[address_register_value] = _short_to_string(value, true);
}


void GtkmmGui::update_delay_timer(unsigned short value)
{
//	delay_timer_value->set_text(_short_to_string(value, true));
	stale_values[delay_timer_value] = _short_to_string(value, true);
}


void GtkmmGui::update_sound_timer(unsigned short value)
{
//	sound_timer_value->set_text(_short_to_string(value, true));
	stale_values[sound_timer_value] = _short_to_string(value, true);

	if(value > 0)
	{
//		sound_status->set_text("OFF");
		stale_values[sound_status] = std::string("OFF");
	}
	else
	{
//		sound_status->set_text("ON");
		stale_values[sound_status] = std::string("ON");
	}
}


void GtkmmGui::update_memory()
{
//	memory_display->set_text(computer->get_memory_string());
	stale_values[memory_display] = computer->get_memory_string();
}


void GtkmmGui::update_stack(unsigned short* stack, unsigned char num_values, unsigned char stack_size)
{
	std::stringstream ss;

	// Create a string for the stack
	ss.clear();
	ss.str(std::string());

	// Go through the stack writing the stack position, and address if present
	for(unsigned char i=0; i < stack_size; i++)
	{
		ss << _byte_to_string(i, true) << ":\t";

		// Either fill in an address, if present, or spaces, in order to ensure consistant line width
		if(i < num_values)
		{
			ss << _short_to_string(stack[i], true);
		}
		else
		{
			ss << "      ";
		}

		if(i < stack_size-1)
		{
			ss << "\n";
		}
	}

	stale_values[stack_display] = ss.str();
}


void GtkmmGui::refresh_display()
{
	screen_area->queue_draw();
}






}	// extern "C"