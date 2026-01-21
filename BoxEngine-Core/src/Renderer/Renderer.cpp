#include "Renderer/Renderer.h"

#include <glad/glad.h>
#include "Utils/Scene.h"

Renderer::Renderer(Window* target)
{
	attach(target);
}

void Scene::update()
{

}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::attach(Window* target)
{
	m_target = target;
}
