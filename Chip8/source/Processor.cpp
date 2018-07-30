#include "Chip8/Emulator/Processor.hpp"
#include "Chip8/Utility/DataTypes.hpp"

#include <fstream>

Chip8Processor::Chip8Processor()
{
}

Chip8Processor::~Chip8Processor()
{
	if (m_finalizeCalled == 0)
		finalize();
}

void Chip8Processor::initialize()
{
	m_PC				= 0x200;	// First 512 bytes are inaccessible by the program
	m_I					= 0;		// Reset index register
	m_opCode			= 0;		// Reset current OpCode
	m_SP				= 0;		// Reset stack pointer
	m_delayTimer		= 0;		// Reset delay timer
	m_soundTimer		= 0;		// Reset sound timer
	drawFlag			= 0;		// Reset draw flag
	quitFlag			= 0;		// Reset quit flag
	m_finalizeCalled	= 0;		// Reset finalization flag
	m_applicationSize	= 0;		// Reset the size of the loaded application or game

	// Reset the memory
	m_memory = new byte[MEMORY_SIZE_BYTES];
	for (size_t i = 0; i < MEMORY_SIZE_BYTES; ++i)
		m_memory[i] = 0;

	// Reset the graphics memory
	m_graphicsMemory = new byte[64 * 32];
	for (size_t j = 0; j < 64 * 32; ++j)
		m_graphicsMemory[j] = 0;

	// Reset registers and keys
	m_V		= new byte[16];
	m_key	= new byte[16];
	for (size_t k = 0; k < 16; ++k)
	{
		m_V[k]		= 0;
		m_key[k]	= 0;
	}

	// Reset stack
	m_stack = new byte[24];
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

	// Retrieve the size of the file by seeking all the way to the end
	fseek(filePtr, 0, SEEK_END);	// Seek to end of the file
	m_applicationSize = ftell(filePtr); // Get the current file pointer
	fseek(filePtr, 0, SEEK_SET);	// Seek back to beginning of the file

	// Allocate a buffer of the same size as the ROM
	byte *romData = new byte[m_applicationSize];
	fread(romData, sizeof(byte), m_applicationSize, filePtr);
	fclose(filePtr);

	// Save the ROM to the memory of the processor (offset of 0x200 a.k.a. 512 bytes to account for the reserved space)
	for (long i = 0; i < m_applicationSize; ++i)
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

void Chip8Processor::finalize()
{
	delete[] m_memory;
	delete[] m_graphicsMemory;
	delete[] m_V;
	delete[] m_stack;
	delete[] m_key;

	m_finalizeCalled = 1;
}

const word Chip8Processor::getPC() const
{
	return m_PC;
}

const long Chip8Processor::getApplicationSize() const
{
	return m_applicationSize;
}

byte *Chip8Processor::getMemoryStart() const
{
	return m_finalizeCalled == 0 ? &m_memory[0] : nullptr;
}
