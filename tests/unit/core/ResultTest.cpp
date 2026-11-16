#include "logguard/core/Result.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <string>
#include <variant>

namespace logguard {
namespace {

enum class TestError : std::uint8_t { kBad };

Result<int, TestError> Half(int value) {
    if (value % 2 != 0) {
        return Err{TestError::kBad};
    }
    return value / 2;
}

TEST(ResultTest, HoldsValueOnSuccess) {
    const auto result = Half(10);
    ASSERT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value(), 5);
}

TEST(ResultTest, HoldsErrorOnFailure) {
    const auto result = Half(3);
    ASSERT_FALSE(result);
    EXPECT_EQ(result.Error(), TestError::kBad);
}

TEST(ResultTest, WorksWhenValueAndErrorHaveTheSameType) {
    const Result<std::string, std::string> ok{std::string{"value"}};
    const Result<std::string, std::string> failed{Err{std::string{"error"}}};
    EXPECT_EQ(ok.Value(), "value");
    EXPECT_EQ(failed.Error(), "error");
}

TEST(ResultTest, AccessingTheWrongSideThrows) {
    const auto result = Half(3);
    EXPECT_THROW((void)result.Value(), std::bad_variant_access);
}

}  // namespace
}  // namespace logguard
