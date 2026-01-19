#include "Core/Window.h"

#include <glad/glad.h>

using namespace std;

Window::Window(unsigned int width, unsigned int height, string& title)
{
	m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if (m_window == nullptr)
	{
		cout << "Error creating GLFW window" << endl;
	}
}

GLFWwindow* Window::getWindow()
{
	return m_window;
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

void Window::pollEvents()
{
	glfwPollEvents();
}

void Window::resize_callback()
{
	glViewport(0, 0, m_width, m_height);
}
