# Contributing

Thanks for your interest in Nexora. Nexora is currently maintained by one person and is at an early stage. Contributions and thoughtful feedback are welcome; check the current implementation and open an issue before starting a large feature or proposing an architectural change. Review and response times may vary while the project is maintained by an individual.

Nexora uses [RmlUi](https://github.com/mikke89/RmlUi) and [FreeType](https://gitlab.freedesktop.org/freetype/freetype) as upstream source submodules. Thank you to their maintainers and contributors for making these projects available. Their source and notices remain under their respective licenses; see the dependency license files and the [project license notes](README.md#license).

Use [GitHub Issues](https://github.com/Novejuro/Nexora/issues) to report bugs or discuss feature proposals. Do not include security vulnerability details in public issues; follow [SECURITY.md](SECURITY.md) instead.

## Development setup

Install CMake 4.0 or newer, Ninja, and a C++23-capable compiler. The `Release` preset selects `clang++`, which must be available on `PATH`:

```sh
git clone --recurse-submodules https://github.com/Novejuro/Nexora.git
cd Nexora
cmake --preset Release
cmake --build --preset Release --parallel
```

If you already cloned without submodules, initialize them with:

```sh
git submodule update --init --recursive
```

The current executable is a console stub. There is no application test suite yet. Pull requests should at least build successfully; describe any manual checks in the pull request.

## Pull requests

- Keep changes focused and explain the problem they solve.
- Update the README or architecture documentation when setup, behavior, dependencies, or supported platforms change.
- Do not commit build output, credentials, or personal information.
- Treat `include/rmlui` and `include/freetype` as pinned upstream submodules. Propose revision updates separately and include the reason and license impact.
- Use a clear, imperative Conventional Commit subject, for example `docs: clarify build requirements`.

By submitting a contribution, you agree that it is provided under the project's Apache License 2.0, as described in [LICENSE](LICENSE).
