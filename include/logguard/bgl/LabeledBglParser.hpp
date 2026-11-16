#pragma once

#include "logguard/bgl/BglEvent.hpp"
#include "logguard/bgl/ParseError.hpp"
#include "logguard/core/Result.hpp"

#include <string>
#include <string_view>

namespace logguard::bgl {

// OFFLINE USE ONLY (evaluation and training tools).
// Runtime code (parsing for detection) must use ParseBglLine from BglParser.hpp.

// Ground-truth label from the first BGL column: "-" means normal,
// anything else is the alert category (e.g. KERNDTLB).
struct BglLabel {
    std::string category;

    [[nodiscard]] bool IsAlert() const noexcept { return category != "-"; }
    bool operator==(const BglLabel&) const = default;
};

struct LabeledBglEvent {
    BglEvent event;
    BglLabel label;
};

[[nodiscard]] Result<LabeledBglEvent, ParseError> ParseLabeledBglLine(std::string_view line);

}  // namespace logguard::bgl
