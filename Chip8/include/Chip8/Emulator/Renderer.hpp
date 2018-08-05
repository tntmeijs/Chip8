#pragma once

#include "GL/gl3w.h"

#include <vector>

// Forward declarations
class Window;

// Shaders are not abstracted because of the simplicity of this renderer
const GLchar *const vertexShaderSourceCode =	"#version 330 core\n"
												"layout(location = 0) in vec2 position;\n"
												"layout(location = 1) in vec2 texCoord;\n"
	
												"out vec2 uv;\n"

												"void main() {\n"
													"uv = texCoord;\n"
													"gl_Position = vec4(position, 0.0, 0.0);\n"
												"}\0";

const GLchar *const fragmentShaderSourceCode =	"#version 330 core\n"

												"in vec2 uv;\n"
												"out vec4 fragColor;\n"

												"uniform sampler2D textureID;\n"
										
												"void main() {\n"
													"float grayscaleValue = texture(textureID, uv).r;\n"
													"fragColor = vec4(grayscaleValue, grayscaleValue, grayscaleValue, 1.0);\n"
												"}\0";

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool initialize(const Window & window);
	void draw() const;

private:
	std::vector<GLfloat> m_quadVertices;

	GLuint m_shader;
};