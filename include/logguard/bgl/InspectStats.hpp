#pragma once

#include "logguard/bgl/BglEvent.hpp"
#include "logguard/bgl/ParseError.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_set>

namespace logguard::bgl {

// Collects the numbers printed by `logguard inspect`: line counts, errors by
// reason, distinct sources and the time range. The result does not depend on
// the order in which lines are added.
class InspectStats {
public:
    void AddEvent(const BglEvent& event);
    void AddError(ParseError error);

    [[nodiscard]] std::uint64_t TotalLines() const noexcept {
        return valid_lines_ + invalid_lines_;
    }
    [[nodiscard]] std::uint64_t ValidLines() const noexcept { return valid_lines_; }
    [[nodiscard]] std::uint64_t InvalidLines() const noexcept { return invalid_lines_; }
    [[nodiscard]] std::uint64_t ErrorCount(ParseError error) const noexcept;
    [[nodiscard]] std::size_t UniqueSources() const noexcept { return sources_.size(); }
    [[nodiscard]] std::optional<Timestamp> Earliest() const noexcept { return earliest_; }
    [[nodiscard]] std::optional<Timestamp> Latest() const noexcept { return latest_; }

private:
    std::uint64_t valid_lines_{};
    std::uint64_t invalid_lines_{};
    std::array<std::uint64_t, kAllParseErrors.size()> errors_{};
    std::unordered_set<std::string> sources_;
    std::optional<Timestamp> earliest_;
    std::optional<Timestamp> latest_;
};

}  // namespace logguard::bgl
