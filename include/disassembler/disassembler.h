#ifndef __DISASSEMBLER_H__
#define __DISASSEMBLER_H__

#include <map>
#include <string>

// Define the commands accoring to their opcode, with variables zeroed out
#define SYS_CALL							0x0000
#define CLEAR_SCREEN						0x00E0
#define RETURN								0x00EE
#define JUMP								0x1000
#define CALL								0x2000
#define SKIP_EQUAL_REGISTER_VALUE			0x3000
#define SKIP_NOT_EQUAL_REGISTER_VALUE		0x4000
#define SKIP_EQUAL_REGISTER_REGISTER		0x5000
#define LOAD_REGISTER_VALUE					0x6000
#define ADD_REGISTER_VALUE					0x7000
#define LOAD_REGISTER_REGISTER				0x8000
#define OR 									0x8001
#define AND 								0x8002
#define XOR									0x8003
#define ADD_REGISTER_REGISTER				0x8004
#define SUBTRACT_REGISTER_REGISTER 			0x8005
#define SHIFT_RIGHT							0x8006
#define SUBTRACT_REGISTER_REGISTER_NEGATIVE	0x8007
#define SHIFT_LEFT							0x800E
#define SKIP_NOT_EQUAL_REGISTER_REGISTER	0x9000
#define LOAD_ADDRESS						0xA000
#define JUMP_OFFSET							0xB000
#define RANDOM								0xC000
#define DRAW								0xD000
#define SKIP_KEY_PRESSED					0xE09E
#define SKIP_KEY_NOT_PRESSED				0xE0A1
#define GET_DELAY_TIMER						0xF007
#define WAIT_KEY_PRESSED					0xF00A
#define SET_DELAY_TIMER						0xF015
#define SET_SOUND_TIMER						0xF018
#define ADD_ADDRESS_REGISTER				0xF01E
#define LOAD_SPRITE_ADDRESS					0xF029
#define STORE_BCD							0xF033
#define STORE_REGISTERS						0xF055
#define LOAD_REGISTERS 						0xF065

// Define the SCHIP  commands
#define SCROLL_DOWN							0x00C0
#define SCROLL_RIGHT						0x00FB
#define SCROLL_LEFT							0x00FC
#define EXIT								0x00FD
#define LORES_MODE							0x00FE
#define HIRES_MODE							0x00FF
#define LOAD_BIG_SPRITE_ADDRESS				0xF030
#define STORE_REGISTERS_TO_HP				0xF075
#define LOAD_REGISTERS_FROM_HP				0xF085

	
// Enumerate whether a piece of code is an Instruction, Data, or Unknown
enum CodeType { UNKNOWN, INSTRUCTION, DATA };

// Bytes will be converted to a structure that holds relevant information
typedef struct Code_Struct {
	CodeType type;
	unsigned short address;
	unsigned short raw_code;
	unsigned short opcode;
	const char* mnemonic;
	unsigned short address_register;
	unsigned char register_x;
	unsigned char register_y;
	unsigned char value;
} Code;


class Disassembler
{
	private:
		// The string of bytes loaded directly from the program
//		unsigned short* raw_program;

		// Structure of each code segment
		Code* _program;

		// What address is the program loaded into?  (Typically 0x200)
		unsigned short start_address;

		// Map from opcode to operation name
		std::map <unsigned short, const char*>* operation_name_map;

		// How big is the program?
		unsigned int program_size;

	public:
		Disassembler();
		~Disassembler();

		unsigned short get_opcode(unsigned short);
		void load_rom(const char*);
		void create_operation_name_map();
		void decode();
		void print();
		std::string decompile_command(Code);
		void trace();
};

#endif