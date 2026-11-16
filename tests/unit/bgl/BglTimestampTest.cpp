#include "logguard/bgl/BglTimestamp.hpp"

#include <gtest/gtest.h>

#include <chrono>

namespace logguard::bgl {
namespace {

TEST(BglTimestampTest, ParsesValidTime) {
    const auto time = ParseBglTime("2005-06-03-15.42.50.363779");
    ASSERT_TRUE(time.has_value());
    EXPECT_EQ(FormatTimestamp(*time), "2005-06-03T15:42:50.363779");
}

TEST(BglTimestampTest, ParsesBoundaryValues) {
    EXPECT_TRUE(ParseBglTime("2005-12-31-23.59.59.999999").has_value());
    EXPECT_TRUE(ParseBglTime("2004-02-29-00.00.00.000000").has_value());  // leap year
}

TEST(BglTimestampTest, OrdersByTime) {
    const auto earlier = ParseBglTime("2005-06-03-15.42.50.363779");
    const auto later = ParseBglTime("2005-06-03-15.42.50.363780");
    ASSERT_TRUE(earlier && later);
    EXPECT_EQ(*later - *earlier, std::chrono::microseconds{1});
}

TEST(BglTimestampTest, RejectsImpossibleDates) {
    EXPECT_FALSE(ParseBglTime("2005-13-03-15.42.50.363779"));  // month 13
    EXPECT_FALSE(ParseBglTime("2005-06-31-15.42.50.363779"));  // June has 30 days
    EXPECT_FALSE(ParseBglTime("2005-02-29-15.42.50.363779"));  // not a leap year
    EXPECT_FALSE(ParseBglTime("2005-06-03-24.00.00.000000"));  // hour 24
}

TEST(BglTimestampTest, RejectsWrongShape) {
    EXPECT_FALSE(ParseBglTime(""));
    EXPECT_FALSE(ParseBglTime("2005-06-03 15:42:50"));
    EXPECT_FALSE(ParseBglTime("2005-06-03-15.42.50.36377"));   // 5 digit fraction
    EXPECT_FALSE(ParseBglTime("2005/06/03-15.42.50.363779"));  // wrong separator
    EXPECT_FALSE(ParseBglTime("2005-06-03-15.42.5x.363779"));  // not a digit
    EXPECT_FALSE(ParseBglTime("2005-06-03-15.42.+5.363779"));  // sign is not a digit
}

}  // namespace
}  // namespace logguard::bgl
