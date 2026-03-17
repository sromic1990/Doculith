# tests/CmakeLists.txt

add_executable(doculith_tests
    smoke_test.cpp
)

target_include_directories(doculith_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(doculith_tests PRIVATE
    Catch2::Catch2WithMain
    imgui_lib
    nfd
)

include(Catch)
catch_discover_tests(doculith_tests)