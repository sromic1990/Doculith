// src/platform/GlfwContext.h

//RAII wrapper for the GLFW library itself.
// glfwInit() is called in the constructor
// glfwTerminate is called in the destructor
// MUST be constructed before any other GLFW type

#pragma once
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Doculith
{
	class GlfwContext
	{
	public:
		GlfwContext()
		{
			if (!glfwInit())
			{
				throw std::runtime_error("glfwInit() failed!");
			}
		}

		~GlfwContext() { glfwTerminate(); }

		GlfwContext(const GlfwContext&) = delete;
		GlfwContext& operator=(const GlfwContext&) = delete;
		GlfwContext(const GlfwContext&&) = delete;
		GlfwContext& operator=(const GlfwContext&&) = delete;
	};
}
