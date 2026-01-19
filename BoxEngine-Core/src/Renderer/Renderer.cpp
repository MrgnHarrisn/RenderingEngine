#include "Renderer/Renderer.h"

#include <glad/glad.h>

Renderer::Renderer(GLFWwindow* target)
{
	attach(target);
}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::attach(GLFWwindow* target)
{
	m_target = target;
}
