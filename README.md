# LogGuard

[![host-ci](https://github.com/Roeiazran/LogGuard/actions/workflows/host-ci.yml/badge.svg)](https://github.com/Roeiazran/LogGuard/actions/workflows/host-ci.yml)

A local-first **C++20** engine that detects anomalous behavior in system logs. Models are trained offline in Python and deployed with **ONNX Runtime**, so no Python is needed at runtime.

> **Status:** early development. M1 (BGL parser and `logguard inspect`) is complete; M2 (feature engine) is next. See the [roadmap](docs/architecture.md#19-roadmap).

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
- [Input Formats](docs/input-formats.md)

## Build

Requires a C++20 compiler, CMake ≥ 3.25 and Ninja.

```bash
cmake --workflow --preset debug               # configure + build + test
./build/debug/apps/cli/logguard --version     # logguard 0.1.0
```

Presets: `debug`, `release`, `asan`, `tsan`, `lint`. See [Getting Started](docs/getting-started.md) for prerequisites, presets and troubleshooting.

## Usage

`inspect` parses a log file and prints a summary. Bad lines are counted by reason instead of stopping the run:

```console
$ ./build/debug/apps/cli/logguard inspect --input data/fixtures/bgl-small.log --format bgl
{
  "format": "bgl",
  "lines": {
    "total": 19,
    "valid": 13,
    "invalid": 6
  },
  "invalid_by_reason": {
    "empty_line": 1,
    "too_long": 0,
    "missing_fields": 3,
    "bad_timestamp": 2,
    "invalid_utf8": 0
  },
  "unique_sources": 9,
  "time_range": {
    "earliest": "2005-06-03T15:42:50.363775",
    "latest": "2005-12-31T23:59:59.999999"
  }
}
```

Exit codes: `0` success, `1` cannot read input, `2` invalid arguments. The accepted BGL format is described in [Input Formats](docs/input-formats.md).

## License

MIT
