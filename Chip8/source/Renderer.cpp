#include "Chip8/Emulator/Renderer.hpp"
#include "Chip8/Emulator/Window.hpp"

#include "GL/gl3w.h"

#include <vector>

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	glDeleteProgram(m_shader);
}

bool Renderer::initialize(const Window & window)
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

	GLint successFlag = 0;

	// Create the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSourceCode, 0);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successFlag);
	
	if (successFlag == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		GLchar * log = new GLchar[maxLength];
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &log[0]);

		printf("Failed to compile the vertex shader.\n");
		printf("Error: %s.\n\n", log);

		delete[] log;

		glDeleteShader(vertexShader);

		return false;
	}

	// Create the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, 0);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFlag);

	if (successFlag == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		GLchar * log = new GLchar[maxLength];
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &log[0]);

		printf("Failed to compile the fragment shader.\n");
		printf("Error: %s.\n\n", log);

		delete[] log;

		glDeleteShader(fragmentShader);

		return false;
	}

	// Create the shader program
	m_shader = glCreateProgram();
	glAttachShader(m_shader, vertexShader);
	glAttachShader(m_shader, fragmentShader);

	glLinkProgram(m_shader);

	glDetachShader(m_shader, vertexShader);
	glDetachShader(m_shader, fragmentShader);

	// No need to keep the shaders around anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(m_shader, GL_LINK_STATUS, &successFlag);

	if (successFlag == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_shader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(m_shader, maxLength, &maxLength, &errorLog[0]);

		printf("Failed to link the program.\n");
		printf("Error: %s.\n\n", errorLog);

		glDeleteProgram(m_shader);

		return false;
	}

	// Since this application will only use a single shader EVER, it is possible to set it only once in here.
	// Not the best way to do it, but this project is not about writing a beautiful renderer...
	glUseProgram(m_shader);

	return true;
}

void Renderer::draw() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}
