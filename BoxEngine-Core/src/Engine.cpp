#include "Engine.h"

Engine::Engine()
{
	Engine::init();
	m_window = (Window*)new Window(800, 800, "Box Engine Game");
}

Engine::Engine(unsigned int width, unsigned int height, const std::string& title)
{
	Engine::init();
	m_window = (Window*)new Window(width, height, title);
	m_renderer = (Renderer*)new Renderer(m_window);
}

Engine::~Engine()
{
	delete m_window;
	delete m_renderer;
	glfwTerminate();
}

void Engine::init()
{
	
}
