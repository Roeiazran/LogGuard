#include "logguard/bgl/InspectStats.hpp"

namespace logguard::bgl {

void InspectStats::AddEvent(const BglEvent& event) {
    ++valid_lines_;
    sources_.insert(event.source_id);
    if (!earliest_ || event.timestamp < *earliest_) {
        earliest_ = event.timestamp;
    }
    if (!latest_ || event.timestamp > *latest_) {
        latest_ = event.timestamp;
    }
}

void InspectStats::AddError(ParseError error) {
    ++invalid_lines_;
    ++errors_.at(static_cast<std::size_t>(error));
}

std::uint64_t InspectStats::ErrorCount(ParseError error) const noexcept {
    return errors_[static_cast<std::size_t>(error)];
}

}  // namespace logguard::bgl
