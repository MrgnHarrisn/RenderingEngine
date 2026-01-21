#pragma once

#include "Core/Window.h"
#include <cerrno>

class Renderer
{
public:
	Renderer(Window* target);
	void setClearColor();
	void render();
private:
	/// <summary>
	/// Attach renderer to the window
	/// </summary>
	/// <param name="target">Window to render to</param>
	void attach(Window* target);

	Window* m_target = nullptr;
};