// ─────────────────────────────────────────────────────────────────────────────
// src/AppStrings.hpp
//
// All user-visible string literals are defined as name constants
// Every string the user reads comes from here.
// To change any user-visible text, change this file only.
// ─────────────────────────────────────────────────────────────────────────────
#pragma once

namespace Doculith::strings
{
	//------------------------App Identity-------------------------//
	inline constexpr const char* kAppName									= "Doculith";
	inline constexpr const char* kAppSubtitle								= "DOCX to PDF Merger";
	inline constexpr const char* kAppTagline								= "Doculith - Document Merger";
	inline constexpr const char* kDebugHint									= "F1: toggle ImGui demo";

	//--------------------------Toolbar----------------------------//
	inline constexpr const char* kAddFileBtn								= "+ Add Files";
	inline constexpr const char* kClearAllBtn								= "Clear All";
	inline constexpr const char* kFileSingular								= "file";
	inline constexpr const char* kFilePlural								= "files";

	//--------------------------File List--------------------------//
	inline constexpr const char* kEmptyQueueHint							= "No files yet.Use '+ Add Files' to begin.";
	inline constexpr const char* kFileColHeader								= "Document";
	inline constexpr const char* kThumbPending								= "DOC";
	inline constexpr const char* kThumbConverting							= "...";
	inline constexpr const char* kThumbFailed								= "ERR";
	inline constexpr const char* kStatusDot									= "\xe2\x97\x8f";
	inline constexpr const char* kRemoveBtn									= "Remove";

	//-----------------------Control Panel-------------------------//
	inline constexpr const char* kOutputPdfHeader							= "Output Destination";
	inline constexpr const char* kBrowseBtn									= "...";
	inline constexpr const char* kMergeHint									= "Set an output path above to enable Merge.";
	inline constexpr const char* kMergeBtnLabel								= "Merge";

	//-----------------------Overlays------------------------------//
	inline constexpr const char* kConvertingTitle							= "Converting documents...";
	inline constexpr const char* kMergingTitle								= "Merging PDFs...";
	inline constexpr const char* kMergingSubtext							= "Building output document";
	inline constexpr const char* kCancellingTitle							= "Cancelling...";
	inline constexpr const char* kCancellingSubtext							= "Wait for current conversion to complete.";

	//-----------------------Result Panels-------------------------//
	inline constexpr const char* kSuccessTitle								= "Merge Complete";
	inline constexpr const char* kOpenFolderBtn								= "Open Output Folder";
	inline constexpr const char* kMergeAgainBtn								= "Merge Again";
	inline constexpr const char* kFailureTitle								= "Operation Failed";
	inline constexpr const char* kRetryBtn									= "Try Again";
	inline constexpr const char* kStartOverBtn								= "Start Over";

	//---------------------Startup Warning-------------------------//
	inline constexpr const char* kWarningTitle								= "Setup Required";
	inline constexpr const char* kWarningAdvice								= "Install LibreOffice, then restart Doculith";
	inline constexpr const char* kDismissBtn								= "Dismiss";

	//---------------------Startup Warning-------------------------//
	inline constexpr const char* kDocFilterLabel							= "Supported Documents";
	inline constexpr const char* kPdfFilterLabel							= "PDF Files";
	inline constexpr const char* kDefaultPdfName							= "merged.pdf";

	//-------------------Domain Error Messages---------------------//
	//Prefixes: concatenated with a suffix (OS message, extension etc.)
	inline constexpr const char* kErrTempDir								= "Cannot create temp directory: ";
	inline constexpr const char* kErrNoConvertedPdfs						= "No converted PDFs to merge.";
	inline constexpr const char* kErrOutputPathEmpty						= "Output path is empty";
	inline constexpr const char* kErrNoMerger								= "No merger configured.";
	inline constexpr const char* kErrConversionFailed						= "Conversion failed:\n";
	inline constexpr const char* kErrNoConverter							= "No converter found for ";
	inline constexpr const char* kErrNoInputPdfs							= "No input PDFs provided ";
	inline constexpr const char* kErrInputNotFound							= "Input PDF not found: ";
	inline constexpr const char* kErrOutputDir								= "Cannot create output directory: ";
	inline constexpr const char* kErrOutputEmpty							= "Merge complete, but output file is missing or empty.";
	inline constexpr const char* kErrPdfMerge								= "PDF merge error: ";
	inline constexpr const char* kErrUnexpectedMerge						= "Unexpected error during merge: ";
	inline constexpr const char* kErrFatal									= "Fatal error:";
}