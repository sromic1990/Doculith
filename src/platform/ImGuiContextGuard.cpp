#include "ImGuiContextGuard.h"
#include <stdexcept>

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

		if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
		{
			ImGui::DestroyContext();
			throw std::runtime_error("Failed to initialize ImGui GLFW backend.");
		}

		if (!ImGui_ImplOpenGL3_Init("#version 330 core"))
		{
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
			throw std::runtime_error("Failed to initialize ImGui OpenGL3 backend.");
		}
	}

	ImGuiContextGuard::~ImGuiContextGuard()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}
