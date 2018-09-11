#include "core/display.h"

#include <iostream>

Display::Display()
{
	width = 64;
	height = 32;

	display = new bool*[width];

	for(int i=0; i<width; i++)
	{
		display[i] = new bool[height];

		for(int j=0; j<height; j++)
		{
			display[i][j] = false;
		}
	}
	
}


Display::Display(unsigned int _width, unsigned int _height)
{
	width = _width;
	height = _height;

	display = new bool*[width];

	for(int i=0; i<width; i++)
	{
		display[i] = new bool[height];

		for(int j=0; j<height; j++)
		{
			display[i][j] = false;
		}
	}
}

/*******************
* set_pixel(unsigned char x, unsigned char y)
* 
* Xor the pixel at point (x,y).  If this sets the pixel to false,
* then return true, otherwise, return false
*******************/
bool Display::set_pixel(unsigned char x, unsigned char y)
{
	if(display[x][y])
	{
		display[x][y] = false;
		return true;
	}
	else
	{
		display[x][y] = true;
		return false;
	}
}


bool Display::flip_pixel(unsigned char x, unsigned char y)
{
	display[x][y] = !display[x][y];

	return !display[x][y];
}


bool Display::get_pixel(unsigned char x, unsigned char y)
{
	return display[x][y];
}


bool Display::write_line(unsigned char x, unsigned char y, unsigned char value)
{
	unsigned char _y = y;
	unsigned char _x;
	bool collision = false;

	// _y may be greater than the display size, wrap around in this case
	_y = _y % height;

	// Assign each bit in the line, 
	for(int i=0; i<8; i++)
	{
		bool is_high = ( (value >> (7-i)) & 0x01) == 0x01;

		// Wrap _x if it is past the right side of the screen
		_x = x + i;
		_x = _x % width;

		if(display[_x][_y] == is_high)
		{
			if(is_high)
			{
				collision = true;
			}
			display[_x][_y] = false;
		}
		else
		{
			display[_x][_y] = true;
		}

	}
	
	return collision;
}


void Display::show()
{
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			if(display[x][y])
			{
				std::cout << "#"; 
			}
			else
			{
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
}


void Display::clear()
{
	for(int x=0; x<width; x++)
	{
		for(int y=0; y<height; y++)
		{
			display[x][y] = false;
		}
	}
}


unsigned int Display::get_width()
{
	return width;
}


unsigned int Display::get_height()
{
	return height;
}