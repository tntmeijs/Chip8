#pragma once

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

private:
	bool setupShaders();
	void setupTexture(int width, int height);
	void setupQuad();

private:
	GLuint m_quadVAO;
	GLuint m_quadVBO;
	GLuint m_shader;
	GLuint m_texture;
};