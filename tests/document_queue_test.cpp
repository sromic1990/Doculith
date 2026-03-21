// tests/document_queue_test.cpp

#include <catch2/catch_test_macros.hpp>
#include "DocumentQueue.h"
#include "Document.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;
using namespace Doculith;

// RAII helper: creates a temp directory with real .docx files
class TempDocxFixture
{
public:
	TempDocxFixture()
	{
		m_dir = fs::temp_directory_path() / "doculith_test";
		fs::create_directories(m_dir);
	}

	~TempDocxFixture()
	{
		fs::remove_all(m_dir);
	}

	// Create an empty .docx file with the given stem and return its path
	fs::path create(const std::string& stem) const
	{
		fs::path p = m_dir / (stem + ".docx");
		std::ofstream(p).flush(); // Creates an empty file.
		return p;
	}

	fs::path dir() const { return m_dir; }

private:
	fs::path m_dir;
};

// Tests
TEST_CASE("Document queue starts empty", "[queue]")
{
	DocumentQueue q;
	REQUIRE(q.empty());
	REQUIRE(q.size() == 0);
	REQUIRE(q.allConverted() == false);
	REQUIRE(q.sourceDirectory() == std::nullopt);
	REQUIRE(q.suggestedOutputPath() == std::nullopt);
}

TEST_CASE("DocumentQueue adds valid docx files", "[queue]")
{
	TempDocxFixture f;
	auto a = f.create("alpha");
	auto b = f.create("beta");

	DocumentQueue q;
	auto r = q.addFiles({a, b});

	REQUIRE(r.added == 2);
	REQUIRE(r.rejected == 0);
	REQUIRE(q.size() == 2);
	REQUIRE(q.documents()[0].displayName == "alpha");
	REQUIRE(q.documents()[1].displayName == "beta");
	REQUIRE(q.sourceDirectory() == f.dir());
}

TEST_CASE("DocumentQueue rejects non-docx files", "[queue]") {
    TempDocxFixture f;
    fs::path pdf = f.dir() / "report.pdf";
    std::ofstream(pdf).flush();
 
    DocumentQueue q;
    auto r = q.addFiles({pdf});
 
    REQUIRE(r.added    == 0);
    REQUIRE(r.rejected == 1);
    REQUIRE(q.empty());
}
 
TEST_CASE("DocumentQueue rejects duplicate files", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("alpha");
 
    DocumentQueue q;
    q.addFiles({a});
    auto r = q.addFiles({a}); // same file again
 
    REQUIRE(r.added    == 0);
    REQUIRE(r.rejected == 1);
    REQUIRE(q.size()   == 1); // still only one
}

TEST_CASE("DocumentQueue enforces single-directory constraint", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("alpha");
 
    // Create a file in a different directory.
    fs::path otherDir = fs::temp_directory_path() / "doculith_other";
    fs::create_directories(otherDir);
    fs::path b = otherDir / "beta.docx";
    std::ofstream(b).flush();
	DocumentQueue q;
    q.addFiles({a});
    auto r = q.addFiles({b}); // different directory — must reject
 
    REQUIRE(r.added    == 0);
    REQUIRE(r.rejected == 1);
    REQUIRE(q.size()   == 1);
 
    fs::remove_all(otherDir);
}

TEST_CASE("DocumentQueue rejects non-existent files", "[queue]") {
    fs::path ghost = fs::temp_directory_path() / "phantom.docx";
    DocumentQueue q;
    auto r = q.addFiles({ghost});
    REQUIRE(r.added    == 0);
    REQUIRE(r.rejected == 1);
}
 
TEST_CASE("DocumentQueue removeAt works correctly", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("a");
    auto b = f.create("b");
    auto c = f.create("c");
 
    DocumentQueue q;
    q.addFiles({a, b, c});
    q.removeAt(1); // Remove "b"
 
    REQUIRE(q.size() == 2);
    REQUIRE(q.documents()[0].displayName == "a");
    REQUIRE(q.documents()[1].displayName == "c");
}
 
TEST_CASE("DocumentQueue removeAt ignores out-of-range index", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("a");
    DocumentQueue q;
    q.addFiles({a});
    REQUIRE_NOTHROW(q.removeAt(99)); // Out of range — no crash
    REQUIRE(q.size() == 1);
}
 
TEST_CASE("DocumentQueue clear resets everything", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("a");
 
    DocumentQueue q;
    q.addFiles({a});
    q.clear();
 
    REQUIRE(q.empty());
    REQUIRE(q.sourceDirectory() == std::nullopt);
}

TEST_CASE("DocumentQueue clear allows new directory after clear", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("a");
 
    // Create a second temp directory.
    fs::path dir2 = fs::temp_directory_path() / "doculith_test2";
    fs::create_directories(dir2);
    fs::path b = dir2 / "b.docx";
    std::ofstream(b).flush();
 
    DocumentQueue q;
    q.addFiles({a});
    q.clear(); // Release directory lock
    auto r = q.addFiles({b}); // Different directory — now allowed
 
    REQUIRE(r.added == 1);
    REQUIRE(q.sourceDirectory() == dir2);
 
    fs::remove_all(dir2);
}

TEST_CASE("DocumentQueue move reorders correctly (forward)", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("a");
    auto b = f.create("b");
    auto c = f.create("c");
 
    DocumentQueue q;
    q.addFiles({a, b, c});
    q.move(0, 2); // Move "a" to position 2
 
    REQUIRE(q.documents()[0].displayName == "b");
    REQUIRE(q.documents()[1].displayName == "c");
    REQUIRE(q.documents()[2].displayName == "a");
}
 
TEST_CASE("DocumentQueue move reorders correctly (backward)", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("a");
    auto b = f.create("b");
    auto c = f.create("c");
 
    DocumentQueue q;
    q.addFiles({a, b, c});
    q.move(2, 0); // Move "c" to position 0
 
    REQUIRE(q.documents()[0].displayName == "c");
    REQUIRE(q.documents()[1].displayName == "a");
    REQUIRE(q.documents()[2].displayName == "b");
}
 
TEST_CASE("DocumentQueue move same index is a no-op", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("a");
    auto b = f.create("b");
 
    DocumentQueue q;
    q.addFiles({a, b});
    q.move(0, 0);
 
    REQUIRE(q.documents()[0].displayName == "a");
    REQUIRE(q.documents()[1].displayName == "b");
}
 
TEST_CASE("DocumentQueue move out-of-range is safe", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("a");
    DocumentQueue q;
    q.addFiles({a});
    REQUIRE_NOTHROW(q.move(0, 99));
    REQUIRE_NOTHROW(q.move(99, 0));
    REQUIRE(q.size() == 1);
}

TEST_CASE("DocumentQueue suggestedOutputPath uses source dir", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("a");
 
    DocumentQueue q;
    q.addFiles({a});
 
    const auto suggested = q.suggestedOutputPath();
    REQUIRE(suggested.has_value());
    REQUIRE(suggested->parent_path() == q.sourceDirectory());
    REQUIRE(suggested->filename()    == "merged.pdf");
}
 
TEST_CASE("DocumentQueue setStatus updates document", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("a");
 
    DocumentQueue q;
    q.addFiles({a});
    REQUIRE(q.documents()[0].status == ConversionStatus::Pending);
    REQUIRE(q.allConverted() == false);
 
    q.setStatus(0, ConversionStatus::Done);
    REQUIRE(q.documents()[0].status == ConversionStatus::Done);
    REQUIRE(q.allConverted() == true);
}
 
TEST_CASE("DocumentQueue allConverted is false if any pending", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("a");
    auto b = f.create("b");
 
    DocumentQueue q;
    q.addFiles({a, b});
    q.setStatus(0, ConversionStatus::Done);
    // b is still Pending
    REQUIRE(q.allConverted() == false);
}
 
TEST_CASE("DocumentQueue allConverted is false if any failed", "[queue]") {
    TempDocxFixture f;
    auto a = f.create("a");
    auto b = f.create("b");
 
    DocumentQueue q;
    q.addFiles({a, b});
    q.setStatus(0, ConversionStatus::Done);
    q.setStatus(1, ConversionStatus::Failed, {}, "soffice crashed");
 
    REQUIRE(q.allConverted() == false); // Failed is not Done
}

TEST_CASE("DocumentQueue setThumbnailId updates correctly", "[queue]")
{
	TempDocxFixture f;
	auto a = f.create("a");

	DocumentQueue q;
	q.addFiles({ a });
	REQUIRE(q.documents()[0].thumbnailTextureId == 0u);

	q.setThumbnailId(0, 42u);
	REQUIRE(q.documents()[0].thumbnailTextureId == 42u);
}

TEST_CASE("DocumentQueue setThumbnailId ignores out-of-range", "[queue]")
{
	DocumentQueue q;
	REQUIRE_NOTHROW(q.setThumbnailId(0, 1u)); // Empty queue, no crash
	REQUIRE_NOTHROW(q.setThumbnailId(99, 1u)); // OutOfBounds, no crash
}
