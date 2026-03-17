cmake_minimum_required(VERSION 4.2)

project (Doculith
    VERSION 0.1.0
    DESCRIPTION "A DOCX to PDF merger with a modern GUI"
    LANGUAGES CXX)

#-------C++ Standard-------------------------
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

#-------Output Directories-------------------
# All compiled executables and libraries go to build/binand build/lib.
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#-------Suppress macOS OpenGL deprecation warnings-------------------------
# Apple deprecated OpenGL in 2018, but it still  works.So this will suppress the noise
if(APPLE)
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif()

#-------Dependencies----------------------------
# All external libraries are declared and fetched in cmake/Dependencies.cmake
include(cmake/Dependencies.cmake)

#-------Main Executable-------------------------
add_executable(doculith
    src/main.cpp
)

target_include_directories(doculith PRIVATE
    src
)

target_link_libraries(doculith PRIVATE
    imgui_lib
    nfd
)

#------Compiler Warnings-------------------------
if(MSVC)
    target_compile_options(doculith PRIVATE
        /W4
        /WX
        /utf-8
        /wd4100 # unreferenced formal parameter
        /wd4201 # nonstandard extension used: nameless struct/union
    )
else()
    # Clang and GCC
    target_compile_options(doculith PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wpedantic
    )
endif()

#------Tests-------------------------------------
enable_testing()
include (CTest)
add_subdirectory(tests)