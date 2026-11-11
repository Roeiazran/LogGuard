# ADR-0004: Core first; embedded hardware is an extension

| | |
|---|---|
| **Status** | Accepted |
| **Date** | 2026-11-11 |
| **Milestone** | M0 |
| **Implemented in** | — |

## Context

The full vision includes ESP32-S3 firmware, fault injection, and on-device inference. Doing hardware in parallel with the core engine would split attention across two toolchains (host CMake and ESP-IDF), add physical dependencies (board, sensor, wiring), and risk leaving both halves unfinished.

The Edge track also depends on Core components: detectors, alert engine, bounded queues, model bundles, and the CLI.

## Decision

Deliver **LogGuard Core** (milestones M0–M5, tag `v1.0.0`) first.
The Edge track (X1–X3) starts only after Core v1.0 and is designed as an **extension**: a new domain library (`logguard_edge`), a new transport abstraction, and a separate firmware project. It must not require changes to Core domain code beyond adding new adapters.

## Alternatives considered

| Option | Pros | Cons | Why not chosen |
|---|---|---|---|
| Parallel Core + Edge | Hardware demo earlier | Two half-finished systems; context switching | Higher delivery risk |
| Edge first | Eye-catching | Builds on interfaces that don't exist yet | Wrong dependency order |
| Drop Edge entirely | Smaller scope | Loses the embedded part of the project | Kept as a planned extension |

## Consequences

- **Positive:** a complete, tested product exists early (`v0.1.0` MVP at M4). Core interfaces are proven before a second domain uses them.
- **Negative:** no hardware demo until after Core v1.0.
- **Follow-ups:** keep Core interfaces domain-agnostic (`IDetector`, `IEventSink`, `BoundedQueue`) so Edge plugs in without refactoring.

## Validation

When X1 starts, the Edge host path is added without modifying `logguard_bgl` and with only additive changes to `logguard_core`.
