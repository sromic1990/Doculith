# tests/CmakeLists.txt

add_executable(doculith_tests
    smoke_test.cpp
)

target_link_libraries(doculith_tests PRIVATE
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(doculith_tests)