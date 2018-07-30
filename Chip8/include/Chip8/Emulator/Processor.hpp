#pragma once

#include "Chip8/Utility/DataTypes.hpp"

class Chip8Processor
{
public:
	Chip8Processor();
	~Chip8Processor();

	void initialize();
	bool loadGame(const char *name);
	void newCycle();
	void updateDisplay();
	void updateKeys();
	void finalize();

	const word getPC() const;
	const long getApplicationSize() const;

	byte *getMemoryStart() const;

	byte drawFlag;
	byte quitFlag;

	const word MEMORY_SIZE_BYTES = 4096;

private:
	// Flag that indicates whether the memory has already been deallocated
	byte m_finalizeCalled;

	// The processor has 4096 bytes of memory
	byte *m_memory;

	// The display has a resolution of 64x32 pixels
	byte *m_graphicsMemory;

	// 16 Registers (8-bit) in total
	byte *m_V;

	// Timers
	byte m_delayTimer;
	byte m_soundTimer;

	// Stack (16 levels of nesting)
	byte *m_stack;

	// Stack pointer to keep track of the current level in the stack
	byte m_SP;

	// Hex keypad
	byte * m_key;
	
	// A single OpCode is 2 bytes
	word m_opCode;

	// Address register (16 bits)
	word m_I;

	// Program counter (16 bits)
	word m_PC;

	// Size of the loaded application or game
	long m_applicationSize;
};