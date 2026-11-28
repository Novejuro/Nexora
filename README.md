# Nexora Launcher

Nexora is an independent, early-stage Minecraft launcher project. The current work focuses on the desktop interface. Account sign-in, game installation, Java runtime management, and game launching are still planned.

[![Made with Slint](https://raw.githubusercontent.com/slint-ui/slint/02bb0be28ad031bb84240ae752561f508e1e18e4/logo/MadeWithSlint-logo-whitebg.png)](https://slint.dev/)

## Project status

The Slint desktop interface has three tabs. Play displays a block-style landscape illustration. Installations and Settings currently show only their page headings. See the [architecture notes](docs/architecture.md) for how Nexora creates the window and which dependencies it builds.

## Build

Install Git, CMake 4.0 or newer, Ninja, a C++23-capable Clang toolchain, and Rust 1.95 or newer with Cargo. On Windows, the MSYS2 CLANG64 package `mingw-w64-clang-x86_64-rust` provides a compatible Rust compiler and Cargo. On macOS, use Apple Clang and the OpenGL framework from the macOS SDK. On Ubuntu, install `xorg-dev`, `libgl1-mesa-dev`, and `libfontconfig1-dev`.

If you previously built with GCC in this checkout, move or delete the old `build/` directory before configuring with Clang.

From the repository root, initialize the pinned dependencies and configure a fresh build tree. The shared Release preset selects Ninja. Pass both compiler names explicitly to select Clang.

```sh
git submodule update --init --recursive
cmake --preset Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cmake --build --preset Release --parallel
```

Expect network access on the first build for submodules, SDL3_image 3.4.8, Cargo dependencies, and Corrosion if a suitable installation is unavailable. A local SDL3_image source tree can be supplied with `-DFETCHCONTENT_SOURCE_DIR_SDL3_IMAGE=<path>`. The Release build preset targets `Nexora`. The executable is written to `build/Nexora` or `build/Nexora.exe`.

## Contributing

Read [CONTRIBUTING.md](CONTRIBUTING.md) for development and review expectations. Community participation is covered by the [Code of Conduct](CODE_OF_CONDUCT.md).

## License

Nexora's own code is licensed under the [Apache License 2.0](LICENSE). Third-party components and assets retain their respective terms.

| Component | Terms used by Nexora |
| --- | --- |
| Slint framework | [Slint Royalty-free Desktop, Mobile, and Web Applications License 2.0](https://github.com/slint-ui/slint/blob/02bb0be28ad031bb84240ae752561f508e1e18e4/LICENSES/LicenseRef-Slint-Royalty-free-2.0.md) for integration in the desktop application. The [upstream license overview](https://github.com/slint-ui/slint/blob/02bb0be28ad031bb84240ae752561f508e1e18e4/LICENSE.md) explains the GPLv3 and commercial alternatives. |
| SDL3 and SDL3_image | zlib license for [SDL3](https://github.com/libsdl-org/SDL/blob/c8da179691f909734cd1b124c930e49311428996/LICENSE.txt) and [SDL3_image](https://github.com/libsdl-org/SDL_image/blob/release-3.4.8/LICENSE.txt). |
| RmlUi | [MIT](https://github.com/mikke89/RmlUi/blob/92f0143424a417d6566a2cd037b747b636c6ca8a/LICENSE.txt). Additional notices apply to some bundled files. |
| FreeType | [FreeType License](https://github.com/freetype/freetype/blob/4fc22c56fca863d74aba9208c31e15aac4299d1d/docs/FTL.TXT). This software is based in part on the work of the FreeType Team (https://freetype.org). |
| Noto Sans CJK SC font | [SIL Open Font License 1.1](assets/font/OFL.txt). Retain the font's copyright notice and license when redistributing it. |

For Slint attribution, Nexora uses the upstream badge on its public project page. When publishing binaries, keep the badge easily visible on a public page, preferably the download page. The selected license covers Slint integrated in the desktop application and restricts standalone redistribution and applications that expose Slint APIs. Preserve upstream notices and include the licenses and acknowledgments required by the components actually shipped.

## Issues

Use [GitHub Issues](https://github.com/Novejuro/Nexora/issues) for bugs and feature proposals. Report vulnerabilities privately using [SECURITY.md](SECURITY.md).

## Disclaimer

Minecraft and all related names, trademarks, and intellectual property are owned by Mojang Studios and Microsoft. Nexora Launcher is an independent project and is not affiliated with, endorsed by, or sponsored by Mojang Studios or Microsoft.
