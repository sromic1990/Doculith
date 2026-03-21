// src/UiEvents.h
// Plain struct describing user-initialized actions fired in one frame.
// Produced by Ui::render(), consumed by Application::dispatch().

#pragma once
#include <cstddef>
#include <optional>

namespace Doculith
{
	struct UiEvents
	{
		bool addFilesClicked		= false;
		bool browseOutputClicked	= false;
		bool clearAllClicked		= false;
		std::optional<std::size_t> removeAtIndex; // Set when user clicks remove
		
		bool mergeClicked			= false;

		// Drag and drop reorder
		// Both set to a valid index when the drag-and-drop reorder completes.
		std::optional<std::size_t> moveFromIndex;
		std::optional<std::size_t> moveToIndex;
	};
}