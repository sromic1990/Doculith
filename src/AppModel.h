// src/AppModel.h
// All business state lives here. Zero UI dependencies.

#pragma once
#include "DocumentQueue.h"
#include <algorithm>
#include <cstring>
#include <string>

namespace Doculith
{
	struct AppModel
	{
		DocumentQueue queue;

		// Output path buffer, owned by AppModel, written to by ImGui::InputText.
		// Ui::render() takes AppModel& (non-const) specifically to allow this.
		char outputPathBuf[512] = {};

		// Human readable status message shown below the control panel.
		std::string addFeedback;

		// Called  by Application::dispatch() after files are successfully added.
		// Populate outputPathBuf from DocumentQueue::suggestedOutputPath()
		void refreshOutputPath()
		{
			const auto suggested = queue.suggestedOutputPath();
			if (suggested.has_value() && outputPathBuf[0] == '\0')
			{
				const auto str = suggested->string();
				const auto len = std::min(str.size(), sizeof(outputPathBuf) - 1u);
				std::memcpy(outputPathBuf, str.data(), len);
				outputPathBuf[len] = '\0';
			}
		}
	};
}