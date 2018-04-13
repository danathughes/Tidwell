#include "display.h"

#include <iostream>

Display::Display()
{
	for(int i=0; i<64; i++)
	{
		for(int j=0; j<32; j++)
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


bool Display::get_pixel(unsigned char x, unsigned char y)
{
	return display[x][y];
}


bool Display::write_line(unsigned char x, unsigned char y, unsigned char value)
{
	unsigned char _y = y;
	unsigned char _x;
	bool collision = false;

	// Wrap y?
	if(_y >= 32)
	{
		_y -= 32;
	}

	// Assign each bit in the line, 
	for(int i=0; i<8; i++)
	{
		bool is_high = ( (value >> (7-i)) & 0x01) == 0x01;

		_x = x + i;
		if(_x > 64)
		{
			_x -= 64;
		}

		if(is_high)
		{
			collision = collision || set_pixel(_x, _y);
		} 
	}

	return collision;
}


void Display::show()
{
	for(int y=0; y<32; y++)
	{
		for(int x=0; x<64; x++)
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