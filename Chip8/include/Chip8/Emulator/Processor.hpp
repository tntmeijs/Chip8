#pragma once

#include "Chip8/Utility/DataTypes.hpp"

// Forward declarations
class Window;

class Chip8Processor
{
public:
	Chip8Processor();
	~Chip8Processor();

	void initialize();
	bool loadGame(const char *name);
	void newCycle();
	void updateDisplay();
	void updateKeys(const Window & window);
	void updateTimers();
	void finalize();

	const word getPC() const;
	const long getApplicationSize() const;

	byte *getMemoryStart() const;

public:
	byte drawFlag;
	byte quitFlag;

	const word MEMORY_SIZE_BYTES = 4096;

private:
	// Functions for the OpCodes in the switch statement
	void CLS(word opCode);
	void RET(word opCode);
	void SYSaddr(word opCode);
	void JPaddr(word opCode);
	void CALLaddr(word opCode);
	void SEvxbyte(word opCode);
	void SNEvxbyte(word opCode);
	void SEvxvy(word opCode);
	void LDvxbyte(word opCode);
	void ADDvxbyte(word opCode);
	void LDvxvy(word opCode);
	void ORvxvy(word opCode);
	void ANDvxvy(word opCode);
	void XORvxvy(word opCode);
	void ADDvxvy(word opCode);
	void SUBvxvy(word opCode);
	void SHRvxvy(word opCode);
	void SUBNvxvy(word opCode);
	void SHLvxvy(word opCode);
	void SNEvxvy(word opCode);
	void LDiaddr(word opCode);
	void JPv0addr(word opCode);
	void RNDvxbyte(word opCode);
	void DRWvxvynibble(word opCode);
	void SKPvx(word opCode);
	void SKNPvx(word opCode);
	void LDvxdt(word opCode);
	void LDvxk(word opCode);
	void LDdtvx(word opCode);
	void LDstvx(word opCode);
	void ADDivx(word opCode);
	void LDfvx(word opCode);
	void LDbvx(word opCode);
	void LDivx(word opCode);
	void LDvxi(word opCode);

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

	// Hex keypad
	byte * m_key;

	// Stack pointer to keep track of the current level in the stack
	word m_SP;

	// Stack (16 levels of nesting)
	word *m_stack;
	
	// A single OpCode is 2 bytes
	word m_opCode;

	// Address register (16 bits)
	word m_I;

	// Program counter (16 bits)
	word m_PC;

	// Size of the loaded application or game
	long m_applicationSize;
};