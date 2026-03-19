// src/platform/ImGuiContextGuard.h
// RAII wrapper for the ImGui context and it's GLFW + OPENGL3 backends
//Requires a GlfwWindow to have been constructed and made current first

#pragma once

// ImGui must be initialized before GLFW
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

namespace Doculith
{
	class ImGuiContextGuard
	{
	public:
		explicit ImGuiContextGuard(GLFWwindow* window);
		~ImGuiContextGuard();

		ImGuiContextGuard(const ImGuiContextGuard&)					= delete;
		ImGuiContextGuard& operator=(const ImGuiContextGuard&)		= delete;
		ImGuiContextGuard(const ImGuiContextGuard&&)				= delete;
		ImGuiContextGuard& operator=(const ImGuiContextGuard&&)		= delete;

	};
}
