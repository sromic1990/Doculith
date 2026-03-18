#include "Application.h"

#include <format>
#include <iostream>
#include <string>

int main()
{
    try
    {
        Doculith::ApplicationConfig config;
        config.windowWidth = 1280;
        config.windowHeight = 800;
        config.windowTitle = Doculith::strings::kAppTagline;
        config.vsync = true;

        Doculith::Application app(config);
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << std::format("{} {}\n", Doculith::strings::kErrFatal, e.what());
        return 1;
    }

    return 0;
}