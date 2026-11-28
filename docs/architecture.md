# Architecture

## Desktop interface

`Nexora::App::run()` calls `MainWindow::create()->run()` to create the Slint window and start its event loop.

Revision [cd706d5](https://github.com/Novejuro/Nexora/tree/cd706d5d07464a0dc150ddc0b61e1f7241de0005) used an SDL3 window with RmlUi's Vulkan backend. Commit [cbeb4b9](https://github.com/Novejuro/Nexora/commit/cbeb4b9bef82e07dbea58f753bf40add865b2b89) added the Slint submodule. Commit `4ba77a8` moved the window to Slint and switched the RmlUi backend linked by CMake to SDL3/OpenGL 3.

CMake passes the `assets/ui/` project to `slint_target_sources`. Its `slint-project.json` selects `MainWindow.slint` and defines search paths for components and SVGs. The build generates the C++ window interface used by `App.cpp`. The application target has no post-build step that copies the UI files or the Noto font beside the executable.

The window declares a preferred size of 1280 by 720 logical pixels and a minimum of 800 by 500. The toolbar updates the selected page in Slint. The account control emits a callback that the C++ application does not yet handle.

## Build and dependencies

The root CMake project requires CMake 4.0 and C++23 with compiler extensions disabled. The Release preset uses Ninja and configures in `build/obj/`. The [build instructions](../README.md#build) cover Clang selection and migration from an existing GCC build.

Git pins four submodules under `include/`. Their roles during migration are:

| Dependency | Role |
| --- | --- |
| Slint | Compiles the UI and provides the window runtime. Its C++ integration uses Corrosion and requires Rust 1.95 or newer with Cargo. |
| SDL3 | Provides the SDL main header and the native message-box helpers. The RmlUi backend also uses SDL3. |
| RmlUi | Its core and SDL/OpenGL 3 backend remain linked to the application target during migration. Window creation now goes through Slint. |
| FreeType | Built from source and made available to the dependency build as `Freetype::Freetype`. |

CMake also fetches SDL3_image 3.4.8 for the RmlUi backend's texture loader and verifies the archive's SHA-256 digest. The repository carries the Noto Sans CJK SC font under `assets/font/`. See the [license notes](../README.md#license) for redistribution terms.

## Launcher operations

Account management, installation, Java discovery, persistence, and game process management remain open design decisions. As these features are implemented, keep presentation in Slint and have C++ application logic coordinate filesystem access, network requests, and child processes.

## Validation

The workflow defines compilation jobs for Windows (MSYS2 CLANG64), Ubuntu, and macOS. The Ubuntu job explicitly selects `clang` for C and `clang++` for C++ when configuring the Release preset. Runtime support requires separate verification of window creation, tab navigation, input, display scaling, and shutdown on each platform.
