# Doculith
A desktop GUI tool that converts .docx files to PDF and merges them into a single document. Built in modern C++20 with a focus on clean architecture, RAII, and testability.

## What It Does

- **Batch convert** — drop in multiple `.docx` files and convert them all to PDF in one click
- **Merge** — stitch the resulting PDFs into a single output file
- **Preview** — thumbnail previews of converted pages, rendered with PDFium
- **Reorder** — drag and drop files to set the merge order before processing

## Technology Stack

| Component | Library | Why |
|-----------|---------|-----|
| Window & input | GLFW | Cross-platform, minimal |
| Rendering | OpenGL 3.3 Core | Ships with Windows; no extra install |
| UI | Dear ImGui | Immediate mode; state lives in your code |
| File dialogs | NFD-Extended | Native OS picker on Windows & macOS |
| DOCX → PDF | LibreOffice headless | Free, open-source, excellent fidelity |
| PDF merging | PoDoFo | Full PDF object model access |
| Thumbnails | PDFium | The engine inside Chrome |
| Testing | Catch2 | Modern, expressive C++ testing |
| Build | CMake + FetchContent | No manual dependency installs |

## Platform Support

| Platform | Status |
|----------|--------|
| Windows (MSVC) | ✅ Primary target |
| macOS (Clang) | ✅ CI tested |
| Linux | Not targeted |

## Prerequisites

- **Visual Studio 2022** with the "Desktop development with C++" workload
- **CMake 4.2+**
- **Git**
- **LibreOffice** installed and on PATH (`soffice --version` should work)

## Building

```bash
# Clone
git clone [Doculith](https://github.com/sromic1990/Doculith.git)
cd doculith

# Configure (generates Visual Studio solution)
cmake -S . -B build -G "Visual Studio 17 2022" -A x64

# Build
cmake --build build --config Release

# Run tests
cd build && ctest -C Release
```

---

## Development Milestones

### Milestone 1
*Goal: A window that opens, renders, and closes cleanly.*

- ✅ CMake project structure with FetchContent dependencies
- ✅ GLFW window + OpenGL context + Dear ImGui render loop
- ✅ `Application` class with DockSpace and theme
- ✅ Catch2 smoke test passing on CI

**Definition of done:** `cmake --build` succeeds, a styled window opens, and GitHub Actions goes green.

---

### Milestone 2
*Goal: Files go in, a merged PDF comes out.*

- [ ] `DocumentQueue` data model with `std::filesystem` paths
- [ ] Native file picker via NFD-Extended
- [ ] `DocxConverter` — spawns LibreOffice headless via `CreateProcess`
- [ ] `ConversionCoordinator` — async conversion with `std::future`, UI stays alive
- [ ] `PdfMerger` — PoDoFo stitches PDFs together
- [ ] `MergeCoordinator` — async merge pipeline
- [ ] `ThumbnailRasteriser` + `ThumbnailCache` — PDFium + OpenGL textures

**Definition of done:** `.docx` files can be picked, click Convert & Merge, and get a single PDF with thumbnail previews.

---

### Milestone 3 — Full UI
*Goal: Something soneone would actually want to use.*

- [ ] Embedded Inter font, HiDPI scaling
- [ ] Toolbar strip, ImGui Table API file list, controls panel — final form
- [ ] Drag-and-drop reordering with visual drop indicator
- [ ] Keyboard reordering (accessibility)
- [ ] `std::variant` state machine replacing `AppState` enum
- [ ] Exhaustive `std::visit` dispatch — impossible states unrepresentable

**Definition of done:** The app handles all edge cases gracefully, looks polished, and the state machine makes illegal transitions a compile error.

---

### Milestone 4 — Ship It
*Goal: Tested, packaged, and CI green.*

- [ ] `IConverter` interface + `MockConverter` for unit testing without LibreOffice
- [ ] Full test suite: unit, integration, and end-to-end
- [ ] Application icon (`.rc` resource file on Windows)
- [ ] LibreOffice detection with startup warning overlay
- [ ] `--version` and `--help` CLI flags
- [ ] GitHub Actions CI with dependency caching — builds in < 5 min on warm cache

**Definition of done:** CI is green, the app ships with an icon, and missing LibreOffice is handled gracefully rather than crashing.

---

## Branching Strategy

```
main
 └── feature/cmake-setup
 └── feature/hello-window
 └── feature/file-picker
 └── ...
```

- **`main`** is always buildable. Never commit directly to it.
- Every **feature on a branch** (`feature/short-description`).
- When a feature is complete: open a Pull Request, review diff, merge.
- Tag each milestone: `git tag milestone-1`, `git tag milestone-2`, etc.

### Why this structure?

- Forces review code before it lands
- Gives a clean history someone can actually read later
- The tags mean someone can always `git checkout milestone-1` and have a working app

---

## Project Structure (target state after Ch. 2)

```
doculith/
├── .github/
│   └── workflows/
│       └── ci.yml
├── cmake/
│   └── Dependencies.cmake
├── src/
│   ├── main.cpp
│   ├── Application.hpp / .cpp
│   ├── DocumentQueue.hpp / .cpp
│   ├── DocxConverter.hpp / .cpp
│   ├── PdfMerger.hpp / .cpp
│   └── ...
├── tests/
│   ├── smoke_test.cpp
│   ├── document_queue_tests.cpp
│   └── ...
├── CMakeLists.txt
├── .gitignore
└── README.md
```

---

## License

MIT
