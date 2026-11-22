# Nexora Launcher

Nexora Launcher is an independent early-stage project for a Minecraft launcher. Its current application opens an empty SDL3 window rendered through RmlUi's Vulkan backend. Launcher interface and Minecraft workflows have not been implemented yet.

## Project status

The repository has a CMake-based C++23 build, a Release preset using Ninja and the platform's default compiler, and Git submodules for SDL3, RmlUi, and FreeType. Nexora uses SDL3 for its window and RmlUi's SDL/Vulkan backend for rendering. Launcher layers, account management, game installation, Java runtime handling, and game launching have not been implemented.

See [docs/architecture.md](docs/architecture.md) for the current foundation, intended boundaries, and work remaining before feature development.

## Build

Requirements:

- CMake 4.0 or newer
- Ninja
- A C++23-capable compiler
- Vulkan-capable graphics support at runtime. macOS requires a Vulkan implementation such as MoltenVK.
- Initialized Git submodules (`include/sdl`, `include/rmlui`, and `include/freetype`)

Configure and build the Release preset from the repository root:

```sh
cmake --preset Release
cmake --build --preset Release --parallel
```

The build tree is `build/obj/`. The executable is written to `build/Nexora` (or `build/Nexora.exe` on Windows). Running it opens an empty, resizable window at 1280 x 720 logical pixels, with a minimum size of 800 x 500 logical pixels. Windows builds use the GUI subsystem and do not open a console window. The Release build preset builds the Nexora application target only.

If the submodules were not initialized during clone, run:

```sh
git submodule update --init --recursive
```

## Contributing

Contributions are welcome. Read [CONTRIBUTING.md](CONTRIBUTING.md) before opening an issue or pull request. Community participation is covered by the [Code of Conduct](CODE_OF_CONDUCT.md); please report security issues using [SECURITY.md](SECURITY.md).

## License

This project is licensed under the [Apache License 2.0](LICENSE). Third-party components have their own license terms: RmlUi's main license is [MIT](include/rmlui/LICENSE.txt), and FreeType is used under the [FreeType License (FTL)](include/freetype/docs/FTL.TXT). Some dependency files have additional notices retained alongside their source. This software is based in part on the work of the FreeType Team (https://freetype.org). Preserve the relevant upstream license and copyright notices when redistributing these components.

## Issues

Use [GitHub Issues](https://github.com/Novejuro/Nexora/issues) for bugs and feature proposals. Do not post security vulnerability details publicly; see [SECURITY.md](SECURITY.md) for private reporting.

## Disclaimer

Minecraft and all related names, trademarks, and intellectual property are owned by Mojang Studios and Microsoft. Nexora Launcher is an independent project and is not affiliated with, endorsed by, or sponsored by Mojang Studios or Microsoft.
