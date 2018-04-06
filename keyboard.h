#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

class Keyboard
{
	private:
		bool keys[0x0F];

	public:
		Keyboard();

		bool is_key_pressed(unsigned char key);
		void press_key(unsigned char key);
		void release_key(unsigned char key);
};

#endif