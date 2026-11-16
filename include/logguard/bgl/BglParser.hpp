#pragma once

#include "logguard/bgl/BglEvent.hpp"
#include "logguard/bgl/ParseError.hpp"
#include "logguard/core/Result.hpp"

#include <string_view>

namespace logguard::bgl {

// Parses one BGL line for the runtime engine. The label column is checked
// for presence but dropped: the returned event has no label (ADR-0006).
[[nodiscard]] Result<BglEvent, ParseError> ParseBglLine(std::string_view line);

}  // namespace logguard::bgl
