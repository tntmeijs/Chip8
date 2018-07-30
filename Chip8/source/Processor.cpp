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

	// Reset registers, stack, and keys
	m_V		= new byte[16];
	m_key	= new byte[16];
	m_stack = new byte[16];
	for (size_t k = 0; k < 16; ++k)
	{
		m_V[k]		= 0;
		m_key[k]	= 0;
		m_stack[k]	= 0;
	}
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
	// Fetch OpCode (combines two bytes into a word)
	word opCode = m_memory[m_PC] << 8 | m_memory[m_PC + 1];

	// Decode the first number of the OpCode
	switch (opCode & 0xF000)
	{
	case 0x0000:
		switch (opCode & 0x000F)
		{
			// CLS
		case 0x0000:
			break;

			// RET
		case 0x000E:
			break;

			// SYS address
		default:
			break;
		}
		break;

		// JP address
	case 0x1000:
		break;

		// CALL address
	case 0x2000:
		break;

		// SE Vx, byte
	case 0x3000:
		break;

		// SNE Vx, byte
	case 0x4000:
		break;

		// SE Vx, Vy
	case 0x5000:
		break;

		// LD Vx, byte
	case 0x6000:
		break;

		// ADD Vx, byte
	case 0x7000:
		break;

	case 0x8000:
		switch (opCode & 0x000F)
		{
			// LD Vx, Vy
		case 0x0000:
			break;

			// OR Vx, Vy
		case 0x0001:
			break;

			// AND Vx, Vy
		case 0x0002:
			break;

			// XOR Vx, Vy
		case 0x0003:
			break;

			// ADD Vx, Vy
		case 0x0004:
			break;

			// SUB Vx, Vy
		case 0x0005:
			break;

			// SHR Vx {, Vy}
		case 0x0006:
			break;

			// SUBN Vx, Vy
		case 0x0007:
			break;

			// SHL Vx {, Vy}
		case 0x000E:
			break;

		default:
			break;
		}
		break;

		// SNE Vx, Vy
	case 0x9000:
		break;

		// LD I, addr
	case 0xA000:
		break;

		// JP V0, addr
	case 0xB000:
		break;

		// RND Vx, byte
	case 0xC000:
		break;

		// DRW Vx, Vy, nibble
	case 0xD000:
		break;

	case 0xE000:
		switch (opCode & 0x00FF)
		{
			// SKP Vx
		case 0x009E:
			break;

			// SKNP Vx
		case 0x00A1:
			break;

		default:
			break;
		}
		break;

	case 0xF000:
		switch (opCode & 0x00FF)
		{
			// LD Vx, DT
		case 0x0007:
			break;

			// LD Vx, K
		case 0x000A:
			break;

			// LD DT, Vx
		case 0x0015:
			break;
			
			// LD ST, Vx
		case 0x0018:
			printf("LD\tST, V%1X\n", (opCode & 0x0F00) >> 8);
			break;

			// ADD I, Vx
		case 0x001E:
			printf("ADD\tI, V%1X\n", (opCode & 0x0F00) >> 8);
			break;

			// LD F, Vx
		case 0x0029:
			printf("LD\tF, V%1X\n", (opCode & 0x0F00) >> 8);
			break;

			// LD B, Vx
		case 0x0033:
			printf("LD\tB, V%1X\n", (opCode & 0x0F00) >> 8);
			break;

			// LD [I], Vx
		case 0x0055:
			printf("LD\t[I], V%1X\n", (opCode & 0x0F00) >> 8);
			break;

			// LD Vx, [I]
		case 0x0065:
			printf("LD\tV%1X, [I]\n", (opCode & 0x0F00) >> 8);
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
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
