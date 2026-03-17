//Include ImGUI header before GLFW to avoid OpenGL header ordering conflicts
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "Application.h"

#include <cstdio>
#include <format>
#include <stdexcept>
#include <string>

namespace
{
    //RAII wrapper for the GLFW library itself
    class GlfwContext
    {
        public:
            GlfwContext()
            {
                if(!glfwInit())
                {
                    throw std::runtime_error("glfwInit failed! Cannot create window!");
                }
            }

            ~GlfwContext()
            {
                glfwTerminate();
            }

            //GLFW is a process-wide singleton. Copying makes no sense
            GlfwContext (const GlfwContext&) = delete;
            GlfwContext& operator= (const GlfwContext&) = delete;
    };

    //RAII wrapper for the GLFWwindow*
    class GlfwWindow
    {
        public:
            GlfwWindow(int width, int height, const std::string& title)
            {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        #endif

                m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
                if(!m_window)
                {
                    throw std::runtime_error("glfwCreateWindow failed!");
                }

                glfwMakeContextCurrent(m_window);
                glfwSwapInterval(1); //Vsync: tying frame rate to monitor refresh
            }

            ~GlfwWindow()
            {
                if(m_window)
                    glfwDestroyWindow(m_window);
            }

            GlfwWindow(const GlfwWindow&) = delete;
            GlfwWindow& operator=(const GlfwWindow&) = delete;
            
            [[nodiscard]] GLFWwindow* get() const noexcept { return m_window; }
            [[nodiscard]] bool shouldClose() const noexcept
            {
                return glfwWindowShouldClose(m_window) != 0;
            }

        private:
            GLFWwindow* m_window = nullptr;
    };

    //RAII wrapper for Dear ImGui context and it's platform backends
    class ImGuiContextGuard
    {
        public:
            explicit ImGuiContextGuard(GLFWwindow* window)
            {
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO& io =ImGui::GetIO();
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

                ImGui::StyleColorsDark();

                ImGui_ImplGlfw_InitForOpenGL(window, true);
                ImGui_ImplOpenGL3_Init("#version 330 core");
            }

            ~ImGuiContextGuard()
            {
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext();
            }

            ImGuiContextGuard(const ImGuiContextGuard&) = delete;
            ImGuiContextGuard& operator=(const ImGuiContextGuard&) = delete;
    };

    //renderFrame builds and usbmits one frame, called every loop iteration.
    //Responsible for: new frame -> UI description -> render -> present.
    void renderFrame(GLFWwindow* window)
    {
        //Tell both backends that a new frame is starting
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Describe the UI for this frame
        {
            ImGui::SetNextWindowSize(ImVec2(500.0f, 240.0f), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowPos(ImVec2(390.0f, 240.0f), ImGuiCond_FirstUseEver);
            ImGui::Begin("Doculith - Document Merger");

            ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "Build Successfull");
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("GLFW + OpenGL + Dear ImGUI are all wired up!");
            ImGui::Spacing();
            ImGui::TextDisabled("Diaplay: %.0f x %.0f FPS: %.1f",
                ImGui::GetIO().DisplaySize.x,
                ImGui::GetIO().DisplaySize.y,
                ImGui::GetIO().Framerate);
            ImGui::End();
        }

        //Finalise ImGui's draw command lists.
        ImGui::Render();

        //Clear the framebuffer and render ImGui's draw data.
        int display_w = 0;
        int display_h = 0;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.12f, 0.12f, 0.13f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

int main()
{
    try
    {
        Doculith::ApplicationConfig config;
        config.windowWidth = 1280;
        config.windowHeight = 800;
        config.windowTitle = Doculith::AppStrings::AppTagline;
        config.vsync = true;

        Doculith::Application app(config);
        app.run();
    }
    catch (const std::exception& e)
    {
        auto errorFormat = std::format("{} {}", Doculith::AppStrings::ErrFatal, e.what());
        std::fprintf(stderr, errorFormat.c_str());
        return 1;
    }

    return 0;
}