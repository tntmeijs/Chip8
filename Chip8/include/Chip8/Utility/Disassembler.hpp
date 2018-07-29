#pragma once

#include "DataTypes.hpp"

class Chip8Disassembler
{
public:
	Chip8Disassembler();
	~Chip8Disassembler();

	void disassemble(word startLocationOfPC) const;

private:
	word m_PC;
	word m_opCode;
};