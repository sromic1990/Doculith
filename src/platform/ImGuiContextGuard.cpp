#include "ImGuiContextGuard.h"
#include <stdexcept>
#include <nfd.hpp>

namespace Doculith
{
	ImGuiContextGuard::ImGuiContextGuard(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.IniFilename = nullptr;

		bool glfwInitOk = ImGui_ImplGlfw_InitForOpenGL(window, true);
		if (!glfwInitOk)
		{
			ImGui::DestroyContext();
			throw std::runtime_error("Failed to initialize ImGui GLFW backend.");
		}

		bool gl3InitOk = ImGui_ImplOpenGL3_Init("#version 330 core");
		if (!gl3InitOk)
		{
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
			throw std::runtime_error("Failed to initialize ImGui OpenGL3 backend.");
		}
		// NFD requires initialization after the window and context exist
		NFD::Init();
	}

	ImGuiContextGuard::~ImGuiContextGuard()
	{
		NFD::Quit();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}
