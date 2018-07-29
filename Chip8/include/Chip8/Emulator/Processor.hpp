#pragma once

#include "Chip8/Utility/DataTypes.hpp"

class Chip8Processor
{
public:
	Chip8Processor();
	~Chip8Processor();

	void initialize();
	void loadGame(const char * name);
	void newCycle();
	void updateDisplay();
	void updateKeys();

	byte drawFlag;
	byte quitFlag;

private:
	// The processor has 4096 bytes of memory
	byte m_memory[4096];

	// The display has a resolution of 64x32 pixels
	byte m_graphicsMemory[64 * 32];

	// 16 Registers (8-bit) in total
	byte m_V[16];

	// Timers
	byte m_delayTimer;
	byte m_soundTimer;

	// Stack (24 levels of nesting)
	byte m_stack[24];

	// Stack pointer to keep track of the current level in the stack
	byte m_SP;

	// Hex keypad
	byte m_key[16];
	
	// A single OpCode is 2 bytes
	word m_opCode;

	// Address register (16 bits)
	word m_I;

	// Program counter (16 bits)
	word m_PC;
};