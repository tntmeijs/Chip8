#include "Chip8/Emulator/Window.hpp"
#include "Chip8/Utility/DataTypes.hpp"

#include "GL/gl3w.h"
#include "GLFW/glfw3.h"

#include <iostream>

byte *Window::m_hexKeyPad;

Window::Window()
	: m_windowHandle(nullptr)
	, m_windowWidth(0)
	, m_windowHeight(0)
{
	m_hexKeyPad = new byte[16];
	for (byte i = 0; i < 16; ++i)
		m_hexKeyPad[i] = 0;
}

Window::~Window()
{
	delete[] m_hexKeyPad;
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

	glfwSetKeyCallback(m_windowHandle, keyboardCallback);

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

	m_windowWidth = width;
	m_windowHeight = height;

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
	// GLFW press and release nicely map to 0 and 1
	byte keyState = action;

	switch (key)
	{
	case GLFW_KEY_1:
		m_hexKeyPad[0x1] = action;
		break;

	case GLFW_KEY_2:
		m_hexKeyPad[0x2] = action;
		break;

	case GLFW_KEY_3:
		m_hexKeyPad[0x3] = action;
		break;

	case GLFW_KEY_4:
		m_hexKeyPad[0xC] = action;
		break;

	case GLFW_KEY_Q:
		m_hexKeyPad[0x4] = action;
		break;

	case GLFW_KEY_W:
		m_hexKeyPad[0x5] = action;
		break;

	case GLFW_KEY_E:
		m_hexKeyPad[0x6] = action;
		break;

	case GLFW_KEY_R:
		m_hexKeyPad[0xD] = action;
		break;

	case GLFW_KEY_A:
		m_hexKeyPad[0x7] = action;
		break;

	case GLFW_KEY_S:
		m_hexKeyPad[0x8] = action;
		break;

	case GLFW_KEY_D:
		m_hexKeyPad[0x9] = action;
		break;

	case GLFW_KEY_F:
		m_hexKeyPad[0xE] = action;
		break;

	case GLFW_KEY_Z:
		m_hexKeyPad[0xA] = action;
		break;

	case GLFW_KEY_X:
		m_hexKeyPad[0x0] = action;
		break;

	case GLFW_KEY_C:
		m_hexKeyPad[0xB] = action;
		break;

	case GLFW_KEY_V:
		m_hexKeyPad[0xF] = action;
		break;

	default:
		break;
	}
}
