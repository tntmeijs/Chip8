#include "Chip8/Emulator/Processor.hpp"

int main(int argc, char const *argv[])
{
	// TODO: add OpenGL initialization
	// TODO: add GLFW3 input initialization

	Chip8Processor chip8Processor;
	chip8Processor.initialize();
	chip8Processor.loadGame("");	// No game just yet... :(

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
