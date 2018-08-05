#pragma once

#include "GL/gl3w.h"

#include <vector>

// Forward declarations
class Window;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void initialize(const Window & window);
	void draw() const;

private:
	std::vector<GLfloat> m_quadVertices;
};