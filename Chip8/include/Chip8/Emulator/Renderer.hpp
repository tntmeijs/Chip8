#pragma once

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

};