#pragma once

#include "logguard/bgl/BglEvent.hpp"

#include <optional>
#include <string>
#include <string_view>

namespace logguard::bgl {

// Parses the BGL "Time" field: YYYY-MM-DD-HH.MM.SS.ffffff (e.g. 2005-06-03-15.42.50.363779).
// Returns std::nullopt for any other shape or an impossible date/time.
[[nodiscard]] std::optional<Timestamp> ParseBglTime(std::string_view text);

// Formats a timestamp as ISO 8601: 2005-06-03T15:42:50.363779
[[nodiscard]] std::string FormatTimestamp(Timestamp timestamp);

}  // namespace logguard::bgl
