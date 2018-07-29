#include "Chip8/Emulator/Processor.hpp"
#include "Chip8/Utility/DataTypes.hpp"

#include <fstream>

Chip8Processor::Chip8Processor()
{
}

Chip8Processor::~Chip8Processor()
{
}

void Chip8Processor::initialize()
{
	m_PC			= 0x200;	// First 512 bytes are inaccessible by the program
	m_I				= 0;		// Reset index register
	m_opCode		= 0;		// Reset current OpCode
	m_SP			= 0;		// Reset stack pointer
	m_delayTimer	= 0;		// Reset delay timer
	m_soundTimer	= 0;		// Reset sound timer
	drawFlag		= 0;		// Reset draw flag
	quitFlag		= 0;		// Reset quit flag

	// Reset the memory
	for (size_t i = 0; i < 4096; ++i)
		m_memory[i] = 0;

	// Reset the graphics memory
	for (size_t j = 0; j < 64 * 32; ++j)
		m_graphicsMemory[j] = 0;

	// Reset registers and keys
	for (size_t k = 0; k < 16; ++k)
	{
		m_V[k] = 0;
		m_key[k] = 0;
	}

	// Reset stack
	for (size_t l = 0; l < 24; ++l)
		m_stack[l] = 0;
}

bool Chip8Processor::loadGame(const char *name)
{
	// Load the binary data
	FILE *filePtr = fopen(name, "rb");

	// The file needs to be open for this to work
	if (filePtr == nullptr)
		return false;

	long fileSize = 0.0;

	// Retrieve the size of the file by seeking all the way to the end
	fseek(filePtr, 0, SEEK_END);	// Seek to end of the file
	long fileSize = ftell(filePtr); // Get the current file pointer
	fseek(filePtr, 0, SEEK_SET);	// Seek back to beginning of the file

	// Allocate a buffer of the same size as the ROM
	byte *romData = new byte[fileSize];
	fread(romData, sizeof(byte), fileSize, filePtr);
	fclose(filePtr);

	// Save the ROM to the memory of the processor (offset of 0x200 a.k.a. 512 bytes to account for the reserved space)
	for (long i = 0; i < fileSize; ++i)
		m_memory[512 + i] = romData[i];

	// No need to keep this data around any longer
	delete[] romData;

	// Successfully loaded the ROM!
	return true;
}

void Chip8Processor::newCycle()
{
}

void Chip8Processor::updateDisplay()
{
}

void Chip8Processor::updateKeys()
{
}
