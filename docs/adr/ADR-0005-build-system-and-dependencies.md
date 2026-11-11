# ADR-0005: CMake presets + pinned FetchContent dependencies

| | |
|---|---|
| **Status** | Proposed — to be accepted with the M0 build setup |
| **Date** | 2026-11-11 |
| **Milestone** | M0 |
| **Implemented in** | — |

## Context

The project needs a cross-platform C++ build (Linux CI, macOS development) with several configurations (debug, release, sanitizers), a handful of third-party libraries, and reproducible builds on a clean machine.
Dependencies are few and mostly small or header-only (GoogleTest, nlohmann/json, CLI11, spdlog). One is large and binary-heavy (ONNX Runtime) and is needed only from M4.

## Decision

- **Build system:** CMake ≥ 3.25 using target-based ("modern") CMake, with the Ninja generator.
- **Configurations:** `CMakePresets.json` defines `debug`, `release`, `asan`, `tsan`. Developers and CI use the same presets.
- **Dependencies:** fetched with `FetchContent`, **pinned to exact release tags** (later: commit hashes), in one place (`cmake/Dependencies.cmake`).
- **ONNX Runtime:** official prebuilt release archive, verified by checksum, behind `LOGGUARD_WITH_ONNX` (default `OFF`).
- **Warnings / sanitizers:** applied via helper functions on our own targets only, never globally through `CMAKE_CXX_FLAGS`.

## Alternatives considered

| Option | Pros | Cons | Why not chosen |
|---|---|---|---|
| vcpkg (manifest mode) | Large catalog, binary caching | Extra tool and bootstrap; slow ONNX Runtime source build | Overhead not justified for ~5 deps; revisit if deps grow |
| Conan | Binary packages, profiles | Python tooling, profiles learning curve | Same as above |
| Git submodules | Explicit versions | Manual updates; nested builds; clone friction | FetchContent achieves the same with less friction |
| System packages (brew/apt) | No download at build | Different versions per machine ⇒ not reproducible | Violates NFR-7 |
| Bazel / Meson | Fast, hermetic (Bazel) | Less common in C++ job market and tooling | CMake is the de facto standard |

## Consequences

- **Positive:** one command sequence works on any machine with CMake + a compiler; versions are visible in one file; IDEs and clangd read presets and `compile_commands.json`.
- **Negative:** first configure downloads sources (needs network); dependency source builds add to clean-build time.
- **Follow-ups:** CI caches the `_deps` directory; ADR revisited if adding more than ~8 dependencies.

## Validation

CI on a clean `ubuntu-latest` runner configures, builds, and tests with only `cmake`, `ninja`, and a compiler installed.
