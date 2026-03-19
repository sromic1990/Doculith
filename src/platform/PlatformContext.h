// src/platform/PlatformContext.hpp
//
// Composes GlfwContext, GlfwWindow, and ImGuiContextGuard.
// Manages the complete platform lifecycle
// Single responsibility: own the OS window, GL context and ImGui context

#pragma once

//ImGuiContextGuard include imgui.h before GLFW, correct ordering guaranteed
#include "ImGuiContextGuard.h"
#include "GlfwContext.h"
#include "GlfwWindow.h"
#include "../ApplicationConfig.h"

namespace Doculith
{
	class PlatformContext
	{
	public:
		explicit PlatformContext(const ApplicationConfig& config);
		~PlatformContext() = default;

		PlatformContext(const PlatformContext&)					= delete;
		PlatformContext& operator=(const PlatformContext&)		= delete;
		PlatformContext(PlatformContext&&)						= delete;
		PlatformContext& operator=(PlatformContext&&)			= delete;

		[[nodiscard]] bool shouldClose() const noexcept;
		[[nodiscard]] float uiScale() const noexcept;

		void pollEvents();
		void beginFrame();
		void endFrame();

	private:
		// DECLARATION ORDER IS THE CONSTRUCTION ORDER (ISO 12.6.2)
		//
		// C++ constructs members in the same order they are declared here.
		// Destruction is just the reverse.
		//
		// m_glfw MUST be first: glfwInit() must succeed before any window call.
		// m_window MUST be second: window must exist before imgui backends init.
		// m_imgui MUST be third: ImGui context must outlive window and GLFW
		//
		//Reordering these declarations may result in application crash
		GlfwContext m_glfw;
		GlfwWindow m_window;
		ImGuiContextGuard m_imgui;

		float m_uiScale = 1.0f;
	};
}
