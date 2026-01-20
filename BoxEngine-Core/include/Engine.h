#pragma once

#include <string>

#include "Core/Window.h"
#include "Renderer/Renderer.h"

class Engine
{
public:
	// shouldn't ever have an empty constructor but will make one anyways
	Engine();
	Engine(unsigned int width, unsigned int height, const std::string& title);
	~Engine();
private:
	Window*		m_window	= nullptr;
	Renderer*	m_renderer	= nullptr;

	void init();
};