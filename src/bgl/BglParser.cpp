#include "logguard/bgl/BglParser.hpp"

#include "logguard/bgl/BglTimestamp.hpp"
#include "logguard/core/Utf8.hpp"

#include <array>
#include <cstddef>
#include <string>

namespace logguard::bgl {
namespace {

// A BGL line has 9 fixed, space-separated fields followed by free text:
// Label Timestamp Date Node Time NodeRepeat Type Component Level Content...
constexpr std::size_t kFixedFields = 9;
constexpr std::size_t kNode = 3;
constexpr std::size_t kTime = 4;
constexpr std::size_t kComponent = 7;
constexpr std::size_t kLevel = 8;

struct SplitLine {
    std::array<std::string_view, kFixedFields> fields;
    std::string_view content;
};

// Cuts the line into its fields without copying (string_view points into `line`).
Result<SplitLine, ParseError> Split(std::string_view line) {
    SplitLine split;
    std::string_view rest = line;
    for (std::size_t i = 0; i < kFixedFields; ++i) {
        const std::size_t space = rest.find(' ');
        const std::string_view field = rest.substr(0, space);
        if (field.empty()) {
            return Err{ParseError::kMissingFields};
        }
        split.fields.at(i) = field;
        if (space == std::string_view::npos) {
            if (i + 1 < kFixedFields) {
                return Err{ParseError::kMissingFields};
            }
            rest = {};
        } else {
            rest.remove_prefix(space + 1);
        }
    }
    split.content = rest;
    return split;
}

// Validates the line and returns its pieces.
Result<SplitLine, ParseError> Validate(std::string_view line) {
    if (line.empty()) {
        return Err{ParseError::kEmptyLine};
    }
    if (!IsValidUtf8(line)) {
        return Err{ParseError::kInvalidUtf8};
    }
    return Split(line);
}

// Builds the runtime event. The label (field 0) is never read.
Result<BglEvent, ParseError> MakeEvent(const SplitLine& split) {
    const auto timestamp = ParseBglTime(split.fields.at(kTime));
    if (!timestamp) {
        return Err{ParseError::kBadTimestamp};
    }
    return BglEvent{
        .timestamp = *timestamp,
        .source_id = std::string{split.fields.at(kNode)},
        .component = std::string{split.fields.at(kComponent)},
        .level = std::string{split.fields.at(kLevel)},
        .message = std::string{split.content},
    };
}

}  // namespace

Result<BglEvent, ParseError> ParseBglLine(std::string_view line) {
    const auto split = Validate(line);
    if (!split) {
        return Err{split.Error()};
    }
    return MakeEvent(split.Value());
}

}  // namespace logguard::bgl
