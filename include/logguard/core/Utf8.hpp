#pragma once

#include <string_view>

namespace logguard {

// True if `text` is well-formed UTF-8 (no overlong forms, no surrogates, max U+10FFFF).
[[nodiscard]] bool IsValidUtf8(std::string_view text) noexcept;

}  // namespace logguard
