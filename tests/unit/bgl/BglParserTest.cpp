#include "logguard/bgl/BglParser.hpp"
#include "logguard/bgl/BglTimestamp.hpp"

#include <gtest/gtest.h>

#include <string>

namespace logguard::bgl {
namespace {

constexpr const char* kValidLine =
    "- 1117838570 2005.06.03 R02-M1-N0-C:J12-U11 2005-06-03-15.42.50.363779 "
    "R02-M1-N0-C:J12-U11 RAS KERNEL INFO instruction cache parity error corrected";

TEST(BglParserTest, ParsesAllFieldsOfAValidLine) {
    const auto result = ParseBglLine(kValidLine);
    ASSERT_TRUE(result.HasValue());

    const BglEvent& event = result.Value();
    EXPECT_EQ(FormatTimestamp(event.timestamp), "2005-06-03T15:42:50.363779");
    EXPECT_EQ(event.source_id, "R02-M1-N0-C:J12-U11");
    EXPECT_EQ(event.component, "KERNEL");
    EXPECT_EQ(event.level, "INFO");
    EXPECT_EQ(event.message, "instruction cache parity error corrected");
}

TEST(BglParserTest, KeepsSpacesInsideTheMessage) {
    const auto result =
        ParseBglLine("- 1 2005.06.03 NODE 2005-06-03-15.42.50.363779 NODE RAS APP ERROR a  b   c ");
    ASSERT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value().message, "a  b   c ");
}

TEST(BglParserTest, AllowsAnEmptyMessage) {
    const auto result =
        ParseBglLine("- 1 2005.06.03 NODE 2005-06-03-15.42.50.363779 NODE RAS APP ERROR");
    ASSERT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value().message, "");
}

TEST(BglParserTest, RejectsEmptyLine) {
    EXPECT_EQ(ParseBglLine("").Error(), ParseError::kEmptyLine);
}

TEST(BglParserTest, RejectsMissingFields) {
    EXPECT_EQ(ParseBglLine("- 1117838570 2005.06.03 R02-M1-N0-C:J12-U11").Error(),
              ParseError::kMissingFields);
    EXPECT_EQ(ParseBglLine("   ").Error(), ParseError::kMissingFields);
}

TEST(BglParserTest, RejectsDoubleSpaceBetweenFixedFields) {
    EXPECT_EQ(ParseBglLine("-  1 2005.06.03 NODE 2005-06-03-15.42.50.363779 NODE RAS APP ERROR x")
                  .Error(),
              ParseError::kMissingFields);
}

TEST(BglParserTest, RejectsBadTimestamp) {
    EXPECT_EQ(
        ParseBglLine("- 1 2005.06.03 NODE 2005-13-03-15.42.50.363779 NODE RAS APP ERROR x").Error(),
        ParseError::kBadTimestamp);
}

TEST(BglParserTest, RejectsInvalidUtf8) {
    const std::string line =
        "- 1 2005.06.03 NODE 2005-06-03-15.42.50.363779 NODE RAS APP ERROR bad \xFF byte";
    EXPECT_EQ(ParseBglLine(line).Error(), ParseError::kInvalidUtf8);
}

TEST(BglParserTest, ErrorNamesAreStable) {
    EXPECT_EQ(ToString(ParseError::kEmptyLine), "empty_line");
    EXPECT_EQ(ToString(ParseError::kTooLong), "too_long");
    EXPECT_EQ(ToString(ParseError::kMissingFields), "missing_fields");
    EXPECT_EQ(ToString(ParseError::kBadTimestamp), "bad_timestamp");
    EXPECT_EQ(ToString(ParseError::kInvalidUtf8), "invalid_utf8");
}

}  // namespace
}  // namespace logguard::bgl
