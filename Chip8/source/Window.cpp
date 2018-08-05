#include "Chip8/Emulator/Window.hpp"

#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

#include <iostream>

Window::Window()
	: m_windowHandle(nullptr)
	, m_windowWidth(0)
	, m_windowHeight(0)
{
}

Window::~Window()
{
	glfwDestroyWindow(m_windowHandle);
	glfwTerminate();
}

bool Window::create(const char * title, int width, int height, int glVersionMajor, int glVersionMinor)
{
	if (!glfwInit())
		return false;

	glfwSetErrorCallback(errorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_windowHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (m_windowHandle == nullptr)
		return false;

	glfwMakeContextCurrent(m_windowHandle);

	// TODO: set keyboard callback here

	// Load OpenGL functions using GL3W
	if (gl3wInit())
	{
		printf("Failed to initialize OpenGL.\n");
		return false;
	}

	// This OpenGL version is not supported
	if (!gl3wIsSupported(glVersionMajor, glVersionMinor))
	{
		printf("OpenGL %i.%i is not supported on this machine.\n", glVersionMajor, glVersionMinor);
		return false;
	}

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	return true;
}

void Window::quit() const
{
	glfwSetWindowShouldClose(m_windowHandle, GLFW_TRUE);
}

void Window::pollKeyboard() const
{
	glfwPollEvents();
}

void Window::display() const
{
	glfwSwapBuffers(m_windowHandle);
}

int Window::getWidth() const
{
	return m_windowWidth;
}

int Window::getHeight() const
{
	return m_windowHeight;
}

void Window::getFramebufferDimensions(int & widthStorage, int & heightStorage) const
{
	glfwGetFramebufferSize(m_windowHandle, &widthStorage, &heightStorage);
}

void Window::errorCallback(int error, const char *description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void Window::keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
}
