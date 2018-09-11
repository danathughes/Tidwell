#ifndef __DISPLAY_H__
#define __DISPLAY_H__

class Display
{
	private:
		bool** display;

		unsigned int width, height;

	public:
		Display();
		Display(unsigned int, unsigned int);

		bool set_pixel(unsigned char, unsigned char);
		bool flip_pixel(unsigned char, unsigned char);

		bool get_pixel(unsigned char, unsigned char);
		bool write_line(unsigned char, unsigned char, unsigned char);
		void show();
		void clear();
	
		unsigned int get_width();
		unsigned int get_height();
};

#endif