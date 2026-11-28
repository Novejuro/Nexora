# Contributing

Nexora is an early-stage project maintained by one person. Feedback and contributions are welcome. Check the current implementation and open an issue before starting a large feature or proposing an architectural change. Review and response times may vary.

Use [GitHub Issues](https://github.com/Novejuro/Nexora/issues) to report bugs or discuss feature proposals. Follow [SECURITY.md](SECURITY.md) for private vulnerability reports.

## Development setup

Follow the [project status](README.md#project-status) and [build instructions](README.md#build) for your checkout. The migration build requires the Slint project files as well as all four initialized submodules. The instructions include compiler selection for a clean build and for an existing GCC checkout.

There is no Nexora application test suite yet. Build the application target before submitting a change. For a change to the Slint interface, start the executable, switch between Play, Installations, and Settings, resize the window, and close it. Record the platform, compiler, and checks you ran in the pull request.

## Pull requests

- Keep changes focused and explain the problem they solve.
- Update the README or architecture notes when setup, behavior, dependencies, or supported platforms change. Include the UI manifest, referenced components, and assets with changes that depend on them.
- Git pins SDL3, RmlUi, FreeType, and Slint as upstream submodules. Propose revision updates separately and explain the reason and license impact.
- Identify the source and distribution terms of new third-party code, fonts, and artwork, and preserve their notices.
- Do not commit build output, credentials, or personal information.
- Use an imperative Conventional Commit subject, for example `docs: clarify build requirements`.

By submitting your own work, you agree to contribute it under the project's [Apache License 2.0](LICENSE).
