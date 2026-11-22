# Architecture

## Current status

Nexora is in the architecture and project-foundation stage. The repository establishes a CMake build, a C++23 language baseline, output-directory conventions, a Ninja Release preset, and source dependencies. The application entry point is currently a minimal console program. No desktop window, launcher UI, or Minecraft-related behavior has been implemented. The next stage is to begin application code on top of this foundation.

Dependency presence in the build does not imply application integration: the current application target does not yet use RmlUi or FreeType. There is no windowing or graphics dependency configured at the application level.

## Current build foundation

- CMake 4.0 is required by the root build.
- C++23 is required, with compiler extensions disabled.
- `CMakePresets.json` defines a Release preset using Ninja and selects `clang++`.
- Configure and build artifacts go under `build/obj/`; the application executable is emitted under `build/`.
- RmlUi and FreeType are included as Git submodules under `include/` and added to the build as source projects.
- The current `Nexora` executable only prints a startup message and exits.

The CI configurations are build checks, not a declared support matrix. Treat a platform as supported only after its build, runtime behavior, and any distribution requirements have been verified and documented.

## Dependencies

The repository currently includes two source submodules:

- RmlUi at `include/rmlui` (pinned at commit `92f0143424a417d6566a2cd037b747b636c6ca8a`; its upstream CMake project declares version 6.4 as a development version; MIT license in `include/rmlui/LICENSE.txt`).
- FreeType at `include/freetype` (Nexora elects to use it under the FreeType License, FTL; see `include/freetype/docs/FTL.TXT`). FreeType also offers GPLv2-or-later terms, which Nexora does not elect. Preserve its license and copyright notices when redistributing.

Their exact revisions are recorded by the Git submodule entries. Both are added through their upstream CMake projects, but the application has not yet connected to their targets. No application windowing or rendering backend has been selected.

The README includes the FreeType Team acknowledgment required by the FTL for binary redistribution documentation. Before distributing binaries, verify that the actual release package includes this documentation and the relevant upstream license and copyright notices. Review licensing obligations again when dependency revisions, build options, or the distribution model change.

## Working principles

The following are guidance for future implementation, not implemented architecture or finalized technology decisions. Keep the structure proportional to working behavior; avoid empty layers and interfaces created before a real need exists. Develop the application incrementally, with each new capability verifiable on its own.

As the application grows, keep presentation separate from launcher operations. UI code should present structured state and report user actions, while C++ application logic owns persistent state and coordinates workflows. Filesystem access, network operations, runtime discovery, and child-process management should remain outside presentation code. Introduce platform-specific boundaries where actual platform differences require them.

## Open decisions

The repository has not yet selected an application UI framework, windowing or rendering backend, or platform support matrix. Launcher behavior and design for accounts, instance and game installation, Java runtime handling, persistence, security, and game process lifecycle also remain undecided. Resolve these choices as implementation requirements become concrete, then update this document to record the decision and its rationale. Do not treat the working principles above as a commitment to a specific library or design.

## Validation

GitHub Actions currently builds the project on Windows (MSYS2/MinGW-w64) and Ubuntu Linux (GCC), then runs the console executable from outside the checkout. These jobs use the platform's default compiler; the separate `Release` CMake preset selects `clang++` and has not yet been validated across platforms. This CI coverage does not establish support for other operating systems or provide interactive launcher checks. Add focused tests and runtime checks as application behavior is implemented, and update the documented support matrix only after verifying the corresponding configurations.
