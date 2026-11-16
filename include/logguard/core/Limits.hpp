#pragma once

#include <cstddef>

namespace logguard {

// Lines longer than this are rejected instead of being loaded into memory.
inline constexpr std::size_t kDefaultMaxLineBytes = 8192;

}  // namespace logguard
