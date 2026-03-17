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

TEST_CASE("Basic arithmatic holds", "[smoke]")
{
    //Maths works! Good!
    REQUIRE(1 + 1 == 2);
    REQUIRE(6 * 7 == 42);
    REQUIRE(0 != 1);
}
