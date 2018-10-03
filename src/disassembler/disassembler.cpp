#include "disassembler.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <set>
#include <queue>

Disassembler::Disassembler()
{

}

Disassembler::~Disassembler()
{

}


void Disassembler::create_operation_name_map()
{
	// Create the map
	operation_name_map = new std::map<unsigned short, const char*>();

	operation_name_map->insert(std::pair<unsigned short, const char*>(SYS_CALL, "SYS "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(CLEAR_SCREEN, "CLS "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(RETURN, "RET "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(JUMP, "JMP "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(CALL, "CALL"));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SKIP_EQUAL_REGISTER_VALUE, "SE  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SKIP_NOT_EQUAL_REGISTER_VALUE, "SNE "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SKIP_EQUAL_REGISTER_REGISTER, "SE  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(LOAD_REGISTER_VALUE, "LD  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(ADD_REGISTER_VALUE, "ADD "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(LOAD_REGISTER_REGISTER, "LD  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(OR, "OR  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(AND, "AND "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(XOR, "XOR "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(ADD_REGISTER_REGISTER, "ADD "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SUBTRACT_REGISTER_REGISTER, "SUB "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SHIFT_RIGHT, "SHR "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SUBTRACT_REGISTER_REGISTER_NEGATIVE, "SUBN"));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SHIFT_LEFT, "SHL "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SKIP_NOT_EQUAL_REGISTER_REGISTER, "SNE "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(LOAD_ADDRESS, "LD  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(JUMP_OFFSET, "JMP "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(RANDOM, "RND "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(DRAW, "DRW "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SKIP_KEY_PRESSED, "SKP "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SKIP_KEY_NOT_PRESSED, "SKNP"));
	operation_name_map->insert(std::pair<unsigned short, const char*>(GET_DELAY_TIMER, "LD  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(WAIT_KEY_PRESSED, "LD  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SET_DELAY_TIMER, "LD  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SET_SOUND_TIMER, "LD  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(ADD_ADDRESS_REGISTER, "ADD "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(LOAD_SPRITE_ADDRESS, "LD  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(STORE_BCD, "LD  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(STORE_REGISTERS, "LD  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(LOAD_REGISTERS, "LD  "));

	// SChip-8 operations
	operation_name_map->insert(std::pair<unsigned short, const char*>(SCROLL_DOWN, "SDC "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SCROLL_RIGHT, "SCR "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(SCROLL_LEFT, "SCL "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(EXIT, "EXIT"));
	operation_name_map->insert(std::pair<unsigned short, const char*>(LORES_MODE, "LOW "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(HIRES_MODE, "HIGH"));
	operation_name_map->insert(std::pair<unsigned short, const char*>(LOAD_BIG_SPRITE_ADDRESS, "LD  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(STORE_REGISTERS_TO_HP, "LD  "));
	operation_name_map->insert(std::pair<unsigned short, const char*>(LOAD_REGISTERS_FROM_HP, "LD  "));
}


void Disassembler::load_rom(const char* filename)
{
	long begin, end;

	// Open the file
	std::ifstream romfile;
	romfile.open(filename, std::ios::in | std::ios::binary);

	// If the file didn't open, return
	if(!romfile.is_open())
	{
		std::cout << "ERROR: File " << filename << " did not open!" << std::endl;
		return;
	}

	// Load the entirity of the file into the raw program

	// Figure out how long the file is
	begin = romfile.tellg();
	romfile.seekg(0, std::ios::end);
	end = romfile.tellg();
	int file_size = (int) (end - begin);
	program_size = file_size / 2;

	// Create an array to store the bytes and disassembled data
	_program = new Code[program_size];

	char* memblock = new char[file_size];

	// Load the bytes into the raw program
	romfile.seekg(0, std::ios::beg);
	romfile.read(memblock, file_size);
	romfile.close();

	start_address = 0x200;

	// Convert the bytes to shorts
	for(int i=0; i<file_size; i+=2)
	{
		_program[i/2].address = start_address + i;
		_program[i/2].raw_code = ((memblock[i] << 8) & 0xFF00) + (memblock[i+1] & 0x00FF); 
	}

	delete [] memblock;
}


void Disassembler::print()
{
	// Just dump it out
	for(int i=0; i<program_size; i++)
	{
		switch(_program[i].type)
		{
			case UNKNOWN:
				std::cout << "UKNOWN     \t";
				break;
			case INSTRUCTION:
				std::cout << "INSTRUCTION\t";
				break;
			case DATA:
				std::cout << "DATA       \t";
				break;
		}
		std::cout << std::hex << "0x" << std::setw(4) << std::setfill('0') << _program[i].address << ":\t\t";
		std::cout << decompile_command(_program[i]) << std::endl;
	}
}


void Disassembler::decode()
{
	for(int i=0; i<program_size; i++)
	{
		_program[i].opcode = get_opcode(_program[i].raw_code);

		std::map<unsigned short, const char*>::iterator it = operation_name_map->find(_program[i].opcode);
		if(it != operation_name_map->end())
		{
			_program[i].mnemonic = it->second;
		}
		else
		{
			_program[i].mnemonic = "UNK ";
		}
		_program[i].address_register = _program[i].raw_code & 0x0FFF;
		_program[i].register_x = (_program[i].raw_code & 0x0F00) >> 8;
		_program[i].register_y = (_program[i].raw_code & 0x00F0) >> 4;
		_program[i].value = (_program[i].raw_code & 0x00FF);

		if(_program[i].opcode == DRAW)
		{
			_program[i].value = _program[i].value & 0x000F;
		}
	}
}


void Disassembler::trace()
{
	// Create a queue of unexplored addresses / commands, and insert the address at 
	// the start of the program memory
	std::queue<unsigned short> frontier;
	std::set<unsigned short> explored;
	frontier.push(start_address);

	// While there's unexplored addresses in the queue, pop them out, collect the command,
	// and try to determine control flow
	while(!frontier.empty())
	{
		unsigned short next_addr;

		unsigned short addr = frontier.front();
		unsigned short end_addr = start_address + 2*program_size;
		frontier.pop();

		// Get the program code at this address
		Code code = _program[(addr - start_address) / 2];

		// Indicate that it's an instruction
		_program[(addr - start_address) / 2].type = INSTRUCTION;

		// Where can the program counter possibly go to?
		switch(code.opcode)
		{
			// Jump operations
			case JUMP:
				// What's the location to jump to?
				next_addr = code.raw_code & 0x0FFF;

				// Has this address been explored?
				if(explored.find(next_addr) == explored.end() and next_addr < end_addr)
				{
					frontier.push(next_addr);
				}
				break;

			// Offset jump case
			case JUMP_OFFSET:
				// This one's tricky....
				break;

			case SYS_CALL:
			case CALL:
				// What's the location to call?
				next_addr = code.raw_code & 0x0FFF;

				// Has this address been explored?
				if(explored.find(next_addr) == explored.end() and next_addr < end_addr)
				{
					frontier.push(next_addr);
				}

				// Once the subroutine is done, the call returns an PC is incremented by 2
				// Has the next address been explored?
				if(explored.find(addr + 0x0002) == explored.end() and next_addr < end_addr)
				{
					frontier.push(addr + 0x0002);
				}

				break;

			case SKIP_EQUAL_REGISTER_VALUE:
			case SKIP_NOT_EQUAL_REGISTER_VALUE:
			case SKIP_EQUAL_REGISTER_REGISTER:
			case SKIP_NOT_EQUAL_REGISTER_REGISTER:
			case SKIP_KEY_PRESSED:
			case SKIP_KEY_NOT_PRESSED:
				// A skip operation can either explore the next command, or the one
				// after that

				if(explored.find(addr + 0x0002) == explored.end() and next_addr < end_addr)
				{
					frontier.push(addr + 0x0002);
				}

				if(explored.find(addr + 0x0004) == explored.end() and next_addr < end_addr)
				{
					frontier.push(addr + 0x0004);
				}

				break;

			case EXIT:
				// Exiting the interpreter results in no more exploration
				break;

			default:
				// Assume that all other operations simply increment the PC by 2
				if(explored.find(addr + 0x0002) == explored.end() and next_addr < end_addr)
				{
					frontier.push(addr + 0x0002);
				}
		}

		// Indicate that this address has been explored
		explored.insert(addr);
	}

	// Now go through the whole program, and anything still labeled as UNKNOWN is Data
	for(int i=0; i<program_size; i++)
	{
		if(_program[i].type == UNKNOWN)
		{
			_program[i].type = DATA;
		}
	}
}


std::string Disassembler::decompile_command(Code code)
{
	// A stringstream used to decompile the command
	std::stringstream ss;

	// Is this data?
	if(code.type == DATA)
	{
		// Just return the raw data
		ss << "\t0x" << std::setfill('0') << std::setw(4) << std::hex << code.raw_code;
		return ss.str();
	}

	// Otherwise, need to decode...
	ss << code.mnemonic;

	// Which kind of code is it?
	switch(code.opcode)
	{
		case SYS_CALL:
		case JUMP:
		case CALL:
			ss << "\t0x" << std::setfill('0') << std::setw(3) << std::hex << code.address_register; 
			break;

		case LOAD_ADDRESS:
			ss << "\tI\t0x" << std::setfill('0') << std::setw(3) << std::hex << code.address_register; 
			break;

		case CLEAR_SCREEN:
		case RETURN:
			break;

		case SKIP_EQUAL_REGISTER_VALUE:
		case SKIP_NOT_EQUAL_REGISTER_VALUE:
		case LOAD_REGISTER_VALUE:
		case ADD_REGISTER_VALUE:
		case RANDOM:
			ss << "\tV" << std::hex << (unsigned short) code.register_x;
			ss << "\t0x" << std::setfill('0') << std::setw(2) << std::hex << (unsigned short) code.value;
			break;

		case SKIP_EQUAL_REGISTER_REGISTER:
		case LOAD_REGISTER_REGISTER:
		case OR:
		case AND:
		case XOR:
		case ADD_REGISTER_REGISTER:
		case SUBTRACT_REGISTER_REGISTER:
		case SUBTRACT_REGISTER_REGISTER_NEGATIVE:
		case SKIP_NOT_EQUAL_REGISTER_REGISTER:
			ss << "\tV" << std::hex << (unsigned short) code.register_x << "\tV" << std::hex << (unsigned short) code.register_y;
			break;

		case SHIFT_RIGHT:
		case SHIFT_LEFT:
		case SKIP_KEY_PRESSED:
		case SKIP_KEY_NOT_PRESSED:		
			ss << "\tV" << std::hex << (unsigned short) code.register_x;
			break;

		case JUMP_OFFSET:
			ss << "V0\t0x" << code.address; 
			break;

		case DRAW:
			ss << "\tV" << std::hex << (unsigned short) code.register_x; 
			ss << "\tV" << std::hex << (unsigned short) code.register_y;
			ss << "\t" << std::hex << (unsigned short) code.value;
			break;

		case GET_DELAY_TIMER:
			ss << "\tV" << std::hex << (unsigned short) code.register_x << "\tDT";
			break;

		case WAIT_KEY_PRESSED:
			ss << "\tV" << std::hex << (unsigned short) code.register_x << "\tK";
			break;

		case SET_DELAY_TIMER:
			ss << "\tDT\tV" << std::hex << (unsigned short) code.register_x;
			break;

		case SET_SOUND_TIMER:
			ss << "\tST\tV" << std::hex << (unsigned short) code.register_x;
			break;

		case ADD_ADDRESS_REGISTER:
			ss << "\tI\tV" << std::hex << (unsigned short) code.register_x;
			break;

		case LOAD_SPRITE_ADDRESS:
			ss << "\tF\tV" << std::hex << (unsigned short) code.register_x;
			break;

		case STORE_BCD:
			ss << "\tB\tV" << std::hex << (unsigned short) code.register_x;
			break;

		case STORE_REGISTERS:
			ss << "\t[I]\tV" << std::hex << (unsigned short) code.register_x;
			break;

		case LOAD_REGISTERS:
			ss << "\tV" << std::hex << (unsigned short) code.register_x << "\t[I]";
			break;

		// SChip commands
		case SCROLL_RIGHT:
		case SCROLL_LEFT:
		case EXIT:
		case LORES_MODE:
		case HIRES_MODE:
			break;		

		case SCROLL_DOWN:
			ss << "\t" << std::hex << (unsigned short) (code.value & 0x000F);
			break;

		case LOAD_BIG_SPRITE_ADDRESS:
			ss << "\tHF\tV" << std::hex << (unsigned short) code.register_x;
			break;

		case STORE_REGISTERS_TO_HP:
			ss << "\tR\tV" << std::hex << (unsigned short) code.register_x;
			break;

		case LOAD_REGISTERS_FROM_HP:
			ss << "\tV" << std::hex << (unsigned short) code.register_x << "\tR";
			break; 

		default:
			ss << "\t0x" << std::hex << (unsigned short) code.raw_code;
			break;
	}

	return ss.str();

}


unsigned short Disassembler::get_opcode(unsigned short code)
{
	// Opcodes are generally organized by the first nybble
	switch(code & 0xF000)
	{
		// Several opcodes are uniquely defined by the first nybble
		case JUMP:
		case CALL:
		case SKIP_EQUAL_REGISTER_VALUE:
		case SKIP_NOT_EQUAL_REGISTER_VALUE:	
		case SKIP_EQUAL_REGISTER_REGISTER:	
		case LOAD_REGISTER_VALUE:			
		case ADD_REGISTER_VALUE:
		case SKIP_NOT_EQUAL_REGISTER_REGISTER:
		case LOAD_ADDRESS:
		case JUMP_OFFSET:
		case RANDOM:
		case DRAW:

			return code & 0xF000;
			break;

		// Opcodes starting with 0x0xxx
		case 0x0000:
			// Consider CLEAR_SCREEN, RETURN
			if(code == CLEAR_SCREEN || code == RETURN)
				return code;
			// Check the SChip-8 commands
			if(code == SCROLL_LEFT || code == SCROLL_RIGHT || code == EXIT || code == LORES_MODE || code == HIRES_MODE)
				return code;
			if(code & 0x00F0 == SCROLL_DOWN)
				return SCROLL_DOWN;
			// Otherwise, it should be considered a system call
			else
				return SYS_CALL;
			break;

		// Opcodes starting with 0x8xxx
		case 0x8000:
			// What's the last nybble?
			switch(code & 0xF00F)
			{
				case LOAD_REGISTER_REGISTER:
				case OR:
				case AND:
				case XOR:
				case ADD_REGISTER_REGISTER:
				case SUBTRACT_REGISTER_REGISTER:
				case SHIFT_RIGHT:
				case SUBTRACT_REGISTER_REGISTER_NEGATIVE:
				case SHIFT_LEFT:
					return code & 0xF00F;
					break;
				default:
					// Unknown code
					return 0xFFFF;
					break;
			}
			break;

			// Opcodes starting with 0xExxx and 0xF000
			case 0xE000:
			case 0xF000:
				// Check the last byte
				switch(code & 0xF0FF)
				{
					case SKIP_KEY_PRESSED:
					case SKIP_KEY_NOT_PRESSED:
					case GET_DELAY_TIMER:
					case WAIT_KEY_PRESSED:
					case SET_DELAY_TIMER:
					case SET_SOUND_TIMER:
					case ADD_ADDRESS_REGISTER:
					case LOAD_SPRITE_ADDRESS:
					case STORE_BCD:
					case STORE_REGISTERS:
					case LOAD_REGISTERS:
						return code & 0xF0FF;
						break;
					// SChip-8 Commands
					case LOAD_BIG_SPRITE_ADDRESS:
					case STORE_REGISTERS_TO_HP:
					case LOAD_REGISTERS_FROM_HP:
						return code & 0xF0FF;
						break;
					default:
						return 0xFFFF;
						break;
				}
				break;

			default:
				return 0xFFFF;
	}
}


int main(int argc, char** argv)
{
	// Make sure that a filename is provided to disassemble
	if(argc < 2)
	{
		std::cout << "Binary file not provided!  USAGE:  Disassemble <program.ch8>" << std::endl;
		return 0;
	}

	Disassembler* disassembler = new Disassembler();
	disassembler->create_operation_name_map();
	disassembler->load_rom(argv[1]);
	disassembler->decode();
	disassembler->trace();
	disassembler->print();

	return 0;
}