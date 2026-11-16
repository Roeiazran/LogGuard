#include "logguard/bgl/BglTimestamp.hpp"
#include "logguard/bgl/InspectStats.hpp"

#include <gtest/gtest.h>

#include <string>

namespace logguard::bgl {
namespace {

BglEvent MakeEvent(const std::string& source, const char* time) {
    BglEvent event;
    event.timestamp = *ParseBglTime(time);
    event.source_id = source;
    return event;
}

TEST(InspectStatsTest, StartsEmpty) {
    const InspectStats stats;
    EXPECT_EQ(stats.TotalLines(), 0U);
    EXPECT_EQ(stats.UniqueSources(), 0U);
    EXPECT_FALSE(stats.Earliest().has_value());
    EXPECT_FALSE(stats.Latest().has_value());
}

TEST(InspectStatsTest, CountsValidAndInvalidLines) {
    InspectStats stats;
    stats.AddEvent(MakeEvent("A", "2005-06-03-15.42.50.000000"));
    stats.AddError(ParseError::kMissingFields);
    stats.AddError(ParseError::kMissingFields);
    stats.AddError(ParseError::kBadTimestamp);

    EXPECT_EQ(stats.TotalLines(), 4U);
    EXPECT_EQ(stats.ValidLines(), 1U);
    EXPECT_EQ(stats.InvalidLines(), 3U);
    EXPECT_EQ(stats.ErrorCount(ParseError::kMissingFields), 2U);
    EXPECT_EQ(stats.ErrorCount(ParseError::kBadTimestamp), 1U);
    EXPECT_EQ(stats.ErrorCount(ParseError::kEmptyLine), 0U);
}

TEST(InspectStatsTest, CountsEachSourceOnce) {
    InspectStats stats;
    stats.AddEvent(MakeEvent("A", "2005-06-03-15.42.50.000000"));
    stats.AddEvent(MakeEvent("B", "2005-06-03-15.42.51.000000"));
    stats.AddEvent(MakeEvent("A", "2005-06-03-15.42.52.000000"));
    EXPECT_EQ(stats.UniqueSources(), 2U);
}

TEST(InspectStatsTest, TimeRangeDoesNotDependOnLineOrder) {
    InspectStats stats;
    stats.AddEvent(MakeEvent("A", "2005-06-05-00.00.00.000000"));
    stats.AddEvent(MakeEvent("A", "2005-06-03-00.00.00.000000"));  // earliest, but not first
    stats.AddEvent(MakeEvent("A", "2005-06-04-00.00.00.000000"));

    ASSERT_TRUE(stats.Earliest() && stats.Latest());
    EXPECT_EQ(FormatTimestamp(*stats.Earliest()), "2005-06-03T00:00:00.000000");
    EXPECT_EQ(FormatTimestamp(*stats.Latest()), "2005-06-05T00:00:00.000000");
}

}  // namespace
}  // namespace logguard::bgl
