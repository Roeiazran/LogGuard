#include "logguard/bgl/BglTimestamp.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstddef>
#include <format>
#include <memory>
#include <system_error>

namespace logguard::bgl {
namespace {

// Reads `text.substr(pos, count)` as a non-negative integer. False if it isn't all digits.
bool ReadNumber(std::string_view text, std::size_t pos, std::size_t count, int& out) {
    const std::string_view digits = text.substr(pos, count);
    const bool all_digits = std::ranges::all_of(
        digits, [](char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; });
    if (!all_digits) {
        return false;
    }
    const auto [end, error] =
        std::from_chars(std::to_address(digits.begin()), std::to_address(digits.end()), out);
    return error == std::errc{} && end == std::to_address(digits.end());
}

}  // namespace

std::optional<Timestamp> ParseBglTime(std::string_view text) {
    // Positions:  0123456789012345678901234 5
    //             2005-06-03-15.42.50.363779
    constexpr std::size_t kLength = 26;
    if (text.size() != kLength || text[4] != '-' || text[7] != '-' || text[10] != '-' ||
        text[13] != '.' || text[16] != '.' || text[19] != '.') {
        return std::nullopt;
    }

    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
    int micros = 0;
    if (!ReadNumber(text, 0, 4, year) || !ReadNumber(text, 5, 2, month) ||
        !ReadNumber(text, 8, 2, day) || !ReadNumber(text, 11, 2, hour) ||
        !ReadNumber(text, 14, 2, minute) || !ReadNumber(text, 17, 2, second) ||
        !ReadNumber(text, 20, 6, micros)) {
        return std::nullopt;
    }

    using namespace std::chrono;
    const year_month_day date{std::chrono::year{year},
                              std::chrono::month{static_cast<unsigned>(month)},
                              std::chrono::day{static_cast<unsigned>(day)}};
    if (!date.ok() || hour > 23 || minute > 59 || second > 59) {
        return std::nullopt;  // e.g. 2005-06-31 or 25:00
    }

    return sys_days{date} + hours{hour} + minutes{minute} + seconds{second} + microseconds{micros};
}

std::string FormatTimestamp(Timestamp timestamp) {
    return std::format("{:%Y-%m-%dT%H:%M:%S}", timestamp);
}

}  // namespace logguard::bgl
