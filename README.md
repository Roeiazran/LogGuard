# LogGuard

[![host-ci](https://github.com/Roeiazran/LogGuard/actions/workflows/host-ci.yml/badge.svg)](https://github.com/Roeiazran/LogGuard/actions/workflows/host-ci.yml)

A local-first **C++20** engine that detects anomalous behavior in system logs. Models are trained offline in Python and deployed with **ONNX Runtime**, so no Python is needed at runtime.

> **Status:** early development. M0 (build foundation) is complete; M1 (BGL parser) is next. See the [roadmap](docs/architecture.md#19-roadmap).

## What it does (planned)

```text
log file ─▶ parse & normalize ─▶ per-source windows ─▶ 72-feature vector ─▶ rules / ONNX model ─▶ alerts.jsonl
```

- Parses the [Loghub BGL](https://github.com/logpai/loghub) dataset with strict label isolation.
- Compares a tuned rules baseline, logistic regression and Random Forest on a chronological holdout.
- Runs the selected model natively in C++ with schema-checked model bundles.
- Bounded, deterministic concurrent pipeline with measured performance.
- **Extension:** ESP32-S3 / FreeRTOS device telemetry and on-device inference (after Core v1.0).

## Documentation

- [Architecture](docs/architecture.md)
- [Architecture Decision Records](docs/adr/README.md)
- [Getting Started](docs/getting-started.md)

## Build

Requires a C++20 compiler, CMake ≥ 3.25 and Ninja.

```bash
cmake --workflow --preset debug     # configure + build + test
./build/debug/apps/cli/logguard     # logguard 0.1.0
```

Presets: `debug`, `release`, `asan`, `tsan`, `lint`. See [Getting Started](docs/getting-started.md) for prerequisites, presets and troubleshooting.

## License

MIT
