//Top-level application orchestrator.Owns the window, the ImGui context,
//and the application state. Drive the render loop.

#pragma once

//Include ImGUI header before GLFW to avoid OpenGL header ordering conflicts
#include "AppStrings.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>

namespace Doculith
{
	//---------------------Application Config---------------------------------
	//All tunable parameters for the application window and UI.
	//Passed to Application at construction time.
	//No buried Magic Numbers.
	struct ApplicationConfig
	{
		int windowWidth = 1280;
		int windowHeight = 800;
		std::string windowTitle = strings::kAppName;
		float uiScale = 1.0f;
		bool vsync = true;
	};
	//---------------------Application Config---------------------------------


	//---------------------Application----------------------------------------
	//Owns the complete application lifetime:
	// - GLFW context and window (RAII)
	// - ImGui context and backends (RAII)
	// - Application state
	// - Render loop
	//
	// Non-copyable, non-movable. One application per process.

	class Application
	{
	public:
		explicit Application(const ApplicationConfig& config = {});
		~Application();

		//Not copyable or movable.
		Application(const Application&)				= delete;
		Application& operator= (const Application&) = delete;
		Application(Application&&)					= delete;
		Application& operator= (Application&&)		= delete;

		//Enters the render loop.Returns when the user closes the window.
		void run();

	private:
		//Initialization called from constructor in this order
		void initGlfw();
		void initWindow();
		void initImGui();
		void applyTheme();
		void applyFonts();

		//Per-frame work
		void beginFrame();
		void renderUI();
		void endFrame();

		//Ui Panels
		void renderMainPanel();

		//Cleanup called from destructor, in reverse init order
		void shutdownImGui();
		void shutdownGlfw();

		ApplicationConfig m_config;
		GLFWwindow* m_window = nullptr;

		bool m_showDemoWindow = false;
	};
	//---------------------Application----------------------------------------
}