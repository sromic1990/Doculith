include (FetchContent)

#Tell Cmake to show us that it is fetching
set(FETCCHCONTENT_QUIET FALSE)

#-------GLFW-------------------------
message (STATUS "Fetching GLFW 3.4...")

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

#Disable GLFW subtargets
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_makeAvailable(glfw)

#-------Dear ImGui-------------------------
message (STATUS "Fetching Dear ImGUI v1.92.6...")

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.6
    GIT_SHALLOW TRUE
)
FetchContent_makeAvailable(imgui)

add_library(imgui_lib STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_lib PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_lib PUBLIC glfw)

#Platform-specific opengl linkage
if(WIN32)
    target_link_libraries(imgui_lib PUBLIC opengl32)
elseif(APPLE)
    find_library(OPENGL_FRAMEWORK OpenGL REQUIRED)
    target_link_libraries(imgui_lib PUBLIC ${OPENGL_FRAMEWORK})
else()
    find_package(OpenGL required)
    target_link_libraries(imgui_lib PUBLIC OpenGL::GL)
endif()

#-------Native File Dialog-------------------------
message(STATUS "Fetching NFD-Extended v1.3.0...")

FetchContent_Declare(
    nfd
    GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
    GIT_TAG v1.3.0
    GIT_SHALLOW TRUE
)
FetchContent_makeAvailable(nfd)

#-------Catch2--------------------------------------
message(STATUS "Fetching Catch2 v3.13.0...")

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.13.0
    GIT_SHALLOW TRUE
)
FetchContent_makeAvailable(catch2)