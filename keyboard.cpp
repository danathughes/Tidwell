#include "keyboard.h"

Keyboard::Keyboard()
{
	for(int i=0; i<0x0F; i++)
	{
		keys[i] = false;
	}
}

bool Keyboard::is_key_pressed(unsigned char key)
{
	return keys[key];
}

void Keyboard::press_key(unsigned char key)
{
	keys[key] = true;
}

void Keyboard::release_key(unsigned char key)
{
	keys[key] = false;
}