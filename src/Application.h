// src/Application.h
// Single responsibility: wire PlatformContext,AppModel and Ui together.

#pragma once
#include "ApplicationConfig.h"
#include "platform/PlatformContext.h"
#include "AppModel.h"
#include "Ui.h"
#include "UiEvents.h"

namespace Doculith
{
	class Application
	{
	public:
		explicit Application(const ApplicationConfig& config = {});
		~Application() = default;

		//Not copyable or movable.
		Application(const Application&)				= delete;
		Application& operator= (const Application&) = delete;
		Application(Application&&)					= delete;
		Application& operator= (Application&&)		= delete;

		//Enters the render loop.Returns when the user closes the window.
		void run();

	private:
		void dispatch(const UiEvents& events);

		ApplicationConfig	m_config;
		PlatformContext		m_platform;
		AppModel			m_model;
		Ui					m_ui;
	};
}