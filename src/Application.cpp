#include "Application.h"

#include <cstdio>
#include <stdexcept>
#include <format>

namespace Doculith
{
    //----------------------Constructor & Destructor-----------------------------------
	Application::Application(const ApplicationConfig& config)
	{
		//Initialize in dependency order.
		//If any step throws, the destructor runs for members already constructed.
		//m_window stays nullptr until initWindow() succeeds,
		//so the destructor can safely check before calling glfwDestroyWindow.
		initGlfw();
		initWindow();
		initImGui();
		applyTheme();
		applyFonts();
	}

	Application::~Application()
	{
		//Shutdown in reverse initialization order
		shutdownImGui();
		shutdownGlfw();
	}
    //----------------------Constructor & Destructor-----------------------------------

    //----------------------Initialization---------------------------------------------
	void Application::initGlfw()
	{
		if (!glfwInit())
		{
			throw std::runtime_error(AppStrings::ErrGlfwInit);
		}
	}

	void Application::initWindow()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_window = glfwCreateWindow(
			m_config.windowWidth,
			m_config.windowHeight,
			m_config.windowTitle.c_str(),
			nullptr,
			nullptr
		);

		if (!m_window)
		{
			glfwTerminate();
			throw std::runtime_error(AppStrings::ErrWindowCreate);
		}

		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(m_config.vsync ? 1 : 0);
	}

	void Application::initImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		io.IniFilename = nullptr;

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init(AppStrings::GLSLVersion);

	}

    void Application::applyTheme() 
	{
        ImGuiStyle& style = ImGui::GetStyle();

        // ── Geometry ──────────────────────────────────────────────────────────
        style.WindowRounding = 8.0f;  // Rounded window corners
        style.ChildRounding = 6.0f;
        style.FrameRounding = 5.0f;  // Rounded buttons, inputs, checkboxes
        style.PopupRounding = 6.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 4.0f;  // Scrollbar grab handle
        style.TabRounding = 5.0f;
        style.WindowPadding = ImVec2(16.0f, 14.0f);
        style.FramePadding = ImVec2(10.0f, 6.0f);
        style.ItemSpacing = ImVec2(10.0f, 8.0f);
        style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
        style.IndentSpacing = 20.0f;
        style.ScrollbarSize = 12.0f;
        style.GrabMinSize = 10.0f;
        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;  // No border on inputs/buttons — cleaner look

        // ── Colours ───────────────────────────────────────────────────────────
        // Helper: converts 0-255 RGBA to ImVec4 0.0-1.0 RGBA.
        auto RGBA = [](int r, int g, int b, int a = 255) -> ImVec4 {
            return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
            };

        ImVec4* col = style.Colors;

        // Backgrounds
        col[ImGuiCol_WindowBg] = RGBA(18, 18, 22);       // Near-black
        col[ImGuiCol_ChildBg] = RGBA(24, 24, 30);
        col[ImGuiCol_PopupBg] = RGBA(28, 28, 36);
        col[ImGuiCol_FrameBg] = RGBA(35, 35, 45);       // Input field bg
        col[ImGuiCol_FrameBgHovered] = RGBA(45, 45, 58);
        col[ImGuiCol_FrameBgActive] = RGBA(55, 55, 70);

        // Title bars
        col[ImGuiCol_TitleBg] = RGBA(18, 18, 22);
        col[ImGuiCol_TitleBgActive] = RGBA(22, 22, 30);
        col[ImGuiCol_TitleBgCollapsed] = RGBA(18, 18, 22);

        // Accent: blue — used for selected items, buttons, checkmarks
        col[ImGuiCol_CheckMark] = RGBA(99, 155, 255);
        col[ImGuiCol_SliderGrab] = RGBA(99, 155, 255);
        col[ImGuiCol_SliderGrabActive] = RGBA(140, 185, 255);

        // Buttons
        col[ImGuiCol_Button] = RGBA(37, 99, 235);       // Blue
        col[ImGuiCol_ButtonHovered] = RGBA(59, 130, 246);       // Lighter blue
        col[ImGuiCol_ButtonActive] = RGBA(29, 78, 216);       // Darker blue

        // Header (selectable rows, tree nodes, collapsible headers)
        col[ImGuiCol_Header] = RGBA(37, 99, 235, 80);
        col[ImGuiCol_HeaderHovered] = RGBA(37, 99, 235, 140);
        col[ImGuiCol_HeaderActive] = RGBA(37, 99, 235, 200);

        // Tabs
        col[ImGuiCol_Tab] = RGBA(28, 28, 36);
        col[ImGuiCol_TabHovered] = RGBA(37, 99, 235, 180);
        col[ImGuiCol_TabActive] = RGBA(37, 99, 235);
        col[ImGuiCol_TabUnfocused] = RGBA(28, 28, 36);
        col[ImGuiCol_TabUnfocusedActive] = RGBA(40, 40, 52);

        // Separators, borders
        col[ImGuiCol_Separator] = RGBA(60, 60, 80);
        col[ImGuiCol_SeparatorHovered] = RGBA(99, 155, 255);
        col[ImGuiCol_SeparatorActive] = RGBA(140, 185, 255);
        col[ImGuiCol_Border] = RGBA(50, 50, 68);
        col[ImGuiCol_BorderShadow] = RGBA(0, 0, 0, 0);

        // Scrollbar
        col[ImGuiCol_ScrollbarBg] = RGBA(18, 18, 22, 80);
        col[ImGuiCol_ScrollbarGrab] = RGBA(60, 60, 80);
        col[ImGuiCol_ScrollbarGrabHovered] = RGBA(80, 80, 105);
        col[ImGuiCol_ScrollbarGrabActive] = RGBA(99, 155, 255);

        // Text
        col[ImGuiCol_Text] = RGBA(230, 230, 240);       // Off-white, easier on eyes
        col[ImGuiCol_TextDisabled] = RGBA(100, 100, 120);

        // Resize grip (bottom-right corner handle)
        col[ImGuiCol_ResizeGrip] = RGBA(37, 99, 235, 40);
        col[ImGuiCol_ResizeGripHovered] = RGBA(37, 99, 235, 140);
        col[ImGuiCol_ResizeGripActive] = RGBA(37, 99, 235, 240);

        // Docking
        col[ImGuiCol_DockingPreview] = RGBA(37, 99, 235, 180);
        col[ImGuiCol_DockingEmptyBg] = RGBA(18, 18, 22);
    }

    void Application::applyFonts()
	{
        ImGuiIO& io = ImGui::GetIO();

        io.FontGlobalScale = 1.25f;
	}
    //----------------------Initialization---------------------------------------------

    //----------------------Shutdown---------------------------------------------------
    void Application::shutdownImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Application::shutdownGlfw()
    {
	    if(m_window)
	    {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
	    }
        glfwTerminate();
    }
    //----------------------Shutdown---------------------------------------------------
    
    //----------------------Render loop------------------------------------------------
    void Application::run()
    {
	    while (!glfwWindowShouldClose(m_window))
	    {
            glfwPollEvents();
            beginFrame();
            renderUI();
            endFrame();
	    }
    }

    void Application::beginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Application::endFrame()
    {
        ImGui::Render();

        int w = 0, h = 0;
        glfwGetFramebufferSize(m_window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.07f, 0.07f, 0.09f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_window);
    }
    //----------------------Render loop------------------------------------------------

    //----------------------Render UI--------------------------------------------------
    //The top-level UI dispatch for a frame
    // Calls individual panel-rendering methods.
    void Application::renderUI()
	{
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags dockFlags =
            ImGuiWindowFlags_NoTitleBar             |
            ImGuiWindowFlags_NoCollapse             |
            ImGuiWindowFlags_NoResize               |
            ImGuiWindowFlags_NoMove                 |
            ImGuiWindowFlags_NoBringToFrontOnFocus  |
            ImGuiWindowFlags_NoNavFocus             |
            ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin(AppStrings::DockSpaceWindow, nullptr, dockFlags);
        ImGui::PopStyleVar(3);
        ImGuiID dockId = ImGui::GetID(AppStrings::DockSpace);
        ImGui::DockSpace(dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();

        renderMainPanel();

        if (m_showDemoWindow)
        {
            ImGui::ShowDemoWindow(&m_showDemoWindow);
        }
	}
    //----------------------Render UI--------------------------------------------------

    //----------------------Render Main Panel------------------------------------------

    void Application::renderMainPanel()
    {
	    //Position and size on first appearance. The user can resize/move it.
        const ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x + 40.0f, vp->WorkPos.y + 40.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(1200.0f, 720.0f), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin(AppStrings::PanelMain, nullptr, ImGuiWindowFlags_NoCollapse))
        {
            ImGui::End();
            return;
        }

        //Header bar
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.39f, 0.61f, 1.0f, 1.0f));
        ImGui::Text(AppStrings::PanelMain);
        ImGui::PopStyleColor();

        ImGui::SameLine();
        ImGui::TextDisabled(AppStrings::AppHeaderDescription);
        ImGui::Separator();
        ImGui::Spacing();

        //Two-column layout: file list (left), controls (right)
        //Column proportions: 65% file area, 35% controls
        const float totalWidth = ImGui::GetContentRegionAvail().x;
        const float leftWidth = totalWidth * 0.65f;
        const float rightWidth = totalWidth * 0.35f - ImGui::GetStyle().ItemSpacing.x;

        //Left: File list panel
        ImGui::BeginChild(AppStrings::PanelFileList, ImVec2(leftWidth, 0.0f), ImGuiChildFlags_Borders);
        {
            ImGui::TextDisabled("%s (%d)", AppStrings::LabelFiles, 0);
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 0.5f, 1.0f));
            ImGui::TextWrapped(AppStrings::InstructionToSelectFiles);
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();

        //Right: Control panel
        ImGui::SameLine();
        ImGui::BeginChild(AppStrings::PanelControls, ImVec2(rightWidth, 0.0f), ImGuiChildFlags_Borders);
        {
	        //Add Files Button
            ImGui::Spacing();
            const float buttonWidth = ImGui::GetContentRegionAvail().x;

            if (ImGui::Button(AppStrings::ButtonAndFiles, ImVec2(buttonWidth, 40.0f)))
            {
	            //Open native picker here
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            //Output destination
            ImGui::TextDisabled(AppStrings::LabelOutputDestination);
            ImGui::Spacing();

            static char outputPath[512] = "";
            ImGui::SetNextItemWidth(buttonWidth - 70.0f);
            ImGui::InputText(AppStrings::PanelOutputPath, outputPath, sizeof(outputPath));
            ImGui::SameLine();
            if(ImGui::Button(AppStrings::ButtonBrowse, ImVec2(60.0f, 0.0f)))
            {
	            //Open folder picker here
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            //Merge button: full width, taller, prominent
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.14f, 0.39f, 0.92f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.23f, 0.51f, 0.96f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.11f, 0.31f, 0.73f, 1.0f));

            if(ImGui::Button(AppStrings::ButtonMerge, ImVec2(buttonWidth, 52.0f)))
            {
	            //Start conversion pipeline
            }
            ImGui::PopStyleColor(3);

            ImGui::Spacing();

            //Debug helper: toggle ImGui demo window with F1
            if (ImGui::IsKeyPressed(ImGuiKey_F1))
            {
                m_showDemoWindow = !m_showDemoWindow;
            }
            ImGui::TextDisabled(AppStrings::InstructionToggleDemoWindow);
        }
        ImGui::EndChild();

        ImGui::End();
    }

    //----------------------Render Main Panel------------------------------------------
}
