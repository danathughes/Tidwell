#ifndef __DISPLAY_H__
#define __DISPLAY_H__

class Display
{
	private:
		bool display[64][32];

	public:
		Display();

		bool set_pixel(unsigned char, unsigned char);
		bool get_pixel(unsigned char, unsigned char);
		bool write_line(unsigned char, unsigned char, unsigned char);
		void show();
		void clear();
	
};

#endif