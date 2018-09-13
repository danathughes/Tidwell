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

void Display::resize(unsigned int _width, unsigned int _height)
{
	// Hang on to the old display
	bool** old_display = display;

	// Create a new display, set all values to 0
	display = new bool*[_width];

	for(int i=0; i<_width; i++)
	{
		display[i] = new bool[_height];
		for(int j=0; j<_height; j++)
		{
			display[i][j] = false;
		}
	}

	// Copy over whatever is in the current display, cropping if necessary
	for(int i=0; i<std::min(_width, width); i++)
	{
		for(int j=0; j<std::min(_height, height); j++)
		{
			display[i][j] = old_display[i][j];
		}
	}

	// Destroy the old display
	for(int i=0; i<width; i++)
	{
		delete old_display[i];
	}
	delete old_display;

	old_display = NULL;

	// Finally, set the new width and height
	width = _width;
	height = _height;

	// Clear the screen.  NOTE:  Is this what to do?
	clear();
}


void Display::scroll_down(unsigned char num_rows)
{
	for(int j=height-num_rows-1; j>=0; j--)
	{
		for(int i=0; i<width; i++)
		{
			display[i][j+num_rows] = display[i][j];
		}
	}

	for(int i=0; i<width; i++)
	{
		for(int j=0; j<num_rows; j++)
		{
			display[i][j] = false;
		}
	}
}


void Display::scroll_left(unsigned char num_cols)
{
	for(int i=num_cols; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			display[i-num_cols][j] = display[i][j];
		}
	}

	for(int i=width-num_cols; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			display[i][j] = false;
		}
	}
}

void Display::scroll_right(unsigned char num_cols)
{
	for(int i=width-num_cols-1; i>=0; i--)
	{
		for(int j=0; j<height; j++)
		{
			display[i+num_cols][j] = display[i][j];
		}
	}

	for(int i=0; i<num_cols; i++)
	{
		for(int j=0; j<height; j++)
		{
			display[i][j] = false;
		}
	}
}