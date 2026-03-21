// src/Document.h
//
// Single representation of a queue entry from enqueue to success.
// Plain value type. Copyable and movable.
// ConversionStatus and Document live here together, they are inseparable.

#pragma once
#include <filesystem>
#include <string>
#include <cstdint>

namespace Doculith
{
	enum class ConversionStatus
	{
		Pending,		// In queue, not yet converted.
		Converting,		//Conversion subprocess running
		Done,			//PDF produced successfully
		Failed			// Conversion failed, see errorMessage
	};

	struct Document
	{
		std::filesystem::path	sourcePath;					// Canonical .docx path
		std::filesystem::path	pdfPath;					// Set when status == done
		std::string				displayName;				// sourcePath.stem().string()
		std::string				errorMessage;				// Non-empty when status == failed
		ConversionStatus		status				= ConversionStatus::Pending;
		std::uint32_t			thumbnailTextureId	= 0;	// GL Texture ID
	};
}
