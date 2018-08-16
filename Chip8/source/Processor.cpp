#include "Chip8/Emulator/Processor.hpp"
#include "Chip8/Utility/DataTypes.hpp"
#include "Chip8/Emulator/Window.hpp"
#include "Chip8/Emulator/Renderer.hpp"

#include <fstream>
#include <random>
#include <chrono>

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
	m_stack = new word[16];
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

	// TODO: use function pointers instead of this horrible switch statement
	// Decode the first number of the OpCode
	// Reference: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.0
	switch (opCode & 0xF000)
	{
	case 0x0000:
		switch (opCode & 0x000F)
		{
			// 00E0 - CLS (Clear the display)
		case 0x0000:
			CLS(opCode);
			break;

			// 00EE - RET (The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer)
		case 0x000E:
			RET(opCode);
			break;

			// 0nnn - SYS address (Execute a RCA 1802 program, not implemented in this emulator)
		default:
			SYSaddr(opCode);
			break;
		}
		break;

		// 1nnn - JP addr (The interpreter sets the program counter to nnn)
	case 0x1000:
		JPaddr(opCode);
		break;

		// 2nnn - CALL addr (The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn)
	case 0x2000:
		CALLaddr(opCode);
		break;

		// 3xkk - SE Vx, byte (The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2)
	case 0x3000:
		SEvxbyte(opCode);
		break;

		// 4xkk - SNE Vx, byte (The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2)
	case 0x4000:
		SNEvxbyte(opCode);
		break;

		// 5xy0 - SE Vx, Vy (The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2)
	case 0x5000:
		SEvxvy(opCode);
		break;

		// 6xkk - LD Vx, byte (The interpreter puts the value kk into register Vx)
	case 0x6000:
		LDvxbyte(opCode);
		break;

		// 7xkk - ADD Vx, byte (Adds the value kk to the value of register Vx, then stores the result in Vx)
	case 0x7000:
		ADDvxbyte(opCode);
		break;

	case 0x8000:
		switch (opCode & 0x000F)
		{
			// 8xy0 - LD Vx, Vy (Stores the value of register Vy in register Vx)
		case 0x0000:
			LDvxvy(opCode);
			break;

			// 8xy1 - OR Vx, Vy (Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx)
		case 0x0001:
			ORvxvy(opCode);
			break;

			// 8xy2 - AND Vx, Vy (Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx)
		case 0x0002:
			ANDvxvy(opCode);
			break;

			// 8xy3 - XOR Vx, Vy (Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx)
		case 0x0003:
			XORvxvy(opCode);
			break;

			// 8xy4 - ADD Vx, Vy (The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,)
			//					  VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.)
		case 0x0004:
			ADDvxvy(opCode);
			break;

			// 8xy5 - SUB Vx, Vy (If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx)
		case 0x0005:
			SUBvxvy(opCode);
			break;

			// 8xy6 - SHR Vx {, Vy} (If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2)
		case 0x0006:
			SHRvxvy(opCode);
			break;

			// 8xy7 - SUBN Vx, Vy (If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx)
		case 0x0007:
			SUBNvxvy(opCode);
			break;

			// 8xyE - SHL Vx {, Vy} (If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2)
		case 0x000E:
			SHLvxvy(opCode);
			break;

		default:
			break;
		}
		break;

		// 9xy0 - SNE Vx, Vy (The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2)
	case 0x9000:
		SNEvxvy(opCode);
		break;

		// Annn - LD I, addr (The value of register I is set to nnn)
	case 0xA000:
		LDiaddr(opCode);
		break;

		// Bnnn - JP V0, addr (The program counter is set to nnn plus the value of V0)
	case 0xB000:
		JPv0addr(opCode);
		break;

		// Cxkk - RND Vx, byte (The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk.
		//						The results are stored in Vx)
	case 0xC000:
		RNDvxbyte(opCode);
		break;

		// TODO: add a setting for the end user to toggle screen coordinate wrapping
		// Dxyn - DRW Vx, Vy, nibble (The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then
		//							  displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen.
		//							  If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned
		//							  so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen)
	case 0xD000:
		DRWvxvynibble(opCode);
		break;

	case 0xE000:
		switch (opCode & 0x00FF)
		{
			// Ex9E - SKP Vx (Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2)
		case 0x009E:
			SKPvx(opCode);
			break;

			// ExA1 - SKNP Vx (Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2)
		case 0x00A1:
			SKNPvx(opCode);
			break;

		default:
			break;
		}
		break;

	case 0xF000:
		switch (opCode & 0x00FF)
		{
			// Fx07 - LD Vx, DT (The value of DT is placed into Vx)
		case 0x0007:
			LDvxdt(opCode);
			break;

			// Fx0A - LD Vx, K (All execution stops until a key is pressed, then the value of that key is stored in Vx)
		case 0x000A:
			LDvxk(opCode);
			break;

			// Fx15 - LD DT, Vx (DT is set equal to the value of Vx)
		case 0x0015:
			LDdtvx(opCode);
			break;
			
			// Fx18 - LD ST, Vx (ST is set equal to the value of Vx)
		case 0x0018:
			LDstvx(opCode);
			break;

			// Fx1E - ADD I, Vx (The values of I and Vx are added, and the results are stored in I)
		case 0x001E:
			ADDivx(opCode);
			break;

			// Fx29 - LD F, Vx (The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx)
		case 0x0029:
			LDfvx(opCode);
			break;

			// Fx33 - LD B, Vx (The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I,
			//					the tens digit at location I+1, and the ones digit at location I+2)
		case 0x0033:
			LDbvx(opCode);
			break;

			// Fx55 - LD [I], Vx (The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I)
		case 0x0055:
			LDivx(opCode);
			break;

			// Fx65 - LD Vx, [I] (The interpreter reads values from memory starting at location I into registers V0 through Vx)
		case 0x0065:
			LDvxi(opCode);
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

	// Advance to the next instruction
	m_PC += 2;
}

void Chip8Processor::updateDisplay(const Window & window, const Renderer & renderer)
{
	// Save the new framebuffer to the render texture
	renderer.updatePixels(m_graphicsMemory);

	// Render the new frame
	renderer.draw();

	// Swap framebuffers
	window.display();

	drawFlag = 0;
}

void Chip8Processor::updateKeys(const Window & window)
{
	window.pollKeyboard();
}

void Chip8Processor::updateTimers()
{
	// Update the delay timer and the sound timer if necessary
	if (m_delayTimer > 0)
		--m_delayTimer;

	if (m_soundTimer > 0)
		--m_soundTimer;
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

void Chip8Processor::CLS(word opCode)
{
	for (size_t i = 0; i < 64 * 32; ++i)
		m_graphicsMemory[i] = 0;
}

void Chip8Processor::RET(word opCode)
{
	m_PC = m_stack[m_SP--];
}

void Chip8Processor::SYSaddr(word opCode)
{
	// Not implemented, just skip this instruction...
}

void Chip8Processor::JPaddr(word opCode)
{
	m_PC = (opCode & 0x0FFF);
}

void Chip8Processor::CALLaddr(word opCode)
{
	m_stack[++m_SP] = m_PC;
	m_PC = (opCode & 0x0FFF);
}

void Chip8Processor::SEvxbyte(word opCode)
{
	if (m_V[(opCode & 0x0F00) >> 8] == (opCode & 0x00FF))
		m_PC += 2;
}

void Chip8Processor::SNEvxbyte(word opCode)
{
	if (m_V[(opCode & 0x0F00) >> 8] != (opCode & 0x00FF))
		m_PC += 2;
}

void Chip8Processor::SEvxvy(word opCode)
{
	if (m_V[(opCode & 0x0F00) >> 8] == m_V[(opCode & 0x00F0) >> 4])
		m_PC += 2;
}

void Chip8Processor::LDvxbyte(word opCode)
{
	m_V[(opCode & 0x0F00) >> 8] = opCode & 0x00FF;
}

void Chip8Processor::ADDvxbyte(word opCode)
{
	m_V[(opCode & 0x0F00) >> 8] += opCode & 0x00FF;
}

void Chip8Processor::LDvxvy(word opCode)
{
	m_V[(opCode & 0x0F00) >> 8] = m_V[(opCode & 0x00F0) >> 4];
}

void Chip8Processor::ORvxvy(word opCode)
{
	m_V[(opCode & 0x0F00) >> 8] |= m_V[(opCode & 0x00F0) >> 4];
}

void Chip8Processor::ANDvxvy(word opCode)
{
	m_V[(opCode & 0x0F00) >> 8] &= m_V[(opCode & 0x00F0) >> 4];
}

void Chip8Processor::XORvxvy(word opCode)
{
	m_V[(opCode & 0x0F00) >> 8] ^= m_V[(opCode & 0x00F0) >> 4];
}

void Chip8Processor::ADDvxvy(word opCode)
{
	// Add Vy to Vx
	m_V[(opCode & 0x0F00) >> 8] += m_V[(opCode & 0x00F0) >> 4];

	// Check whether the sum of the values of the two registers exceed 8 bits (0xFF)
	// This calculation checks whether the value of X is bigger than the maximum (255) minus the value of Y,
	// if it is, it means that the value caused the byte to start counting from zero again.
	if (m_V[(opCode & 0x0F00) >> 8] > 0xFF - m_V[(opCode & 0x00F0) >> 4])
		m_V[0xF] = 1;	// Carry flag set
	else
		m_V[0xF] = 0;	// Carry flag unset
}

void Chip8Processor::SUBvxvy(word opCode)
{
	if (m_V[(opCode & 0x0F00) >> 8] > m_V[(opCode & 0x00F0) >> 4])
		m_V[0xF] = 1;
	else
		m_V[0xF] = 0;

	m_V[(opCode & 0x0F00) >> 8] -= m_V[(opCode & 0x00F0) >> 4];
}

void Chip8Processor::SHRvxvy(word opCode)
{
	if ((opCode & 1) == 1)
		m_V[0xF] = 1;
	else
		m_V[0xF] = 0;

	// Divide the value of Vx by 2
	m_V[(opCode & 0x0F00) >> 8] /= 2;
}

void Chip8Processor::SUBNvxvy(word opCode)
{
	if (m_V[(opCode & 0x0F00) >> 8] > m_V[(opCode & 0x00F0) >> 4])
		m_V[0xF] = 1;	// No borrow flag set
	else
		m_V[0xF] = 0;	// No borrow flag unset

						// Subtract Vy from Vx
	m_V[(opCode & 0x0F00) >> 8] -= m_V[(opCode & 0x00F0) >> 4];
}

void Chip8Processor::SHLvxvy(word opCode)
{
	// Since the value in the register is 8 bits, the MSB can be retrieved by shifting all bits 7 places to the right
	if ((m_V[(opCode & 0x0F00) >> 8] >> 7) == 1)
		m_V[0xF] = 1;
	else
		m_V[0xF] = 0;

	// Multiply the value of Vx by 2
	m_V[(opCode & 0x0F00) >> 8] *= 2;
}

void Chip8Processor::SNEvxvy(word opCode)
{
	if (m_V[(opCode & 0x0F00) >> 8] != m_V[(opCode & 0x00F0) >> 4])
		m_PC += 2;
}

void Chip8Processor::LDiaddr(word opCode)
{
	m_I = (opCode & 0X0FFF);
}

void Chip8Processor::JPv0addr(word opCode)
{
	m_PC = (opCode & 0x0FFF) + m_V[0x0];
}

void Chip8Processor::RNDvxbyte(word opCode)
{
	// Create and seed the random number generator
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> distribution(0, 255);

	// Get a random value
	byte randomValue = static_cast<byte>(distribution(generator));

	// Perform bit-wise AND on kk and the random number, then store the result in register Vx
	m_V[(opCode & 0x0F00) >> 8] = randomValue & (opCode & 0x00FF);
}

// Dxyn - DRW Vx, Vy, nibble (The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then
//							  displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen.
//							  If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned
//							  so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen)
void Chip8Processor::DRWvxvynibble(word opCode)
{
	byte coordinateX	= m_V[(opCode & 0x0F00) >> 8];
	byte coordinateY	= m_V[(opCode & 0x00F0) >> 4];
	byte numOfBytes		= (opCode & 0x000F);

	// Reset the Vf register
	m_V[0xF] = 0;

	// Loop through every byte of the sprite
	for (byte i = 0; i < numOfBytes; ++i)
	{
		// Retrieve the sprite data
		byte spriteData = m_memory[m_I + i];

		for (byte j = 0; j < 8; ++j)
		{
			// Check every bit of the byte
			if ((spriteData & (0x80 >> j)) != 0)
			{
				// If the pixel on the display is already set to one, the Vf register needs to be set
				if (m_graphicsMemory[(coordinateX + j + ((coordinateY + i) * 64))] == 1)
					m_V[0xF] = 1;
				
				// XOR the new pixel value with the existing screen pixel value
				m_graphicsMemory[coordinateX + j + ((coordinateY + i) * 64)] ^= 1;
			}
		}
	}

	drawFlag = 1;
}

void Chip8Processor::SKPvx(word opCode)
{
	if (m_V[(opCode & 0x0F00) >> 8] == 1)	// Key down, contact
		m_PC += 2;
}

void Chip8Processor::SKNPvx(word opCode)
{
	if (m_V[(opCode & 0x0F00) >> 8] == 0)	// Key up, no contact
		m_PC += 2;
}

void Chip8Processor::LDvxdt(word opCode)
{
	m_V[(opCode & 0x0F00) >> 8] = m_delayTimer;
}

void Chip8Processor::LDvxk(word opCode)
{
	for (byte i = 0; i < 16; ++i)
		if (m_key[i] == 1)
			m_V[(opCode & 0x0F00) >> 8] = m_key[i];
}

void Chip8Processor::LDdtvx(word opCode)
{
	m_delayTimer = m_V[(opCode & 0x0F00) >> 8];
}

void Chip8Processor::LDstvx(word opCode)
{
	m_soundTimer = m_V[(opCode & 0x0F00) >> 8];
}

void Chip8Processor::ADDivx(word opCode)
{
	m_I += m_V[(opCode & 0x0F00) >> 8];
}

void Chip8Processor::LDfvx(word opCode)
{
	m_I = m_V[(opCode & 0x0F00) >> 8];
}

void Chip8Processor::LDbvx(word opCode)
{
	byte value = m_V[(opCode & 0x0F00) >> 8];

	m_memory[m_I + 0] = value / 100;		// Ones
	m_memory[m_I + 1] = (value / 10) % 10;	// Tens
	m_memory[m_I + 2] = value % 10;			// Hundreds
}

void Chip8Processor::LDivx(word opCode)
{
	for (byte i = 0; i < ((opCode & 0x0F00) >> 8); ++i)
		m_memory[m_I + i] = m_V[i];
}

void Chip8Processor::LDvxi(word opCode)
{
	for (byte i = 0; i < ((opCode & 0x0F00) >> 8); ++i)
		m_V[i] = m_memory[m_I + i];
}
