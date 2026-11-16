#include "logguard/bgl/ParseError.hpp"

namespace logguard::bgl {

std::string_view ToString(ParseError error) noexcept {
    switch (error) {
        case ParseError::kEmptyLine:
            return "empty_line";
        case ParseError::kTooLong:
            return "too_long";
        case ParseError::kMissingFields:
            return "missing_fields";
        case ParseError::kBadTimestamp:
            return "bad_timestamp";
        case ParseError::kInvalidUtf8:
            return "invalid_utf8";
    }
    return "unknown";
}

}  // namespace logguard::bgl
