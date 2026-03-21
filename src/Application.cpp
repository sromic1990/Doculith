#include "Application.h"
#include <nfd.hpp>

namespace Doculith
{
	Application::Application(const ApplicationConfig& config)
		:	m_config {config},
			m_platform {config},
			m_ui {}
	{
		m_ui.applyTheme();
		m_ui.applyFonts(m_platform.uiScale());
	}

	void Application::run()
	{
		while (!m_platform.shouldClose())
		{
			m_platform.pollEvents();
			m_platform.beginFrame();

			const UiEvents events = m_ui.render(m_model);
			dispatch(events);

			m_platform.endFrame();
		}
	}

	void Application::dispatch(const UiEvents& events)
	{
		if (events.addFilesClicked) onAddFilesClicked();
		if (events.browseOutputClicked) onBrowseOutputClicked();
		if (events.clearAllClicked) onClearAllClicked();
		if (events.removeAtIndex.has_value()) onRemoveAt(*events.removeAtIndex);

		if (events.moveFromIndex.has_value() && events.moveToIndex.has_value())
		{
			m_model.queue.move(*events.moveFromIndex, *events.moveToIndex);
		}
	}

	void Application::onAddFilesClicked()
	{
		// Open the native multi-select file dialog
		NFD::UniquePathSet outPaths; // UniquePathSet for multi-select.
		const nfdu8filteritem_t filterList[] = 
		{
			{ "Word Documents", "docx" }
		};

		const nfdresult_t result = NFD::OpenDialogMultiple(
			outPaths,
			filterList,
			1,
			nullptr
		);

		if (result == NFD_CANCEL) return; // User dismissed, nothing to do.
		if (result != NFD_OKAY)
		{
			m_model.addFeedback = "File dialog error: ";
			if (const char* err = NFD::GetError())
			{
				m_model.addFeedback += err;
			}
			return;
		}

		// Collect the selected paths
		std::vector<fs::path> selected;
		nfdpathsetsize_t count = 0;
		NFD::PathSet::Count(outPaths, count);

		for (nfdpathsetsize_t i = 0; i < count; ++i)
		{
			NFD::UniquePathSetPath singlePath;
			NFD::PathSet::GetPath(outPaths, i, singlePath);
			selected.emplace_back(singlePath.get());
		}

		// Add to queue and update feedback
		const AddResult r = m_model.queue.addFiles(selected);
		m_model.addFeedback = r.feedback;
		m_model.refreshOutputPath();
	}

	void Application::onBrowseOutputClicked()
	{
		// Single file save dialog for output PDF path
		NFD::UniquePath outPath;
		const nfdu8filteritem_t filterList[] = {
			{"PDF Files", "pdf" }
		};

		const nfdresult_t result = NFD::SaveDialog(
			outPath,
			filterList,
			1,
			nullptr,				// defaultPath
			"merged.pdf"			// defaultName 
			);

		if (result == NFD_OKAY)
		{
			const std::string chosen(outPath.get());
			const std::size_t len = std::min(chosen.size(), sizeof(m_model.outputPathBuf) - 1u);

			std::memcpy(m_model.outputPathBuf, chosen.data(), len);
			m_model.outputPathBuf[len] = '\0';
		}
	}

	void Application::onClearAllClicked()
	{
		clearModel();
	}

	void Application::onRemoveAt(std::size_t index)
	{
		m_model.queue.removeAt(index);
		if (m_model.queue.empty())
		{
			clearModel(false);
		}
	}

	void Application::clearModel(bool emptyQueue)
	{
		if (emptyQueue) m_model.queue.clear();
		m_model.outputPathBuf[0] = '\0';
		m_model.addFeedback.clear();
	}
}
