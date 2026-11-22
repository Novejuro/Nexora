# Nexora Launcher

Nexora Launcher is an independent early-stage project for Minecraft Java Edition. The desktop preview includes version selection, a game catalog, and a mod catalog. The catalogs use sample data, and launch and download actions display preview feedback.

## Project status

The desktop preview uses RmlUi and FreeType with Win32 or Linux X11 and OpenGL. The C++ launcher service interface provides integration points for listing instances and catalogs, launching games, and downloading games and mods.

See [docs/architecture.md](docs/architecture.md) for the current foundation, intended boundaries, and work remaining before feature development.

## Build

Requirements:

- CMake 4.0 or newer
- Ninja
- A C++23-capable compiler (`clang++` is selected by the supplied preset)
- Initialized Git submodules (`include/rmlui` and `include/freetype`)
- OpenGL development files and, on Linux, X11 development files

On Windows with Visual Studio Build Tools, run these commands in a developer shell with Clang and Ninja on `PATH`.

Configure and build the Release preset from the repository root:

```sh
cmake --preset Release
cmake --build --preset Release --parallel
```

Run `build/Nexora` (or `build/Nexora.exe` on Windows) to open the preview. Keep the generated `assets/` directory next to the executable.

For a Debug build, run `cmake --preset Debug` and `cmake --build --preset Debug --parallel`. The executable is written to `build/debug/`.

To stage the Release preview for a ZIP archive, run `cmake --install build/obj --component Nexora --prefix build/package/Nexora`. The staged directory contains the executable, assets, and license notices.

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
