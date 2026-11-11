# ADR-0003: CLI-first delivery before any HTTP server or UI

| | |
|---|---|
| **Status** | Accepted |
| **Date** | 2026-11-11 |
| **Milestone** | M0 |
| **Implemented in** | — |

## Context

The value of LogGuard is the engine: correct parsing, valid features, honest detection, and bounded performance. A web server and dashboard add a lot of surface area (HTTP framework, job management, uploads, auth, frontend build) that does not improve the engine and can hide whether it works.

## Decision

The first and primary interface is the `logguard` **command-line tool**. All logic lives in libraries. The CLI only parses arguments, loads configuration, wires components, and maps outcomes to exit codes.
An HTTP server, if built later, is another thin adapter that links the same libraries and owns no business logic.

## Alternatives considered

| Option | Pros | Cons | Why not chosen |
|---|---|---|---|
| Web app first | Visual, demo-friendly | Large scope before the engine is validated; harder to test deterministically | Delays the core |
| Library only, no CLI | Smallest surface | Not runnable by reviewers; no end-to-end tests | Not a usable product |

## Consequences

- **Positive:** scriptable, deterministic, easy to test end-to-end in CI; reproducible runs via `run-manifest.json`.
- **Negative:** less visual for demos until the optional web milestone.
- **Follow-ups:** stable exit codes and output formats (architecture §8, §12).

## Validation

Integration tests drive the CLI binary on fixtures and compare outputs to expected files.
