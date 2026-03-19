#include "PlatformContext.h"
#include <algorithm> // std::max

namespace Doculith
{
	PlatformContext::PlatformContext(const ApplicationConfig& config)
		: m_glfw{},
		m_window{ config },
		m_imgui{ m_window.get() }
	{
		// Detect the display scale factor
		// 1 on standard screens, 2.0 on Retina/HiDPI
		//This must be queried AFTER the window has been created.
		float scaleX = 1.0f, scaleY = 1.0f;
		glfwGetWindowContentScale(m_window.get(), &scaleX, &scaleY);
		m_uiScale = std::max(scaleX, scaleY);
	}
	
	bool PlatformContext::shouldClose() const noexcept
	{
		return m_window.shouldClose();
	}

	float PlatformContext::uiScale() const noexcept
	{
		return m_uiScale;
	}

	void PlatformContext::pollEvents()
	{
		glfwPollEvents();
	}

	void PlatformContext::beginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void PlatformContext::endFrame()
	{
		ImGui::Render();

		int w = 0, h = 0;
		glfwGetFramebufferSize(m_window.get(), &w, &h);
		glViewport(0, 0, w, h);
		glClearColor(0.07f, 0.07f, 0.09f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_window.get());
	}
}
