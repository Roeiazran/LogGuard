# ADR-0006: Label isolation at the parser boundary

| | |
|---|---|
| **Status** | Accepted |
| **Date** | 2026-11-16 |
| **Milestone** | M1 |
| **Implemented in** | Branch `m1/parser-inspect` (commits `f736e7f`, `df74a5a`) |

## Context

Every BGL line starts with a ground-truth label: `-` for a normal line, or an alert category such as `KERNDTLB`.
The label is what we evaluate against, so it must never influence detection. If it reaches feature extraction, even by accident (a debug field, a shortcut in a rule), every reported metric becomes meaningless. This is called *label leakage*, and it is one of the most common mistakes in log anomaly detection work.
Code review alone cannot reliably prevent it. A rule like "please don't read `event.label`" is easy to break later.

## Decision

LogGuard splits the label off **at the moment a line is parsed** and makes the runtime type unable to hold it.

- `BglEvent`, the type used by the runtime engine, has **no label field**.
- `ParseBglLine()` (`logguard/bgl/BglParser.hpp`) is the runtime parser. It checks that the label column exists, then drops it.
- `ParseLabeledBglLine()` (`logguard/bgl/LabeledBglParser.hpp`) is for **offline tools only** (evaluation, label export). It returns `LabeledBglEvent { BglEvent event; BglLabel label; }`.
- Both parsers share the same validation code, so they accept and reject exactly the same lines.
- A compile-time test (`static_assert` + concept) fails the build if anyone adds a `label` member to `BglEvent`.
- From M2, only offline commands (e.g. `export-labels`) may include `LabeledBglParser.hpp`. Detection code paths must not.

## Alternatives considered

| Option | Pros | Cons | Why not chosen |
|---|---|---|---|
| `std::optional<Label>` inside `BglEvent` | One type, simple | The label travels with every event; any code can read it | Leakage is one line of code away |
| Strip the label in a preprocessing script | Runtime never sees it | Two formats to maintain; tests must cover the script too | Splitting inside the parser is simpler and is tested in C++ |
| Separate offline reader library | Strongest separation (link-time) | Extra library for one function today | Can be done later without changing callers; revisit in M3 |

## Consequences

- **Positive:** leakage requires an explicit, reviewable include of the offline header; the type system and a test enforce the rule; runtime and offline parsing cannot drift apart.
- **Negative / costs:** two entry points to maintain; the offline label path is exercised only by tests until M3.
- **Follow-ups:** M2 `export-features` must use only `ParseBglLine`; M3 label export uses `ParseLabeledBglLine` and joins on window IDs.

## Validation

`LabeledBglParserTest` checks that both parsers produce the same event and errors, and that `BglEvent` has no label member (compile-time). Revisit if a second domain (Edge) needs labels at runtime, for example for online evaluation.
