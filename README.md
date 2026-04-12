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

## Branching Strategy

```
main
 └── dev
      └── feature/cmake-setup
      └── feature/hello-window
      └── feature/file-picker
      └── ...
```

- **`main`** is always buildable. Never commit directly to it.
- **`dev`** is the development branch.
- Every **feature on a branch** (`feature/short-description`) is branched off from dev.
- When a feature is complete: open a Pull Request, review diff, merge to dev, then to main.
- Tag each release: `git tag v1.0.0`, `git tag v1.1.0`, etc.

### Why this structure?

- Forces review code before it lands
- Gives a clean history that someone can actually read later
- The tags mean someone can always `git checkout v1.0.0` and have a working app

---

## Project Structure

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
