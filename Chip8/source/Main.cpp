#include <iostream>
#include <chrono>

#include "Chip8/Emulator/Processor.hpp"
#include "Chip8/Emulator/Window.hpp"
#include "Chip8/Emulator/Renderer.hpp"

int main(int argc, char const *argv[])
{	
	const char *GAME_PATH = "../roms/games/Rocket Launcher.ch8";

	Chip8Processor chip8Processor;
	chip8Processor.initialize();

	// Crash the emulator when loading fails
	if (!chip8Processor.loadGame(GAME_PATH))
	{
		printf("Failed to the ROM.\n");
		std::cin.get();
		return -1;
	}
	
	Window window;
	Renderer renderer;

	if (!window.create("Chip8 emulation - Tahar Meijs", 640, 320, 3, 3))
		return -1;

	if (!renderer.initialize(window))
		return -1;

	printf("ROM successfully loaded.\n");

	std::chrono::high_resolution_clock::time_point then = std::chrono::high_resolution_clock::now();

	// Main application loop
	while (chip8Processor.quitFlag == 0)
	{
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		float duration = std::chrono::duration_cast<std::chrono::duration<float>>(now - then).count();
		
		// The Chip8 runs at a clock speed of 500Hz
		if (duration < 0.002f)
			continue;

		// The Chip8 timers should update at 60Hz
		if (duration >= 0.016f)
			chip8Processor.updateTimers();

		// Simulate a CPU cycle
		chip8Processor.newCycle();

		// Draw whenever a clear / display OpCode has been processed
		if (chip8Processor.drawFlag == 1)
			chip8Processor.updateDisplay(window, renderer);

		// Update the input
		chip8Processor.updateKeys(window);

		// Update the emulator global timer
		then = now;
	}

	window.quit();

    return 0;
}
