#include "logguard/core/Version.hpp"

namespace logguard {

std::string_view Version() noexcept {
    return LOGGUARD_VERSION;
}

}  // namespace logguard
