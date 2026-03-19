// src/Platform/GlfwWindow.h
// RAII wrapper for GLFWwindow*
// Configures OpenGL hints, creates the window, makes context current
// Enables vsync, and sets the window icon color
// Requires a GlfwContext to have been constructed first

#pragma once
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

namespace Doculith
{
	struct ApplicationConfig;

	class GlfwWindow
	{
	public:
		explicit GlfwWindow(const ApplicationConfig& config);
		~GlfwWindow();

		GlfwWindow(const GlfwWindow&) = delete;
		GlfwWindow& operator=(const GlfwWindow&) = delete;
		GlfwWindow(const GlfwWindow&&) = delete;
		GlfwWindow& operator=(const GlfwWindow&&) = delete;

		[[nodiscard]] GLFWwindow* get() const noexcept { return m_window; }
		[[nodiscard]] bool shouldClose() const noexcept
		{
			return glfwWindowShouldClose(m_window) != 0;
		}

	private:
		GLFWwindow* m_window = nullptr;
	};
}
