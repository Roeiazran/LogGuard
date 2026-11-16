#include "logguard/bgl/BglParser.hpp"
#include "logguard/bgl/LabeledBglParser.hpp"

#include <gtest/gtest.h>

namespace logguard::bgl {
namespace {

// Compile-time proof of label isolation (ADR-0006): if anyone adds a `label`
// member to BglEvent, this file stops compiling.
template <typename T>
concept HasLabelMember = requires(T value) { value.label; };
static_assert(!HasLabelMember<BglEvent>, "BglEvent must not carry the ground-truth label");

constexpr const char* kNormalLine =
    "- 1117838570 2005.06.03 R02-M1-N0-C:J12-U11 2005-06-03-15.42.50.363779 "
    "R02-M1-N0-C:J12-U11 RAS KERNEL INFO instruction cache parity error corrected";
constexpr const char* kAlertLine =
    "KERNDTLB 1118536327 2005.06.11 R30-M0-N9-C:J16-U01 2005-06-11-17.32.07.581048 "
    "R30-M0-N9-C:J16-U01 RAS KERNEL FATAL data TLB error interrupt";

TEST(LabeledBglParserTest, DashMeansNormal) {
    const auto result = ParseLabeledBglLine(kNormalLine);
    ASSERT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value().label.category, "-");
    EXPECT_FALSE(result.Value().label.IsAlert());
}

TEST(LabeledBglParserTest, AnythingElseIsAnAlertCategory) {
    const auto result = ParseLabeledBglLine(kAlertLine);
    ASSERT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value().label.category, "KERNDTLB");
    EXPECT_TRUE(result.Value().label.IsAlert());
}

TEST(LabeledBglParserTest, EventMatchesTheRuntimeParser) {
    const auto labeled = ParseLabeledBglLine(kAlertLine);
    const auto runtime = ParseBglLine(kAlertLine);
    ASSERT_TRUE(labeled.HasValue());
    ASSERT_TRUE(runtime.HasValue());
    EXPECT_EQ(labeled.Value().event, runtime.Value());
}

TEST(LabeledBglParserTest, ReportsTheSameErrorsAsTheRuntimeParser) {
    EXPECT_EQ(ParseLabeledBglLine("").Error(), ParseError::kEmptyLine);
    EXPECT_EQ(ParseLabeledBglLine("KERNDTLB 1 2005.06.11").Error(), ParseError::kMissingFields);
}

}  // namespace
}  // namespace logguard::bgl
