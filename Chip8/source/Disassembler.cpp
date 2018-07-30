#include "Chip8/Utility/Disassembler.hpp"
#include "Chip8/Utility/DataTypes.hpp"

#include <iostream>

Chip8Disassembler::Chip8Disassembler()
{
}

Chip8Disassembler::~Chip8Disassembler()
{
}

void Chip8Disassembler::disassemble(word startLocationOfPC, word memorySize, byte * memory)
{
	m_PC = startLocationOfPC;

	// Column names in the disassembly console view
	printf("=================================\n");
	printf("Index | OpCode | Assembly Command\n");
	printf("=================================\n");

	// Add the offset of the PC to the memory size to get the correct end of the binary data in the ROM
	while (m_PC < startLocationOfPC + memorySize)
	{
		// Fetch OpCode
		word opCode = memory[m_PC] << 8 | memory[m_PC + 1];

		// Print the program counter and OpCode values in hexadecimal
		printf("0x%04X\t0x%04X\t", m_PC, opCode);

		// Decode the first number of the OpCode
		switch (opCode & 0xF000)
		{
		case 0x0000:
			switch (opCode & 0x000F)
			{
			case 0x0000:
				printf("CLS\n");
				break;

			case 0x000E:
				printf("RET\n");
				break;

			default:
				// The "SYS" command that calls an RCA 1802 program
				printf("SYS\t0x%3X\n", opCode & 0x0FFF);
				break;
			}
			break;

		case 0x1000:
			printf("JP\t0x%3X\n", opCode & 0x0FFF);
			break;

		case 0x2000:
			printf("CALL\t0x%3X\n", opCode & 0x0FFF);
			break;

		case 0x3000:
			printf("SE\tV%1X, 0x%2X\n", (opCode & 0x0F00) >> 8, opCode & 0x00FF);
			break;

		case 0x4000:
			printf("SNE\tV%1X, 0x%2X\n", (opCode & 0x0F00) >> 8, opCode & 0x00FF);
			break;

		case 0x5000:
			printf("SE\tV%1X, V%1X\n", (opCode & 0x0F00) >> 8, (opCode & 0x00F0) >> 4);
			break;

		case 0x6000:
			printf("LD\tV%1X, %i\n", (opCode & 0x0F00) >> 8, opCode & 0x00FF);
			break;

		case 0x7000:
			printf("ADD\tV%1X, %i\n", (opCode & 0x0F00) >> 8, opCode & 0x00FF);
			break;

		case 0x8000:
			switch (opCode & 0x000F)
			{
			case 0x0000:
				printf("LD\tV%1X, V%1X\n", (opCode & 0x0F00) >> 8, (opCode & 0x00F0) >> 4);
				break;

			case 0x0001:
				printf("OR\tV%1X, V%1X\n", (opCode & 0x0F00) >> 8, (opCode & 0x00F0) >> 4);
				break;

			case 0x0002:
				printf("AND\tV%1X, V%1X\n", (opCode & 0x0F00) >> 8, (opCode & 0x00F0) >> 4);
				break;

			case 0x0003:
				printf("XOR\tV%1X, V%1X\n", (opCode & 0x0F00) >> 8, (opCode & 0x00F0) >> 4);
				break;

			case 0x0004:
				printf("ADD\tV%1X, V%1X\n", (opCode & 0x0F00) >> 8, (opCode & 0x00F0) >> 4);
				break;

			case 0x0005:
				printf("SUB\tV%1X, V%1X\n", (opCode & 0x0F00) >> 8, (opCode & 0x00F0) >> 4);
				break;

			case 0x0006:
				printf("SHR\tV%1X {, V%1X}\n", (opCode & 0x0F00) >> 8, (opCode & 0x00F0) >> 4);
				break;

			case 0x0007:
				printf("SUBN\tV%1X, V%1X\n", (opCode & 0x0F00) >> 8, (opCode & 0x00F0) >> 4);
				break;

			case 0x000E:
				printf("SHL\tV%1X {, V%1X}\n", (opCode & 0x0F00) >> 8, (opCode & 0x00F0) >> 4);
				break;

			default:
				printf("\n");
				break;
			}
			break;

		case 0x9000:
			printf("SNE\tV%1X, V%1X\n", (opCode & 0x0F00) >> 8, (opCode & 0x00F0) >> 4);
			break;

		case 0xA000:
			printf("LD\tI, 0x%3X\n", opCode & 0x0FFF);
			break;

		case 0xB000:
			printf("JP\tV0, 0x%3X\n", opCode & 0x0FFF);
			break;

		case 0xC000:
			printf("RND\tV%1X, %i\n", (opCode & 0x0F00) >> 8, opCode & 0x00FF);
			break;

		case 0xD000:
			printf("DRW\tV%1X, V%1X, %i\n", (opCode & 0x0F00) >> 8, (opCode & 0x00F0) >> 4, (opCode & 0x000F));
			break;

		case 0xE000:
			switch (opCode & 0x00FF)
			{
			case 0x009E:
				printf("SKP\tV%1X\n", (opCode & 0x0F00) >> 8);
				break;

			case 0x00A1:
				printf("SKNP\tV%1X\n", (opCode & 0x0F00) >> 8);
				break;

			default:
				printf("\n");
				break;
			}
			break;

		case 0xF000:
			switch (opCode & 0x00FF)
			{
			case 0x0007:
				printf("LD\tV%1X, DT\n", (opCode & 0x0F00) >> 8);
				break;

			case 0x000A:
				printf("LD\tV%1X, K\n", (opCode & 0x0F00) >> 8);
				break;

			case 0x0015:
				printf("LD\tDT, V%1X\n", (opCode & 0x0F00) >> 8);
				break;

			case 0x0018:
				printf("LD\tST, V%1X\n", (opCode & 0x0F00) >> 8);
				break;

			case 0x001E:
				printf("ADD\tI, V%1X\n", (opCode & 0x0F00) >> 8);
				break;

			case 0x0029:
				printf("LD\tF, V%1X\n", (opCode & 0x0F00) >> 8);
				break;

			case 0x0033:
				printf("LD\tB, V%1X\n", (opCode & 0x0F00) >> 8);
				break;

			case 0x0055:
				printf("LD\t[I], V%1X\n", (opCode & 0x0F00) >> 8);
				break;

			case 0x0065:
				printf("LD\tV%1X, [I]\n", (opCode & 0x0F00) >> 8);
				break;

			default:
				printf("\n");
				break;
			}
			break;

		default:
			printf("\n");
			break;
		}

		// Move the program counter one instruction ahead (2 bytes)
		m_PC += 2;
	}
}
