# ADR-0007: Expected failures are values (`Result<T, E>`), exceptions are for the rest

| | |
|---|---|
| **Status** | Accepted |
| **Date** | 2026-11-16 |
| **Milestone** | M1 |
| **Implemented in** | Branch `m1/parser-inspect` (commits `2431c55`, `86f8e49`) |

## Context

Real log files contain broken lines: empty lines, truncated lines, bad timestamps, invalid bytes. In BGL these are *normal input*, not rare accidents, and the product must count them by reason (architecture §11).
C++ offers several ways to report failure: exceptions, error codes with output parameters, `std::optional`, and C++23 `std::expected`. LogGuard is on C++20 (ADR-0001), so `std::expected` is not available on every supported compiler.

## Decision

- **Expected, data-driven failures are returned as values** using `logguard::Result<T, E>` (`logguard/core/Result.hpp`): it holds either a `T` or an `E`. Errors are small `enum class` types (e.g. `bgl::ParseError`) with stable names for reports.
- `Result` follows the shape of `std::expected` (`HasValue()`, `Value()`, `Error()`, `Err{...}`), so it can be replaced by `std::expected` when the project moves to C++23.
- **Exceptions are reserved** for environment and programming failures (out of memory, broken invariants, calling `Value()` on an error). They are caught **once**, in `main`, which prints the message and exits with code 1.
- Problems the CLI can foresee (missing input file, bad arguments) are reported as messages plus documented exit codes (1 and 2), not exceptions.

## Alternatives considered

| Option | Pros | Cons | Why not chosen |
|---|---|---|---|
| Throw on every bad line | Familiar, little code | Slow and noisy for thousands of bad lines; easy to forget a `catch` | Bad lines are normal input, not exceptional |
| `std::optional<T>` | Standard, simple | Says *that* it failed, not *why*; we need counts by reason | Loses the error reason |
| `bool` + output parameter | No dependencies | Easy to ignore, awkward to read | Error-prone |
| `tl::expected` library | Full `std::expected` API | Extra dependency for ~40 lines of code | Our needs are small |
| Move to C++23 for `std::expected` | Standard type | Not yet supported on all compilers we target | Revisit later |

## Consequences

- **Positive:** every failure reason is explicit in the function's type; `[[nodiscard]]` makes ignoring a result a compiler warning; no exception cost per bad line.
- **Negative / costs:** callers must check the result before using it; a small home-grown type to maintain.
- **Follow-ups:** JSON/config loading (M2) and model loading (M4) use the same pattern with their own error enums.

## Validation

`ResultTest` covers values, errors, same-type `T`/`E`, and wrong-side access. The parser tests check every `ParseError`. Revisit when moving to C++23 (replace with `std::expected`).
