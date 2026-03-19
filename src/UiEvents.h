// src/UiEvents.h
// Plain struct describing user-initialized actions fired in onde frame.
// Produced by Ui::render(), consumed by Application::dispatch().

#pragma once

namespace Doculith
{
	struct UiEvents
	{
		bool addFilesClicked		= false;
		bool browseOutputClicked	= false;
		bool clearAllClicked		= false;

		bool mergeClicked			= false;
	};
}