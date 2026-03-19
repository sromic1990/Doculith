#include "GlfwWindow.h"
#include "../ApplicationConfig.h"

namespace Doculith
{
	GlfwWindow::GlfwWindow(const ApplicationConfig& config)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_window = glfwCreateWindow(
			config.windowWidth,
			config.windowHeight,
			config.windowTitle.c_str(),
			nullptr, nullptr
		);

		if (!m_window)
		{
			throw std::runtime_error("glfwCreateWindow() failed!");
		}

		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(config.vsync ? 1 : 0);

	}

	GlfwWindow::~GlfwWindow()
	{
		if (m_window)
		{
			glfwDestroyWindow(m_window);
		}
	}
}
