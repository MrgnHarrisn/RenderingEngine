#pragma once

#include <GLFW/glfw3.h>
#include <cerrno>

class Renderer
{
public:
	Renderer(GLFWwindow* target);

	void render();
private:
	/// <summary>
	/// Attach renderer to the window
	/// </summary>
	/// <param name="target">Window to render to</param>
	void attach(GLFWwindow* target);

	GLFWwindow* m_target = nullptr;
};