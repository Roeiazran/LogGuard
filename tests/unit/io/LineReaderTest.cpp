#include "logguard/io/LineReader.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>

namespace logguard::io {
namespace {

std::vector<Line> ReadAll(const std::string& text, std::size_t max_line_bytes = 100) {
    std::istringstream input{text};
    LineReader reader{input, max_line_bytes};
    std::vector<Line> lines;
    while (auto line = reader.Next()) {
        lines.push_back(*line);
    }
    return lines;
}

TEST(LineReaderTest, EmptyInputHasNoLines) {
    EXPECT_TRUE(ReadAll("").empty());
}

TEST(LineReaderTest, SplitsOnNewlines) {
    const auto lines = ReadAll("first\nsecond\n");
    ASSERT_EQ(lines.size(), 2U);
    EXPECT_EQ(lines[0].text, "first");
    EXPECT_EQ(lines[1].text, "second");
}

TEST(LineReaderTest, ReadsLastLineWithoutNewline) {
    const auto lines = ReadAll("first\nlast");
    ASSERT_EQ(lines.size(), 2U);
    EXPECT_EQ(lines[1].text, "last");
}

TEST(LineReaderTest, KeepsEmptyLines) {
    const auto lines = ReadAll("a\n\nb\n");
    ASSERT_EQ(lines.size(), 3U);
    EXPECT_EQ(lines[1].text, "");
}

TEST(LineReaderTest, StripsWindowsLineEndings) {
    const auto lines = ReadAll("a\r\nb\r\n");
    ASSERT_EQ(lines.size(), 2U);
    EXPECT_EQ(lines[0].text, "a");
    EXPECT_EQ(lines[1].text, "b");
}

TEST(LineReaderTest, FlagsTooLongLinesAndKeepsGoing) {
    const auto lines = ReadAll("12345\n1234567890\nok\n", 5);
    ASSERT_EQ(lines.size(), 3U);
    EXPECT_FALSE(lines[0].too_long);  // exactly at the limit is fine
    EXPECT_TRUE(lines[1].too_long);
    EXPECT_EQ(lines[1].text, "12345");  // only the first 5 bytes are stored
    EXPECT_FALSE(lines[2].too_long);
    EXPECT_EQ(lines[2].text, "ok");
}

}  // namespace
}  // namespace logguard::io
