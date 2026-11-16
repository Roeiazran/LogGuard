# Input Formats

How LogGuard reads each supported log format, and exactly which lines it accepts.
The parser code is the reference: `src/bgl/BglParser.cpp` and `src/bgl/BglTimestamp.cpp`.

## BGL (`--format bgl`)

Source: [Loghub BGL](https://github.com/logpai/loghub), Blue Gene/L supercomputer logs.

### Line layout

Nine fixed fields separated by **single spaces**, followed by free text:

```text
Label Timestamp Date Node Time NodeRepeat Type Component Level Content...
- 1117838570 2005.06.03 R02-M1-N0-C:J12-U11 2005-06-03-15.42.50.363779 R02-M1-N0-C:J12-U11 RAS KERNEL INFO instruction cache parity error corrected
```

| # | Field | Example | Used as | Validated |
|---|---|---|---|---|
| 0 | Label | `-`, `KERNDTLB` | Ground truth, **offline tools only** ([ADR-0006](adr/ADR-0006-label-isolation.md)) | Present |
| 1 | Timestamp | `1117838570` | Not used (second resolution only) | Present |
| 2 | Date | `2005.06.03` | Not used (duplicate of Time) | Present |
| 3 | Node | `R02-M1-N0-C:J12-U11` | `BglEvent::source_id` | Present |
| 4 | Time | `2005-06-03-15.42.50.363779` | `BglEvent::timestamp` | Strict format and real date/time |
| 5 | NodeRepeat | `R02-M1-N0-C:J12-U11` | Not used | Present |
| 6 | Type | `RAS` | Not used | Present |
| 7 | Component | `KERNEL` | `BglEvent::component` | Present |
| 8 | Level | `INFO` | `BglEvent::level` | Present |
| 9+ | Content | `instruction cache …` | `BglEvent::message` (spaces kept as-is) | May be empty |

Node values are opaque strings: `NULL` and `UNKNOWN_LOCATION` appear in the real dataset and are accepted as ordinary sources.

### Validation rules

Checks run in this order; the first failure decides the reason.

| Order | Rule | Error reason (`inspect` output) |
|---|---|---|
| 1 | Line is at most 8192 bytes (excluding the line ending) | `too_long` |
| 2 | Line is not empty | `empty_line` |
| 3 | Line is valid UTF-8 | `invalid_utf8` |
| 4 | Nine non-empty fields before the content; a double space between fixed fields counts as a missing field | `missing_fields` |
| 5 | Time is exactly `YYYY-MM-DD-HH.MM.SS.ffffff` (26 characters, digits only), with a real calendar date, hour ≤ 23, minute ≤ 59, second ≤ 59 | `bad_timestamp` |

Line endings `\n` and `\r\n` are both accepted. A last line without a newline is still read.

### Time

The `Time` field has microsecond resolution and is used for ordering and time ranges. BGL does not record a time zone; LogGuard treats the value as-is (no zone conversion) and prints it in ISO 8601 form: `2005-06-03T15:42:50.363779`.

### Labels

`-` means a normal line; any other value is an alert category. Labels are never stored in runtime types. See [ADR-0006](adr/ADR-0006-label-isolation.md).

### Test fixture

`data/fixtures/bgl-small.log` (19 lines) covers normal and alert-labeled lines, an empty line, a truncated line, a double space, an impossible date (June 31), a wrong time format, an empty message, and an out-of-order earliest timestamp. Its expected `inspect` output is `tests/integration/inspect-bgl-small.expected.json`.
