#include <iostream>

#include "Chip8/Emulator/Processor.hpp"
#include "Chip8/Utility/Disassembler.hpp"

int main(int argc, char const *argv[])
{
	// TODO: add OpenGL initialization
	// TODO: add GLFW3 input initialization

	Chip8Processor chip8Processor;
	chip8Processor.initialize();

	// Crash the emulator when loading fails
	if (!chip8Processor.loadGame("../roms/games/Missile [David Winter].ch8"))
	{
		printf("Failed to the ROM.\n");
		std::cin.get();
		return -1;
	}

	printf("ROM successfully loaded.\n");

	Chip8Disassembler chip8Disassembler;
	chip8Disassembler.disassemble(chip8Processor.getPC(), chip8Processor.getApplicationSize(), chip8Processor.getMemoryStart());

	// Main application loop
	while (!chip8Processor.quitFlag)
	{
		// Simulate a CPU cycle
		chip8Processor.newCycle();

		// Draw whenever a clear / display OpCode has been processed
		if (chip8Processor.drawFlag)
			chip8Processor.updateDisplay();

		// Update the input
		chip8Processor.updateKeys();
	}

    return 0;
}
