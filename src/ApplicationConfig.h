#pragma once

#include <string>
#include "AppStrings.h"

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
		float uiScale = 1.0f; //set at runtime from glfwGetWindowContentScale
		bool vsync = true;
	};
	//---------------------Application Config---------------------------------
}
