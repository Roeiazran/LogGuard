# Architecture Decision Records

An ADR records **one** significant decision: the context that forced it, the options considered, what was chosen, and the consequences accepted.
ADRs are immutable once accepted. To change a decision, write a new ADR that *supersedes* the old one.

## When to write one

Write an ADR when a decision:

- is hard or expensive to reverse (language, build system, data contracts, threading model),
- affects more than one component, or
- would make a new contributor ask "why is it done this way?"

## Process

1. Copy [`template.md`](template.md) to `ADR-NNNN-short-title.md` (next free number).
2. Status `Proposed` while under discussion; `Accepted` when merged with the implementing change.
3. Fill in the implementing commit / PR once it exists.
4. Add a row to the index below.

## Index

| ADR | Title | Status | Milestone |
|---|---|---|---|
| [0001](ADR-0001-cpp20-host-python-offline.md) | C++20 owns the runtime; Python is offline-only | Accepted | M0 |
| [0002](ADR-0002-domain-separation.md) | BGL logs and Edge telemetry are separate domains | Accepted | M0 |
| [0003](ADR-0003-cli-first.md) | CLI-first delivery before any HTTP server or UI | Accepted | M0 |
| [0004](ADR-0004-core-first-hardware-extension.md) | Core first; embedded hardware is an extension | Accepted | M0 |
| [0005](ADR-0005-build-system-and-dependencies.md) | CMake presets + pinned FetchContent dependencies | Accepted | M0 |
| 0006 | Label isolation at the parser boundary | Planned | M1 |
| 0007 | Error-handling strategy (result types vs. exceptions) | Planned | M1 |
| 0008 | Template extraction method and vocabulary fitting | Planned | M2 |
| 0009 | Feature schema versioning and golden vectors | Planned | M2 |
| 0010 | Chronological split and window labeling rule | Planned | M3 |
| 0011 | Model bundle format, manifest validation, parity tolerance | Planned | M4 |
| 0012 | Bounded queues, source partitioning, deterministic output | Planned | M5 |
