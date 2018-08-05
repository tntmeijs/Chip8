#include "Chip8/Emulator/Renderer.hpp"
#include "Chip8/Emulator/Window.hpp"

#include "GL/gl3w.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::initialize(const Window & window)
{
	// Set the framebuffer to cover the complete window
	int width = 0;
	int height = 0;
	window.getFramebufferDimensions(width, height);

	glViewport(0, 0, window.getWidth(), window.getHeight());
	glClearColor(0.223f, 0.8f, 0.8f, 1.0f);	// This color is called "teal"

	// Create a simple quad, no indices needed
	m_quadVertices =
	{
		 0.5f,  0.5f,
		-0.5f,  0.5f,
		-0.5f, -0.5f,

		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f
	};
}

void Renderer::draw() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}
