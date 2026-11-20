# Agent Instructions

## Project Context

Nexora Launcher is an independent, early-stage project focused on a launcher for Minecraft. Treat the repository and its documented decisions as the source of truth: review them before proposing or making changes, and do not assume product capabilities or technical choices that are not supported by the code or documentation.

## Working Guidelines

- Keep changes focused on the requested task and follow established patterns as the project grows.
- Prefer clear, maintainable solutions over unnecessary abstractions or dependencies.
- Update documentation when user-visible behavior, setup, configuration, or supported platforms change.
- Never add secrets, credentials, private keys, personal information, or confidential data to committed files.
- Keep repository content, comments, and commit messages in English.

## Verification

- Before running checks, inspect the available project scripts and tooling; use the checks that actually exist.
- Add or update tests for behavior changes when a test framework is available.
- Report the checks run and any checks that could not be run. Do not claim unverified behavior.

## Commits

Use [Conventional Commits](https://www.conventionalcommits.org/) with the format `<type>[optional scope]: <description>`.

Common types include `feat`, `fix`, `docs`, `refactor`, `test`, `build`, and `chore`. Keep the subject concise and imperative. Add a body when context is useful, and document breaking changes with a `BREAKING CHANGE:` footer.
