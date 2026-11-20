# Architecture

## Decision

Nexora Launcher will use **C++23**, **Qt 6**, **CMake**, and **QML (Qt Quick)** for its cross-platform desktop application. The target desktop platforms are Windows, Linux, and macOS.

This is the selected project direction, not a claim that platform support or packaging is already implemented. The repository is in early development and does not yet contain an application or build system.

## Goals

- Provide a polished desktop interface with responsive interaction and restrained resource use.
- Share application behavior across Windows, Linux, and macOS while isolating operating-system differences.
- Keep the launcher maintainable as game installation, configuration, and process-management capabilities are developed.
- Prefer established Qt and C++ facilities over introducing dependencies without a clear need.

## Technology Responsibilities

- **C++23** contains application and launcher-domain behavior, including configuration, filesystem operations, game preparation, and child-process management as those capabilities are implemented.
- **Qt 6** provides the application framework and cross-platform services. Qt Quick supplies the presentation runtime.
- **QML** defines the interface, visual states, and interaction. It should remain focused on presentation rather than owning complex launcher rules or filesystem and process operations.
- **CMake** defines the project build, discovers Qt modules, and supports platform-specific packaging workflows as they are added.

The QML interface communicates with C++ through explicit Qt interfaces such as registered types, models, properties, signals, and invokable operations. Keep those interfaces narrow and typed. C++ owns persistent state and operations with operating-system effects; QML presents state and requests user actions.

## Application Boundaries

Organize code around responsibilities rather than exposing platform APIs throughout the interface:

- **Presentation:** QML views and reusable UI components.
- **Application layer:** coordinates user actions and launcher workflows.
- **Domain layer:** represents launcher concepts and rules independently of QML details.
- **Platform and infrastructure layer:** implements filesystem, process, networking, and operating-system integration behind C++ interfaces where platform behavior differs.

These are responsibility boundaries, not a requirement to create a large framework before the corresponding behavior exists. Keep the initial project structure proportionate to implemented features.

The launcher should treat the Minecraft Java runtime as an external child process. Runtime discovery or provisioning, launch arguments, process lifetime, and output handling belong to the C++ application layer; the UI should receive structured state and progress updates.

## Cross-Platform Strategy

Use shared C++ and QML behavior by default. Isolate genuine operating-system differences in small platform-specific implementations and keep their contracts consistent. Use Qt abstractions when they cover the requirement; use native APIs only where needed and contain them behind those interfaces.

Validate the actual supported configurations before making compatibility claims. In particular, test packaging and launch behavior on Windows, macOS, and representative Linux environments, including relevant display-server and graphics configurations. Qt Quick depends on the installed graphics stack, and deployment requirements vary by platform and distribution.

## Build and Dependency Policy

CMake is the source of truth for configuring and building the application. Use Qt's supported CMake integration and declare required Qt modules explicitly. Pin or constrain Qt and compiler versions in the build and CI configuration once those are established.

Use C++23 as the language baseline, but keep required compiler features within the intersection supported by the chosen compiler versions on all target platforms. Avoid relying on optional or unevenly implemented standard-library features without verifying them in the platform build matrix.

Keep dependencies focused. Before adding a library, document the capability it supplies, its platform support, maintenance status, and licensing implications.

## Licensing and Distribution

Review the license terms for the selected Qt modules and every third-party dependency before distribution. Qt is available under multiple licensing models, and obligations depend on the modules, versions, and distribution choices. Document the project's chosen compliance approach before shipping binaries; this architecture decision does not itself determine a Qt license or distribution model.

## Validation Before Release

The following remain implementation and release work, not completed capabilities:

- Build and launch on Windows, Linux, and macOS using documented compiler and Qt versions.
- Verify QML module and Qt library deployment for each supported distribution format.
- Exercise UI rendering, accessibility-relevant scaling, and input behavior across the target environments.
- Test game child-process launch, exit, and error reporting without blocking the interface.
- Measure cold startup, idle memory use, and packaged size on representative hardware, then set project-specific targets from those results.
- Complete a Qt and dependency license review for the intended distribution model.
