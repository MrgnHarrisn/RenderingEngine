#pragma once

#include <string>

#include "Core/Window.h"
#include "Renderer/Renderer.h"

class Engine
{
public:
	/// <summary>
	/// Empty constructor for some reason
	/// </summary>
	Engine();

	/// <summary>
	/// Basic constructor
	/// </summary>
	/// <param name="width">Width of window</param>
	/// <param name="height">Height of window</param>
	/// <param name="title">Title of window</param>
	Engine(unsigned int width, unsigned int height, const std::string& title);

	/// <summary>
	/// Run the engine
	/// </summary>
	void run();

	/// <summary>
	/// Initialize Engine
	/// </summary>
	void init();

	/// <summary>
	/// Destructor for the window
	/// </summary>
	~Engine();
private:
	Window*		m_window	= nullptr;
	Renderer*	m_renderer	= nullptr;
};