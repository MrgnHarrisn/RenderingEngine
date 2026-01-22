#include "Engine.h"

Engine::Engine()
{
	m_window = (Window*)new Window(800, 800, "Box Engine Game");
}

Engine::Engine(unsigned int width, unsigned int height, const std::string& title)
{
	m_window = (Window*)new Window(width, height, title);
	m_renderer = (Renderer*)new Renderer(m_window);
}

void Engine::setClearColor(float x, float y, float z)
{
	m_renderer->setClearColor({ x, y, z });
}

Engine::~Engine()
{
	delete m_window;
	delete m_renderer;
	glfwTerminate();
}

void Engine::init()
{
	// do something here?
}

void Engine::run()
{
	while (!m_window->shouldClose())
	{
		m_window->pollEvents();
		m_renderer->render();
		m_window->swapBuffers();
	}
}