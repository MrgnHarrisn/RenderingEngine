#include "Engine.h"

Engine::Engine()
{
	m_window = (Window*)new Window(800, 800, "Box Engine Game");
}

Engine::Engine(unsigned int width, unsigned int height, const std::string& title)
{
	m_window = (Window*)new Window(width, height, title);
	m_renderer = (Renderer*)new Renderer(m_window);
	m_world = (World*)new World();
}

Engine::~Engine()
{
	delete m_window;
	delete m_renderer;
	delete m_world;
	glfwTerminate();
}

void Engine::init()
{
	// do something here?
	// read in main.berp file to gain data etc.
	m_world->generateTerrain();
#ifndef NDEBUG
	testModelLoading();
#endif // !NDEBUG
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