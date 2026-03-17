#pragma once
// ─────────────────────────────────────────────────────────────────────────────
// src/AppStrings.hpp
//
// Central store for all user-visible strings and app metadata.
// No magic string literals scattered through the codebase.
// To change any text, there is exactly one place to look.
// ─────────────────────────────────────────────────────────────────────────────

namespace Doculith
{
	struct AppStrings
	{
		static constexpr const char* GLSLVersion							= "#version 330 core";

		static constexpr const char* AppName								= "Doculith";
		static constexpr const char* AppVersion								= "0.1.0";
		static constexpr const char* AppDescription							= "DOCX to PDF Merger";
		static constexpr const char* AppTagline								= "Doculith - Document Merger";
		static constexpr const char* AppHeaderDescription					= "| DOCX to PDF Merger";
		static constexpr const char* WindowTitle							= "Doculith";

		static constexpr const char* ButtonAndFiles							= "+ Add Files";
		static constexpr const char* ButtonMerge							= "Merge";
		static constexpr const char* ButtonBrowse							= "...";
		static constexpr const char* LabelOutputDestination					= "Output Destination";
		static constexpr const char* LabelOutputPath						= "Output path:";
		static constexpr const char* LabelNoFiles							= "No files selected";
		static constexpr const char* LabelFiles								= "Files";

		static constexpr const char* PanelMain								= "Doculith";
		static constexpr const char* PanelFileList							= "##FileListPanel";
		static constexpr const char* PanelControls							= "##ControlsPanel";
		static constexpr const char* PanelOutputPath						= "##OutputPath";
		static constexpr const char* DockSpace								= "##MainDockSpace";
		static constexpr const char* DockSpaceWindow						= "##DockSpaceWindow";

		static constexpr const char* ErrGlfwInit							= "glfwInit() failed!";
		static constexpr const char* ErrWindowCreate						= "glfwCreateWindow() failed!";
		static constexpr const char* ErrFatal								= "Fatal error:";

		static constexpr const char* InstructionToSelectFiles				= "No files selected. Use 'Add Files' to get started.";
		static constexpr const char* InstructionToggleDemoWindow			= "F1: Toggle ImGui demo";
	};
}