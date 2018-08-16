#pragma once

#include "Chip8/Utility/DataTypes.hpp"

#include "GL/gl3w.h"

// Forward declarations
class Window;

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool initialize(const Window & window);
	void draw() const;
	void updatePixels(byte *graphicsMemory) const;

private:
	bool setupShaders();
	void setupTexture();
	void setupQuad();

private:
	GLuint m_quadVAO;
	GLuint m_quadVBO;
	GLuint m_shader;
	GLuint m_texture;
};