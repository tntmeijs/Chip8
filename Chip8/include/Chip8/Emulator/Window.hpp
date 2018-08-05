#pragma once

// Forward declarations
struct GLFWwindow;

class Window
{
public:
	Window();
	~Window();

	bool create(const char * title, int width, int height, int glVersionMajor = 3, int glVersionMinor = 3);
	
	void quit() const;
	void pollKeyboard() const;

	int getWidth() const;
	int getHeight() const;

private:
	static void errorCallback(int error, const char *description);
	static void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

private:
	GLFWwindow *m_windowHandle;

	int m_windowWidth;
	int m_windowHeight;
};