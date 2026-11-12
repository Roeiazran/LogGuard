# ADR-0005: CMake presets + pinned FetchContent dependencies

| | |
|---|---|
| **Status** | Accepted |
| **Date** | 2026-11-12 (proposed 2026-11-11) |
| **Milestone** | M0 |
| **Implemented in** | Branch `m0/build-foundation` (commits `f21c695` … `37d5370`) |

## Context

The project needs a cross-platform C++ build (Linux CI, macOS development) with several configurations (debug, release, sanitizers, static analysis), a handful of third-party libraries, and reproducible builds on a clean machine.
Dependencies are few and mostly small or header-only (GoogleTest, nlohmann/json, CLI11, spdlog). One is large and binary-heavy (ONNX Runtime) and is needed only from M4.

## Decision

- **Build system:** CMake ≥ 3.25 using target-based ("modern") CMake, with the Ninja generator. In-source builds are rejected.
- **Configurations:** `CMakePresets.json` defines `debug`, `release`, `asan`, `tsan`, and `lint`, plus workflow presets (configure + build + test in one command). Developers and CI use the same presets.
- **Dependencies:** fetched with `FetchContent` from an **exact release archive URL verified by SHA-256**, all declared in one file (`cmake/Dependencies.cmake`). A changed or tampered archive fails the configure step.
- **ONNX Runtime:** official prebuilt release archive, verified by checksum, behind `LOGGUARD_WITH_ONNX` (default `OFF`).
- **Tooling scope:** warnings, sanitizers and clang-tidy are applied **per target** through `logguard_configure_target()`, never globally through `CMAKE_CXX_FLAGS`, so third-party code is not affected.
- **Tool versions:** clang-format and clang-tidy versions are pinned in CI, so formatting results do not depend on the runner image.

## Alternatives considered

| Option | Pros | Cons | Why not chosen |
|---|---|---|---|
| vcpkg (manifest mode) | Large catalog, binary caching | Extra tool and bootstrap; slow ONNX Runtime source build | Overhead not justified for ~5 deps; revisit if deps grow |
| Conan | Binary packages, profiles | Python tooling, profiles learning curve | Same as above |
| Git submodules | Explicit versions | Manual updates; nested builds; clone friction | FetchContent achieves the same with less friction |
| System packages (brew/apt) | No download at build | Different versions per machine ⇒ not reproducible | Violates NFR-7 |
| Bazel / Meson | Fast, hermetic (Bazel) | Less common in C++ job market and tooling | CMake is the de facto standard |
| FetchContent with `GIT_TAG` | Familiar | Tags can be moved; needs git; slower clone | Archive + SHA-256 is verifiable and faster |

## Consequences

- **Positive:** `cmake --workflow --preset debug` works on any machine with CMake, Ninja and a compiler; dependency versions are visible in one file; IDEs, clangd and clang-tidy read presets and `compile_commands.json`.
- **Negative:** first configure downloads sources (needs network); dependency source builds add to clean-build time. On macOS, Homebrew's clang-tidy needs the SDK path passed explicitly (handled in `cmake/ClangTidy.cmake`).
- **Follow-ups:** cache `_deps` in CI if build time grows; revisit this ADR if the project needs more than ~8 dependencies.

## Validation

CI on clean `ubuntu-24.04` (GCC 14, Clang 18) and `macos-15` runners configures, builds and tests using only CMake, Ninja and a compiler, with warnings as errors. Sanitizer and clang-tidy jobs use the same presets as local development.
