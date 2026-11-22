# Architecture

## Current status

Nexora is in the early application-foundation stage. The repository establishes a CMake build, a C++23 language baseline, output-directory conventions, a Ninja Release preset, and source dependencies. The application opens an empty SDL3 window and renders an empty RmlUi context through Vulkan. Launcher UI and Minecraft-related behavior have not been implemented.

SDL3, RmlUi, FreeType, and the RmlUi SDL/Vulkan backend are connected to the application build. The initial window loop does not load a UI document.

## Current build foundation

- CMake 4.0 is required by the root build.
- C++23 is required, with compiler extensions disabled.
- `CMakePresets.json` defines a Release preset using Ninja and leaves compiler selection to the platform toolchain.
- Configure and build artifacts go under `build/obj/`; the application executable is emitted under `build/`.
- SDL3, RmlUi, and FreeType are included as Git submodules under `include/` and added to the build as source projects.
- The `Nexora` executable opens a resizable SDL3/Vulkan window at 1280 x 720 logical pixels, with an 800 x 500 logical-pixel minimum, and runs an empty RmlUi context. The RmlUi context tracks framebuffer pixel dimensions and the window's display scale.

The CI configurations are build checks, not a declared support matrix. Treat a platform as supported only after its build, runtime behavior, and any distribution requirements have been verified and documented.

## Dependencies

The repository currently includes three source submodules:

- SDL at `include/sdl` (pinned by its Git submodule entry; zlib license in `include/sdl/LICENSE.txt`).
- RmlUi at `include/rmlui` (pinned at commit `92f0143424a417d6566a2cd037b747b636c6ca8a`; its upstream CMake project declares version 6.4 as a development version; MIT license in `include/rmlui/LICENSE.txt`).
- FreeType at `include/freetype` (Nexora elects to use it under the FreeType License, FTL; see `include/freetype/docs/FTL.TXT`). FreeType also offers GPLv2-or-later terms, which Nexora does not elect. Preserve its license and copyright notices when redistributing.

Their exact revisions are recorded by the Git submodule entries. SDL3 is built as a source dependency. RmlUi core and the SDL/Vulkan backend are connected to the `Nexora` target. RmlUi samples are disabled in the application build.

The README includes the FreeType Team acknowledgment required by the FTL for binary redistribution documentation. Before distributing binaries, verify that the actual release package includes this documentation and the relevant upstream license and copyright notices. Review licensing obligations again when dependency revisions, build options, or the distribution model change.

## Working principles

The following are guidance for future implementation, not implemented architecture or finalized technology decisions. Keep the structure proportional to working behavior; avoid empty layers and interfaces created before a real need exists. Develop the application incrementally, with each new capability verifiable on its own.

As the application grows, keep presentation separate from launcher operations. UI code should present structured state and report user actions, while C++ application logic owns persistent state and coordinates workflows. Filesystem access, network operations, runtime discovery, and child-process management should remain outside presentation code. Introduce platform-specific boundaries where actual platform differences require them.

## Open decisions

Launcher behavior and design for accounts, instance and game installation, Java runtime handling, persistence, security, and game process lifecycle remain undecided. SDL3, Vulkan, and RmlUi are the current application foundation. Cross-platform builds are configured in CI, while interactive runtime behavior still needs verification on Windows, Linux, and macOS.

## Validation

GitHub Actions builds the project on Windows (MSYS2/MinGW-w64), Ubuntu Linux (GCC), and macOS (Apple Clang). These jobs validate compilation, not interactive Vulkan presentation. Verify window creation, input, DPI scaling, and shutdown on each platform before declaring runtime support.
