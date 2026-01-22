#pragma once

#include "Core/Window.h"
#include <cerrno>
#include <glm/glm.hpp>

class Renderer
{
public:

	/// <summary>
	/// Create renderer and attach it to a window
	/// </summary>
	/// <param name="target">Target window to render to</param>
	Renderer(Window* target);

	/// <summary>
	/// Set the clear color
	/// </summary>
	/// <param name="color">Color</param>
	void setClearColor(glm::vec3 color);

	/// <summary>
	/// Render something
	/// </summary>
	void render();
private:
	/// <summary>
	/// Attach renderer to the window
	/// </summary>
	/// <param name="target">Window to render to</param>
	void attach(Window* target);

	Window* m_target = nullptr;
};