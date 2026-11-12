#include "logguard/core/Version.hpp"

#include <gtest/gtest.h>

#include <string_view>

namespace logguard {
namespace {

TEST(VersionTest, IsNotEmpty) {
    EXPECT_FALSE(Version().empty());
}

TEST(VersionTest, MatchesProjectVersion) {
    EXPECT_EQ(Version(), std::string_view{LOGGUARD_EXPECTED_VERSION});
}

}  // namespace
}  // namespace logguard
