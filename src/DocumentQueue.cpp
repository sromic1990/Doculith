// src/DocumentQueue.cpp

#include "DocumentQueue.h"
#include <algorithm>

namespace Doculith
{
	namespace fs = std::filesystem;

	AddResult DocumentQueue::addFiles(const std::vector<std::filesystem::path>& paths)
	{
		AddResult result;

		for (const auto& rawPath : paths)
		{
			// Must be a regular file that exists
			std::error_code ec;
			if (!fs::exists(rawPath, ec) || !fs::is_regular_file(rawPath, ec))
			{
				++result.rejected;
				continue;
			}

			// Canonicalise
			const fs::path canonical = fs::canonical(rawPath, ec);
			if (ec) { ++result.rejected; continue; }

			// Must have DOCX extension (case-insensitive)
			auto ext = canonical.extension().string();
			std::ranges::transform(ext, ext.begin(), 
				[](unsigned char c) -> char
				{
					return static_cast<char>(std::tolower(c));
				}
			);
			if (ext != ".docx") { ++result.rejected; continue; }

			// No duplicates
			const bool duplicate = std::ranges::any_of(
				m_docs, 
				[&](const Document & d) { return d.sourcePath == canonical; });
			if (duplicate) { ++result.rejected; continue; }

			// Same directory constraint
			const fs::path docDir = canonical.parent_path();
			if (m_sourceDirectory.has_value() && *m_sourceDirectory!= docDir)
			{
				++result.rejected;
				continue;
			}

			// Lock the directory (first accepted file sets the directory)
			if (!m_sourceDirectory.has_value()) { m_sourceDirectory = docDir; }

			//Append to queue
			Document doc;
			doc.sourcePath = canonical;
			doc.displayName = canonical.stem().string();
			m_docs.push_back(std::move(doc));
			++result.added;
		}

		if (result.added > 0 && result.rejected > 0)
		{
			result.feedback = std::to_string(result.added) + "file(s) added. "
				+ std::to_string(result.rejected) + " skipped "
				+ "(wrong type, duplicate or different folder).";
		}
		else if (result.added > 0)
		{
			result.feedback = std::to_string(result.added) + " file(s) added.";
		}
		else
		{
			result.feedback = "No files added. Check file types and folder.";
		}
		
		return result;
	}

	void DocumentQueue::removeAt(std::size_t index)
	{
		if (index >= m_docs.size()) return;
		m_docs.erase(m_docs.begin() + static_cast<std::ptrdiff_t>(index));

		if (m_docs.empty())
		{
			m_sourceDirectory.reset(); // Release directory lock
		}
	}

	void DocumentQueue::move(std::size_t from, std::size_t to)
	{
		if (from == to) return;
		if (from >= m_docs.size() || to >= m_docs.size()) return;

		if (from < to)
		{
			// Moving forward: rotate the range [from, to] left by one.
			std::rotate(
				m_docs.begin() + static_cast<std::ptrdiff_t>(from),
				m_docs.begin() + static_cast<std::ptrdiff_t>(from +1),
				m_docs.begin() + static_cast<std::ptrdiff_t>(to +1)
				);
		}
		else
		{
			// Moving backward: rotate the range [to, from]right by one.
			std::rotate(
				m_docs.begin() + static_cast<std::ptrdiff_t>(to),
				m_docs.begin() + static_cast<std::ptrdiff_t>(from),
				m_docs.begin() + static_cast<std::ptrdiff_t>(from + 1)
			);
		}
	}

	void DocumentQueue::clear()
	{
		m_docs.clear();
		m_sourceDirectory.reset();
	}

	void DocumentQueue::setStatus(std::size_t index, ConversionStatus status,
		const std::filesystem::path& pdfPath, const std::string& errorMsg)
	{
		if (index >= m_docs.size()) return;
		m_docs[index].status = status;
		m_docs[index].pdfPath = pdfPath;
		m_docs[index].errorMessage = errorMsg;
	}

	void DocumentQueue::setThumbnailId(std::size_t index, std::uint32_t id)
	{
		if (index >= m_docs.size()) return;
		m_docs[index].thumbnailTextureId = id;
	}

	std::optional<std::filesystem::path> DocumentQueue::sourceDirectory() const noexcept
	{
		return m_sourceDirectory;
	}

	std::optional<std::filesystem::path> DocumentQueue::suggestedOutputPath() const
	{
		if (!m_sourceDirectory.has_value()) return std::nullopt;
		return *m_sourceDirectory / "merged.pdf";
	}

	bool DocumentQueue::allConverted() const noexcept
	{
		if (m_docs.empty()) return false;

		return std::ranges::all_of(m_docs
		                           , [](const Document& d)
		                           {
			                           return d.status == ConversionStatus::Done;
		                           });
	}
}
