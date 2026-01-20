#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Window
{
public:
	/// <summary>
	/// Window Constructor
	/// </summary>
	/// <param name="width">Width of the window</param>
	/// <param name="height">Height of the window</param>
	/// <param name="title">Title of the window</param>
	Window(unsigned int width, unsigned int height, const std::string& title);

	/// <summary>
	/// Returns the target window
	/// </summary>
	/// <returns></returns>
	GLFWwindow* getWindow();

	/// <summary>
	/// Should the window close
	/// </summary>
	/// <returns></returns>
	bool shouldClose();

	/// <summary>
	/// Swap buffers
	/// </summary>
	void swapBuffers();

	/// <summary>
	/// Poll events
	/// </summary>
	void pollEvents();
private:
	GLFWwindow* m_window = nullptr;
	unsigned int m_width, m_height;

	// resize function
	void resize_callback();
};