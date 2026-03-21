// src/Ui.h
// All DearImgui rendering lives here. No platform dependencies.
// Single Responsibility: translate AppModel into pixel, report UiEvents

#pragma once
#include "AppModel.h"
#include "UiEvents.h"

namespace Doculith
{
	class Ui
	{
	public:
		Ui()						= default;
		~Ui()						= default;

		Ui(const Ui&)				= delete;
		Ui& operator=(const Ui&)	= delete;
		Ui(Ui&&)					= delete;
		Ui& operator=(Ui&&)			= delete;

		// Called once after the ImGui context is created
		void applyTheme();
		void applyFonts(float uiScale);

		// Called once per frame. Renders the UI and returns this frame's events.
		// render() takes AppModel& because ImGui::InputText writes directly into model.outputPathBuf
		// All other mutations flow through the returned UiEvents.
		UiEvents render(AppModel& model);

	private:
		void renderDockSpace();
		void renderMainPanel(UiEvents& events, AppModel& model);
		void renderFileList(UiEvents& events, const AppModel& model, float panelWidth);
		void renderControlsPanel(UiEvents& events, AppModel& model);

		//Demo Window state is internal to the UI layer, it is not business logic
		bool m_showDemoWindow = false;

	};
}