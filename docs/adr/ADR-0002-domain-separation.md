# ADR-0002: BGL logs and Edge telemetry are separate domains

| | |
|---|---|
| **Status** | Accepted |
| **Date** | 2026-11-11 |
| **Milestone** | M0 |
| **Implemented in** | — |

## Context

LogGuard analyzes text system logs (BGL) now, and later numeric device telemetry from an ESP32-S3 (Edge). Both turn into float vectors that are scored by a detector, so it is tempting to share one feature pipeline or even one model.

They differ in every way that matters to correctness: the unit of grouping (event count per node vs. time window per device and boot), the features (template frequencies vs. sensor statistics and health counters), the label source (dataset labels vs. fault-injection logs), and the split strategy (chronological vs. episode-aware).

## Decision

Each domain has its **own** parser/decoder, window builder, feature extractor, feature schema ID, training pipeline, and models.
Domains share only generic infrastructure: `FeatureWindow`, `IDetector`, `AlertEngine`, `BoundedQueue`, sinks, and the CLI shell.

Every detector declares the schema ID it accepts. The pipeline refuses to pair a window with a detector whose schema differs.

## Alternatives considered

| Option | Pros | Cons | Why not chosen |
|---|---|---|---|
| One generic "feature vector" pipeline | Less code | Silent misuse: a 72-float BGL model would accept any 72 floats | Correctness risk |
| One model trained on both domains | Single artifact | Meaningless: features have unrelated semantics | Invalid ML |

## Consequences

- **Positive:** a wrong model/domain pairing is a startup error, not a silent wrong answer. Each domain can evolve its schema independently.
- **Negative:** some structural duplication between `logguard_bgl` and (future) `logguard_edge`.
- **Follow-ups:** schema ID + hash check in `OnnxDetector` (ADR-0011).

## Validation

A test that loads a model whose manifest declares another schema and asserts exit code 4.
