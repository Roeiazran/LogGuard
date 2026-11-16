#include "logguard/core/Utf8.hpp"

#include <cstddef>
#include <cstdint>

namespace logguard {
namespace {

struct SequenceInfo {
    std::size_t length{};     // total bytes in the sequence, 0 = invalid lead byte
    std::uint32_t bits{};     // code point bits carried by the lead byte
    std::uint32_t minimum{};  // smallest code point allowed for this length
};

SequenceInfo DecodeLeadByte(unsigned char lead) noexcept {
    if ((lead & 0xE0U) == 0xC0U) {
        return {.length = 2, .bits = lead & 0x1FU, .minimum = 0x80U};
    }
    if ((lead & 0xF0U) == 0xE0U) {
        return {.length = 3, .bits = lead & 0x0FU, .minimum = 0x800U};
    }
    if ((lead & 0xF8U) == 0xF0U) {
        return {.length = 4, .bits = lead & 0x07U, .minimum = 0x10000U};
    }
    return {};
}

}  // namespace

bool IsValidUtf8(std::string_view text) noexcept {
    std::size_t pos = 0;
    while (pos < text.size()) {
        const auto lead = static_cast<unsigned char>(text[pos]);
        if (lead < 0x80U) {  // plain ASCII
            ++pos;
            continue;
        }

        const SequenceInfo info = DecodeLeadByte(lead);
        if (info.length == 0 || text.size() - pos < info.length) {
            return false;
        }

        std::uint32_t code_point = info.bits;
        for (std::size_t i = 1; i < info.length; ++i) {
            const auto byte = static_cast<unsigned char>(text[pos + i]);
            if ((byte & 0xC0U) != 0x80U) {  // continuation bytes look like 10xxxxxx
                return false;
            }
            code_point = (code_point << 6U) | (byte & 0x3FU);
        }

        const bool overlong = code_point < info.minimum;
        const bool surrogate = code_point >= 0xD800U && code_point <= 0xDFFFU;
        if (overlong || surrogate || code_point > 0x10FFFFU) {
            return false;
        }
        pos += info.length;
    }
    return true;
}

}  // namespace logguard
