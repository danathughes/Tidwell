#ifndef __CHIP_LISTENER__
#define __CHIP_LISTENER__

class ChipListener
{
	public:
		virtual void update_register(unsigned char, unsigned char) = 0;
		virtual void update_program_counter(unsigned short) = 0;
		virtual void update_stack_pointer(unsigned short) = 0;
		virtual void update_address_register(unsigned short) = 0;
		virtual void refresh_display() = 0;

		virtual void update_delay_timer(unsigned short) = 0;
		virtual void update_sound_timer(unsigned short) = 0;

		~ChipListener() {}

	protected:
		ChipListener() {}

};

#endif