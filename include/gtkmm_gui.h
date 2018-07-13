#ifndef __GTKMM_GUI_H__
#define __GTKMM_GUI_H__

#include "computer.h"
#include "chip_listener.h"

#include <gtk/gtk.h>
#include <gtkmm.h>

#include <map>

class GtkmmGui : public ChipListener
{
	private:

		// GTKMM Stuff
		Glib::RefPtr<Gtk::Application> gtkApp;
		Gtk::Window*  window;
		Gtk::FileChooserDialog*	load_rom_dialog;

		// Important widgets
		Gtk::Label* register_values[16];
		Gtk::Label* program_counter_value;
		Gtk::Label* stack_pointer_value;
		Gtk::Label* address_register_value;
		Gtk::Label* memory_display;

		Gtk::Label* sound_timer_value;
		Gtk::Label* delay_timer_value;
		Gtk::Label* sound_status;

		Gtk::ToggleButton* run_button;

		Gtk::DrawingArea* screen_area;

		std::map <Gtk::Label*, std::string> stale_values;

		void setup_labels(Glib::RefPtr<Gtk::Builder>);
		void setup_keyboard(Glib::RefPtr<Gtk::Builder>);
		void setup_menu(Glib::RefPtr<Gtk::Builder>);

		// Callback function
		bool on_key_press(GdkEventButton*, int);
		bool on_key_release(GdkEventButton*, int);

		void on_click_cycle();
		void on_run_toggled();

		void on_load_rom_activate();

		bool update_timeout();

		bool draw_screen(Cairo::RefPtr<Cairo::Context>);

	public:
		GtkmmGui(Computer*, int, char**);
		~GtkmmGui();

		Computer* computer;

		void build();
		void run();

		// Chip-8 Listener
		void update_register(unsigned char, unsigned char);
		void update_program_counter(unsigned short);
		void update_stack_pointer(unsigned short);
		void update_address_register(unsigned short);

		void update_delay_timer(unsigned short);
		void update_sound_timer(unsigned short);

		void refresh_display();

		void fill_memory_display();

		bool running;
};




#endif