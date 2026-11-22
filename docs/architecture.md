# Architecture

## Current status

Nexora has a native desktop preview for version selection, game downloads, and mod downloads. It displays sample instances and catalogs. The preview adapter reports that launch and download operations are unavailable.

The RmlUi document and styles live under `assets/`. `LauncherUi` translates input into calls to `LauncherServices`. `PreviewServices` supplies sample data and feedback, and a future implementation can accept requests and queue application work in the background.

## Current build foundation

- CMake 4.0 is required by the root build.
- C++23 is required, with compiler extensions disabled.
- `CMakePresets.json` defines Release and Debug presets using Ninja and `clang++`.
- Release and Debug build trees are `build/obj/` and `build/obj-debug/`. The corresponding executables are emitted in `build/` and `build/debug/`.
- RmlUi and FreeType are included as Git submodules under `include/` and added to the build as source projects.
- The desktop preview uses the Win32 or X11 OpenGL backend from RmlUi and loads assets relative to the executable.

The CI configurations are build checks, not a declared support matrix. Treat a platform as supported only after its build, runtime behavior, and any distribution requirements have been verified and documented.

## Dependencies

The repository currently includes two source submodules:

- RmlUi at `include/rmlui` (pinned at commit `92f0143424a417d6566a2cd037b747b636c6ca8a`; its upstream CMake project declares version 6.4 as a development version; MIT license in `include/rmlui/LICENSE.txt`).
- FreeType at `include/freetype` (Nexora elects to use it under the FreeType License, FTL; see `include/freetype/docs/FTL.TXT`). FreeType also offers GPLv2-or-later terms, which Nexora does not elect. Preserve its license and copyright notices when redistributing.

Their exact revisions are recorded by the Git submodule entries. The desktop preview links RmlUi Core, which uses FreeType to render fonts. The preview packages the Lato fonts supplied with the RmlUi samples and their license.

The README includes the FreeType Team acknowledgment required by the FTL for binary redistribution documentation. Before distributing binaries, verify that the actual release package includes this documentation and the relevant upstream license and copyright notices. Review licensing obligations again when dependency revisions, build options, or the distribution model change.

## Working principles

The following are guidance for future implementation, not implemented architecture or finalized technology decisions. Keep the structure proportional to working behavior; avoid empty layers and interfaces created before a real need exists. Develop the application incrementally, with each new capability verifiable on its own.

As the application grows, keep presentation separate from launcher operations. UI code should present structured state and report user actions, while C++ application logic owns persistent state and coordinates workflows. Filesystem access, network operations, runtime discovery, and child-process management should remain outside presentation code. Introduce platform-specific boundaries where actual platform differences require them.

## Open decisions

A platform support matrix remains open. The service implementations for accounts, instance and game installation, Java runtime handling, persistence, security, and the game process lifecycle also remain open.

## Validation

The GitHub Actions workflow builds on Windows with MSYS2/MinGW-w64 and on Ubuntu with GCC. It runs the UI smoke check from outside the checkout and uses Xvfb on Linux. The check covers page rendering, layout, selection, catalog filters, and preview action feedback.
