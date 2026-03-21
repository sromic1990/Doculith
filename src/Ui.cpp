// src/Ui.cpp
#include "Ui.h"
#include "AppStrings.h"
#include "imgui.h"

namespace Doculith
{
	void Ui::applyFonts(float /*uiScale*/)
	{
        ImGui::GetIO().FontGlobalScale = 1.25f;
	}

	UiEvents Ui::render(AppModel& model)
	{
        UiEvents events;

        renderDockSpace();
        renderMainPanel(events, model);

        if (m_showDemoWindow)
        {
            ImGui::ShowDemoWindow(&m_showDemoWindow);
        }

        return events;
	}

	void Ui::renderDockSpace()
	{
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags dockFlags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("##DockSpaceWindow", nullptr, dockFlags);
        ImGui::PopStyleVar(3);
        ImGuiID dockId = ImGui::GetID("##MainDockSpace");
        ImGui::DockSpace(dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();
	}

	void Ui::renderMainPanel(UiEvents& events, AppModel& model)
	{
        //Position and size on first appearance. The user can resize/move it.
        const ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(vp->WorkPos.x + 40.0f, vp->WorkPos.y + 40.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(1200.0f, 720.0f), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin(strings::kAppName, nullptr, ImGuiWindowFlags_NoCollapse))
        {
            ImGui::End();
            return;
        }

        //Header bar
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.39f, 0.61f, 1.0f, 1.0f));
        ImGui::Text(strings::kAppName);
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::TextDisabled(strings::kAppSubtitle);
        ImGui::Separator();
        ImGui::Spacing();

        //Two-column layout: file list (left), controls (right)
        //Column proportions: 65% file area, 35% controls
        const float totalWidth = ImGui::GetContentRegionAvail().x;
        const float leftWidth = totalWidth * 0.65f;
        //const float rightWidth = totalWidth * 0.35f - ImGui::GetStyle().ItemSpacing.x;

        renderFileList(events, model, leftWidth);
        ImGui::SameLine();
        renderControlsPanel(events, model);

        ImGui::End();
	}

    void Ui::renderFileList(UiEvents& events, const AppModel& model, float panelWidth)
    {
        ImGui::BeginChild("##FileListPanel", ImVec2(panelWidth, 0.0f), ImGuiChildFlags_Borders);

        const auto& docs = model.queue.documents();

        // Header row
        const std::string header = "Files (" + std::to_string(docs.size()) + ")";
        ImGui::TextDisabled("%s", header.c_str());
        ImGui::Separator();
        ImGui::Spacing();

        // Empty docs state
        if (docs.empty())
        {
            const float childH = ImGui::GetContentRegionAvail().y;
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + childH * 0.4f);
            const char* msg = "No files selected. Use '+ Add Files' to begin.";
            const ImVec2 textSz = ImGui::CalcTextSize(msg);
            ImGui::SetCursorPosX((panelWidth - textSz.x) * 0.5f);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.35f, 0.35f, 0.45f, 1.0f));
            ImGui::TextUnformatted(msg);
            ImGui::PopStyleColor();
            ImGui::EndChild();
            return;
        }

        // Document rows
        // Track any deferred remove request
        // We record the index and act AFTER the loop.
        // Removing during iteration would invalidate the docs reference.
        // Then underlying vector may reallocate
        std::optional<std::size_t> pendingRemove;

        for (std::size_t i =0; i < docs.size(); ++i)
        {
            const auto& doc = docs[i];

            // PushID gives each row a unique ID scope.
            // Without this, all remove buttons will share the same ImGui ID, 
            // and only the last one would respond correctly.
            ImGui::PushID(static_cast<int>(i));

            // Status dot
            ImVec4 dotColor = ImVec4(0.4f, 0.4f, 0.5f, 1.0f); // Pending (grey)
            if (doc.status == ConversionStatus::Converting)
                dotColor = ImVec4(1.0f, 0.8f, 0.2f, 1.0f); // Yellow
            if (doc.status == ConversionStatus::Done)
                dotColor = ImVec4(0.2f, 0.9f, 0.4f, 1.0f); // Green
            if (doc.status == ConversionStatus::Failed)
                dotColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f); // Red

            ImGui::PushStyleColor(ImGuiCol_Text, dotColor);
            ImGui::TextUnformatted("●");
            ImGui::PopStyleColor();
            ImGui::SameLine(0.0f, 8.0f);

            // Filename
            ImGui::TextUnformatted(doc.displayName.c_str());

            // Show the source directory path in muted text below the filename
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 0.5f, 1.0f));
            ImGui::Text(" %s", doc.sourcePath.parent_path().string().c_str());
            ImGui::PopStyleColor();

            // Error message
            if (doc.status == ConversionStatus::Failed && !doc.errorMessage.empty())
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
                ImGui::Text(" Error: %s", doc.errorMessage.c_str());
                ImGui::PopStyleColor();
            }

            // Remove button (right-aligned)
            constexpr float removeW = 70.0f;
            ImGui::SameLine(panelWidth - removeW - ImGui::GetStyle().WindowPadding.x);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.28f, 0.08f, 0.08f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.12f, 0.12f, 1.0f));
            if (ImGui::SmallButton(strings::kRemoveBtn))
            {
                pendingRemove = i; // Deferred, applied after the loop
            }

            ImGui::PopStyleColor(2);

            ImGui::Separator();
            ImGui::PopID();
        }

        ImGui::EndChild();

        if (pendingRemove.has_value())
        {
            events.removeAtIndex = *pendingRemove;
        }
    }

    void Ui::renderControlsPanel(UiEvents& events, AppModel& model)
    {
        ImGui::BeginChild("##ControlsPanel", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Borders);
        {
        	ImGui::Spacing();
            const float buttonWidth = ImGui::GetContentRegionAvail().x;

            //Add Files Button
            if (ImGui::Button(strings::kAddFileBtn, ImVec2(buttonWidth, 40.0f)))
            {
	            //Open native picker here
	            events.addFilesClicked = true;
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            //Output destination
            ImGui::TextDisabled(strings::kOutputPdfHeader);
            ImGui::Spacing();

            ImGui::SetNextItemWidth(buttonWidth - 70.0f);
            ImGui::InputText("##OutputPath", model.outputPathBuf, sizeof(model.outputPathBuf));
            ImGui::SameLine();
            if (ImGui::Button(strings::kBrowseBtn, ImVec2(60.0f, 0.0f)))
            {
	            //Open folder picker here
	            events.browseOutputClicked = true;
            }

            // Show the source directory for context
            if(const auto srcDir = model.queue.sourceDirectory())
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.35f, 0.35f, 0.45f, 1.0f));
                ImGui::TextWrapped("Source: %s", srcDir->string().c_str());
                ImGui::PopStyleColor();
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            //Merge button: full width, taller, prominent
            const bool canMerge = !model.queue.empty();
            ImGui::BeginDisabled(!canMerge);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.14f, 0.39f, 0.92f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.23f, 0.51f, 0.96f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.11f, 0.31f, 0.73f, 1.0f));

            if (ImGui::Button(strings::kMergeBtnLabel, ImVec2(buttonWidth, 52.0f)))
            {
                //Start conversion pipeline
                events.mergeClicked = true;
            }
            ImGui::PopStyleColor(3);
            ImGui::EndDisabled();

            // Feedback message
            if (!model.addFeedback.empty())
            {
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.75f, 1.0f, 1.0f));
                ImGui::TextWrapped("%s", model.addFeedback.c_str());
                ImGui::PopStyleColor();
            }

            ImGui::Spacing();

            //Debug helper: toggle ImGui demo window with F1
            if (ImGui::IsKeyPressed(ImGuiKey_F1, false))
            {
                m_showDemoWindow = !m_showDemoWindow;
            }
            ImGui::TextDisabled(strings::kDebugHint);
        }
        ImGui::EndChild();
    }

    void Ui::applyTheme()
	{
        ImGuiStyle& style = ImGui::GetStyle();

        // ── Geometry ──────────────────────────────────────────────────────────
        style.WindowRounding            = 8.0f;  // Rounded window corners
        style.ChildRounding             = 6.0f;
        style.FrameRounding             = 5.0f;  // Rounded buttons, inputs, checkboxes
        style.PopupRounding             = 6.0f;
        style.ScrollbarRounding         = 6.0f;
        style.GrabRounding              = 4.0f;  // Scrollbar grab handle
        style.TabRounding               = 5.0f;
        style.WindowPadding             = ImVec2(16.0f, 14.0f);
        style.FramePadding              = ImVec2(10.0f, 6.0f);
        style.ItemSpacing               = ImVec2(10.0f, 8.0f);
        style.ItemInnerSpacing          = ImVec2(8.0f, 6.0f);
        style.IndentSpacing             = 20.0f;
        style.ScrollbarSize             = 12.0f;
        style.GrabMinSize               = 10.0f;
        style.WindowBorderSize          = 1.0f;
        style.ChildBorderSize           = 1.0f;
        style.FrameBorderSize           = 0.0f;  // No border on inputs/buttons — cleaner look

        // ── Colours ───────────────────────────────────────────────────────────
        // Helper: converts 0-255 RGBA to ImVec4 0.0-1.0 RGBA.
        auto RGBA = [](int r, int g, int b, int a = 255) -> ImVec4 
		{
            return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		};

        ImVec4* col = style.Colors;

        // Backgrounds
        col[ImGuiCol_WindowBg]              = RGBA(18, 18, 22);       // Near-black
        col[ImGuiCol_ChildBg]               = RGBA(24, 24, 30);
        col[ImGuiCol_PopupBg]               = RGBA(28, 28, 36);
        col[ImGuiCol_FrameBg]               = RGBA(35, 35, 45);       // Input field bg
        col[ImGuiCol_FrameBgHovered]        = RGBA(45, 45, 58);
        col[ImGuiCol_FrameBgActive]         = RGBA(55, 55, 70);

        // Title bars
        col[ImGuiCol_TitleBg]               = RGBA(18, 18, 22);
        col[ImGuiCol_TitleBgActive]         = RGBA(22, 22, 30);
        col[ImGuiCol_TitleBgCollapsed]      = RGBA(18, 18, 22);

        // Accent: blue — used for selected items, buttons, checkmarks
        col[ImGuiCol_CheckMark]             = RGBA(99, 155, 255);
        col[ImGuiCol_SliderGrab]            = RGBA(99, 155, 255);
        col[ImGuiCol_SliderGrabActive]      = RGBA(140, 185, 255);

        // Buttons
        col[ImGuiCol_Button]                = RGBA(37, 99, 235);       // Blue
        col[ImGuiCol_ButtonHovered]         = RGBA(59, 130, 246);       // Lighter blue
        col[ImGuiCol_ButtonActive]          = RGBA(29, 78, 216);       // Darker blue

        // Header (selectable rows, tree nodes, collapsible headers)
        col[ImGuiCol_Header]                = RGBA(37, 99, 235, 80);
        col[ImGuiCol_HeaderHovered]         = RGBA(37, 99, 235, 140);
        col[ImGuiCol_HeaderActive]          = RGBA(37, 99, 235, 200);

        // Tabs
        col[ImGuiCol_Tab]                   = RGBA(28, 28, 36);
        col[ImGuiCol_TabHovered]            = RGBA(37, 99, 235, 180);
        col[ImGuiCol_TabActive]             = RGBA(37, 99, 235);
        col[ImGuiCol_TabUnfocused]          = RGBA(28, 28, 36);
        col[ImGuiCol_TabUnfocusedActive]    = RGBA(40, 40, 52);

        // Separators, borders
        col[ImGuiCol_Separator]             = RGBA(60, 60, 80);
        col[ImGuiCol_SeparatorHovered]      = RGBA(99, 155, 255);
        col[ImGuiCol_SeparatorActive]       = RGBA(140, 185, 255);
        col[ImGuiCol_Border]                = RGBA(50, 50, 68);
        col[ImGuiCol_BorderShadow]          = RGBA(0, 0, 0, 0);

        // Scrollbar
        col[ImGuiCol_ScrollbarBg]           = RGBA(18, 18, 22, 80);
        col[ImGuiCol_ScrollbarGrab]         = RGBA(60, 60, 80);
        col[ImGuiCol_ScrollbarGrabHovered]  = RGBA(80, 80, 105);
        col[ImGuiCol_ScrollbarGrabActive]   = RGBA(99, 155, 255);

        // Text
        col[ImGuiCol_Text]                  = RGBA(230, 230, 240);       // Off-white, easier on eyes
        col[ImGuiCol_TextDisabled]          = RGBA(100, 100, 120);

        // Resize grip (bottom-right corner handle)
        col[ImGuiCol_ResizeGrip]            = RGBA(37, 99, 235, 40);
        col[ImGuiCol_ResizeGripHovered]     = RGBA(37, 99, 235, 140);
        col[ImGuiCol_ResizeGripActive]      = RGBA(37, 99, 235, 240);

        // Docking
        col[ImGuiCol_DockingPreview]        = RGBA(37, 99, 235, 180);
        col[ImGuiCol_DockingEmptyBg]        = RGBA(18, 18, 22);
	}
}
