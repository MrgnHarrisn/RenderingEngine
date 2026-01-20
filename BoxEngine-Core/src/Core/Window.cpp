#include "Core/Window.h"

#include <glad/glad.h>

using namespace std;

Window::Window(unsigned int width, unsigned int height, const string& title) :
	m_height(height), m_width(width)
{

	// Set the window hints (will this even work for the rest of the project?)
	if (!glfwInit()) {
		printf("Failed to initialize GLAD\n");
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, title.c_str(), NULL, NULL);

	if (m_window == nullptr)
	{
		cout << "Error creating GLFW window" << endl;
		exit(-1);
	}

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
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
