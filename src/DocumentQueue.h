// src/DocumentQueue.h
//
// Invariant: all documents are unique, canonical .docx paths from the same parent dir.
//Zero UI dependencies. Fully unit-testable.

#pragma once
#include "Document.h"
#include <filesystem>
#include <optional>
#include <vector>
#include <string>


namespace Doculith
{
	//Returned by addFiles() so callers get structured feedback.
	struct AddResult
	{
		int added = 0;			//How many files were actually queued.
		int rejected = 0;		//How many were rejected (wrong type, duplicate etc.)
		std::string feedback;	//Human-readable summary for UI
	};

	class DocumentQueue
	{
	public:
		DocumentQueue() = default;
		~DocumentQueue() = default;

		DocumentQueue(const DocumentQueue&) = delete;
		DocumentQueue& operator=(const DocumentQueue&) = delete;
		DocumentQueue(DocumentQueue&&) = default;
		DocumentQueue& operator=(DocumentQueue&&) = default;

		// Mutation:
		// Add files from a list of paths. Returns a summary of what happened.
		// Enforces:	.docx extension, regular file, no duplicates,
		//				all files in the same parent directory as existing queue.
		[[nodiscard]] AddResult addFiles(const std::vector<std::filesystem::path>& paths);

		// Remove the document at index. releases directory lock if queue empties
		// No-op if index is out of range
		void removeAt(std::size_t index);
		
		// Reorder: move the document at |from| to position |to|.
		// No-op if from == to or either index is out of range
		void move(std::size_t from, std::size_t to);

		// Remove all documents. Releases directory lock
		void clear();

		// Set the conversion status of document at index.
		// pdfPath: set when status == Done.
		// ErrorMsg: set when status == Failed.
		void setStatus(	std::size_t index,
						ConversionStatus status,
						const std::filesystem::path& pdfPath = {},
						const std::string& errorMsg = {});

		// Update the GL thumbnail texture ID for document at index.
		void setThumbnailId(std::size_t index, std::uint32_t id);

		// Query:
		[[nodiscard]] bool empty() const noexcept { return m_docs.empty(); }
		[[nodiscard]] std::size_t size() const noexcept { return m_docs.size(); }

		// Read-only view of all documents.
		[[nodiscard]] const std::vector<Document>& documents() const noexcept { return m_docs; }

		// The directory all current documents live in.
		// nullopt when the queue is empty.
		[[nodiscard]] std::optional<std::filesystem::path> sourceDirectory() const noexcept;

		// Suggested output PDF path: sourceDirectory / "merged.pdf"
		// nullopt when the queue is empty
		[[nodiscard]] std::optional<std::filesystem::path> suggestedOutputPath() const;

		// True when every document has status == Done.
		// Returns false (not simply true) when the queue is empty.
		[[nodiscard]] bool allConverted() const noexcept;

	private:
		std::vector<Document> m_docs;
		std::optional<std::filesystem::path> m_sourceDirectory;
	};
}