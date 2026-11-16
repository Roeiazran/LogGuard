#pragma once

#include <chrono>
#include <string>

namespace logguard::bgl {

// Microsecond-precision point in time taken from the BGL "Time" field.
using Timestamp = std::chrono::sys_time<std::chrono::microseconds>;

// One valid BGL log line, as seen by the runtime engine.
// It deliberately has NO label field: the ground-truth label never reaches
// detection code (ADR-0006). Offline tools use LabeledBglEvent instead.
struct BglEvent {
    Timestamp timestamp;
    std::string source_id;  // BGL "Node", e.g. R02-M1-N0-C:J12-U11
    std::string component;  // e.g. KERNEL, APP, MMCS
    std::string level;      // e.g. INFO, ERROR, FATAL
    std::string message;    // free text after the fixed fields

    bool operator==(const BglEvent&) const = default;
};

}  // namespace logguard::bgl
