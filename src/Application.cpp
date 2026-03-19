#include "Application.h"

namespace Doculith
{
	Application::Application(const ApplicationConfig& config)
		:	m_config {config},
			m_platform {config},
			m_ui {}
	{
		m_ui.applyTheme();
		m_ui.applyFonts(m_platform.uiScale());
	}

	void Application::run()
	{
		while (!m_platform.shouldClose())
		{
			m_platform.pollEvents();
			m_platform.beginFrame();

			const UiEvents events = m_ui.render(m_model);
			dispatch(events);

			m_platform.endFrame();
		}
	}

	void Application::dispatch(const UiEvents& events)
	{
		(void)events;
	}
}
