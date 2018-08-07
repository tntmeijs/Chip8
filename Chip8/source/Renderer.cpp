#include "Chip8/Emulator/Renderer.hpp"
#include "Chip8/Emulator/Window.hpp"

#include "GL/gl3w.h"

#include <iostream>

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_quadVAO);
	glDeleteBuffers(1, &m_quadVBO);
	glDeleteProgram(m_shader);
}

bool Renderer::initialize(const Window & window)
{
	// Set the framebuffer to cover the complete window
	int width = 0;
	int height = 0;
	window.getFramebufferDimensions(width, height);

	glViewport(0, 0, width, height);
	glClearColor(0.223f, 0.8f, 0.8f, 1.0f);	// This color is called "teal"
	
	if (!setupShaders())
		return false;

	// Set the texture index
	glUseProgram(m_shader);
	glUniform1i(glGetUniformLocation(m_shader, "textureID"), 0);
	glUseProgram(0);

	setupTexture(width, height);
	setupQuad();

	return true;
}

void Renderer::draw() const
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_shader);
	
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glBindVertexArray(m_quadVAO);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
}

bool Renderer::setupShaders()
{
	const GLchar *const vertexShaderSourceCode =	"#version 330 core\n"
													"layout(location = 0) in vec2 position;\n"
													"layout(location = 1) in vec2 texCoord;\n"

													"out vec2 uv;\n"

													"void main() {\n"
														"uv = texCoord;\n"
														"gl_Position = vec4(position, 0.0, 1.0);\n"
													"}\0";

	const GLchar *const fragmentShaderSourceCode =	"#version 330 core\n"

													"in vec2 uv;\n"
													"out vec4 fragColor;\n"

													"uniform sampler2D textureID;\n"

													"void main() {\n"
														"float grayscaleValue = texture(textureID, uv).r;\n"
														"fragColor = vec4(grayscaleValue, grayscaleValue, grayscaleValue, 1.0);\n"
													"}\0";

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

		GLchar * log = new GLchar[maxLength];
		glGetProgramInfoLog(m_shader, maxLength, &maxLength, &log[0]);

		printf("Failed to link the program.\n");
		printf("Error: %s.\n\n", log);

		delete[] log;

		glDeleteProgram(m_shader);

		return false;
	}

	return true;
}

void Renderer::setupTexture(int width, int height)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::setupQuad()
{
	GLfloat vertices[] =
	{
		// Position		// Texture coordinate
		 1.0f,  1.0f,	1.0f, 1.0f,
		-1.0f,  1.0f,	0.0f, 1.0f,
		-1.0f, -1.0f,	0.0f, 0.0f,

		-1.0f, -1.0f,	0.0f, 0.0f,
		 1.0f, -1.0f,	1.0f, 0.0f,
		 1.0f,  1.0f,	1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &m_quadVBO);

	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid *)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid *)(sizeof(GLfloat) * 2));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
