#include "Renderer/Renderer.h"

#include <glad/glad.h>
#include "Utils/Scene.h"

Renderer::Renderer(Window* target)
{
	attach(target);
}

void Renderer::setClearColor(glm::vec3 color)
{
	glClearColor(color.x, color.y, color.z, 1.0f);
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
