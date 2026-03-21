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

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");

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
