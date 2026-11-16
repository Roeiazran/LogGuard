#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace logguard::bgl {

// Why a line could not be turned into a BglEvent. Values, not exceptions (ADR-0007).
enum class ParseError : std::uint8_t {
    kEmptyLine,
    kTooLong,
    kMissingFields,
    kBadTimestamp,
    kInvalidUtf8,
};

// Every error, in a fixed order. Used to print reports in a stable order.
inline constexpr std::array kAllParseErrors{
    ParseError::kEmptyLine,    ParseError::kTooLong,     ParseError::kMissingFields,
    ParseError::kBadTimestamp, ParseError::kInvalidUtf8,
};

// Stable snake_case name used in reports, e.g. "missing_fields".
[[nodiscard]] std::string_view ToString(ParseError error) noexcept;

}  // namespace logguard::bgl
