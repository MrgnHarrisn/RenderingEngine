#include "Renderer/Renderer.h"

#include <glad/glad.h>

Renderer::Renderer(Window* target)
{
	attach(target);
}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::attach(Window* target)
{
	m_target = target;
}
