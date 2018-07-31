#pragma once

#include "DataTypes.hpp"

class Chip8Disassembler
{
public:
	Chip8Disassembler();
	~Chip8Disassembler();

	void disassemble(word startLocationOfPC, word memorySize, byte * memory);
	static void printOpCode(word opCode);

private:
	word m_PC;
	word m_opCode;
};