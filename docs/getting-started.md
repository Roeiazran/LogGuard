# Getting Started

How to build, test and develop LogGuard on a fresh machine.

## 1. Prerequisites

| Tool | Version | macOS (Homebrew) | Ubuntu 24.04 |
|---|---|---|---|
| C++20 compiler | GCC ≥ 12, Clang ≥ 16, or Apple clang | Xcode Command Line Tools: `xcode-select --install` | `sudo apt install g++-14` or `clang-18` |
| CMake | ≥ 3.25 | `brew install cmake` | `sudo apt install cmake` |
| Ninja | any recent | `brew install ninja` | `sudo apt install ninja-build` |
| clang-format / clang-tidy | 23.x (optional, for formatting and linting) | `brew install llvm` | `pipx install clang-format==23.1.3 clang-tidy==23.1.3` |

On macOS, Homebrew's LLVM tools are not on `PATH` by default:

```bash
echo 'export PATH="/opt/homebrew/opt/llvm/bin:$PATH"' >> ~/.zshrc
```

Do not put Homebrew's `clang++` first when building; the presets use the system compiler (Apple clang) unless `CXX` is set.

## 2. Build and test

```bash
git clone https://github.com/Roeiazran/LogGuard.git
cd LogGuard
cmake --workflow --preset debug     # configure + build + test
./build/debug/apps/cli/logguard     # prints: logguard 0.1.0
```

The first configure downloads GoogleTest (network required). Build output goes to `build/<preset>/`, which is safe to delete at any time.

Step by step (useful while developing):

```bash
cmake --preset debug            # configure (only needed once, or after CMake changes)
cmake --build --preset debug    # build what changed
ctest --preset debug            # run tests
```

## 3. Presets

| Preset | Use it for |
|---|---|
| `debug` | Daily development |
| `release` | Optimized build, benchmarks |
| `asan` | Find memory errors and undefined behavior: `cmake --workflow --preset asan` |
| `tsan` | Find data races: `cmake --workflow --preset tsan` |
| `lint` | Run clang-tidy on all LogGuard code: `cmake --preset lint && cmake --build --preset lint` |

To choose a compiler, set `CXX` before the first configure of a preset, e.g. `CXX=clang++-18 cmake --preset debug`.

## 4. Before opening a pull request

```bash
scripts/format.sh                                   # format code
cmake --preset debug -DLOGGUARD_WARNINGS_AS_ERRORS=ON
cmake --build --preset debug && ctest --preset debug
cmake --workflow --preset asan                      # sanitizer run
```

CI runs the same checks (see [architecture §16.5](architecture.md#165-ci-quality-gates-github-actions)).

## 5. Repository layout

```text
apps/cli/        logguard executable
include/logguard public headers, one folder per library
src/             library implementations (mirrors include/)
tests/           GoogleTest suites (mirrors src/)
cmake/           build helpers: warnings, sanitizers, clang-tidy, dependencies
scripts/         developer scripts (format.sh)
docs/            architecture, ADRs, guides
```

## 6. Troubleshooting

| Symptom | Fix |
|---|---|
| `In-source builds are not allowed` | Use a preset or `-B build/<name>`; delete any `CMakeCache.txt` / `CMakeFiles/` created in the repo root. |
| A changed option seems ignored | Options are cached per build directory. Pass `-D…` again or delete `build/<preset>/`. |
| clang-tidy: `'string_view' file not found` (macOS) | Configure through the `lint` preset or `LOGGUARD_ENABLE_CLANG_TIDY=ON`; CMake passes the macOS SDK path automatically. |
| TSan: `unexpected memory mapping` (Linux) | Newer kernels use more address randomization than TSan supports: `sudo sysctl vm.mmap_rnd_bits=28`. |
| `clang-format` reports differences that CI does not (or vice versa) | Use clang-format 23.x, the version pinned in CI. |
