# ADR-0001: C++20 owns the runtime; Python is offline-only

| | |
|---|---|
| **Status** | Accepted |
| **Date** | 2026-11-11 |
| **Milestone** | M0 |
| **Implemented in** | — |

## Context

LogGuard must process large log files (BGL ≈ 4.7M lines) with predictable memory, measurable throughput, and bounded concurrency. It also needs machine-learning models, and the ML ecosystem (scikit-learn, pandas) is Python-first.

Running Python in the analysis path would add interpreter start-up, GIL limits on parallel work, a heavy deployment footprint, and a second place where features could be computed differently from training.

## Decision

The runtime (parsing, windowing, feature extraction, detection, alerting, I/O) is written in **C++20**.
Python is used **only offline** for training, evaluation, and exporting models to **ONNX**. The C++ runtime loads the exported model through ONNX Runtime, and no Python interpreter is needed at analysis time.

Training features are produced by the C++ extractor (`logguard export-features`), so Python never re-implements feature logic.

## Alternatives considered

| Option | Pros | Cons | Why not chosen |
|---|---|---|---|
| All Python | Fastest to prototype; one language | Throughput/concurrency limits; heavy runtime; weak control over memory | Does not meet performance or deployment goals |
| C++ runtime calling embedded Python | Reuse sklearn models directly | Interpreter embedding complexity; GIL; deployment of Python env | Couples runtime to Python |
| C++17 | Wider compiler support | No `std::span`, concepts, `std::jthread`, ranges, chrono calendar types | C++20 is well supported by gcc ≥ 12 / clang ≥ 16 |
| Rust | Memory safety | Less mature ONNX/ML ecosystem; project goal includes modern C++ | Out of scope for this project's goals |

## Consequences

- **Positive:** predictable performance and memory; single static-ish binary; clear separation between experimentation and production.
- **Negative:** two languages and two toolchains; model export can fail for unsupported estimators; parity between Python and C++ must be tested.
- **Follow-ups:** ONNX parity tests (ADR-0011); `export-features` command (M2).

## Validation

`logguard analyze --model …` runs on a machine without Python installed. Golden-vector parity tests pass in CI.
