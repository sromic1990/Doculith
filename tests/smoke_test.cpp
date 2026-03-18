#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test infrastructure is operational","[smoke]")
{
    //This tests existence and passage proves:
    //- Catch2 compiled and linked correctly.
    //- CTest discovered this test via catch_discover_tests.
    //- The test binary executes and exits cleanly.
    //A green tick here is a meaningful assertion in it's own right.
    REQUIRE(true);
}

TEST_CASE("Basic arithmetic holds", "[smoke]")
{
    //Maths works! Good!
    REQUIRE(1 + 1 == 2);
    REQUIRE(6 * 7 == 42);
    REQUIRE(0 != 1);
}

//--------------------ApplicationConfig default--------------------
#include "Application.h"

TEST_CASE("ApplicationConfig has sensible defaults", "[application]")
{
    Doculith::ApplicationConfig config;

    REQUIRE(config.windowWidth == 1280);
    REQUIRE(config.windowHeight == 800);
    REQUIRE(config.vsync == true);
    REQUIRE(config.windowTitle == Doculith::strings::kAppName);
    REQUIRE(config.uiScale == 1.0f);
}

TEST_CASE("ApplicationConfig is configurable", "[application]")
{
    Doculith::ApplicationConfig config;
    config.windowWidth = 1920;
    config.windowHeight = 1080;
    config.windowTitle = "My Doculith";
    config.vsync = false;

    REQUIRE(config.windowWidth == 1920);
    REQUIRE(config.windowHeight == 1080);
    REQUIRE(config.windowTitle == "My Doculith");
    REQUIRE(config.vsync == false);
}
