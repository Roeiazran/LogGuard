#include "logguard/core/Utf8.hpp"

#include <gtest/gtest.h>

namespace logguard {
namespace {

TEST(Utf8Test, AcceptsAsciiAndEmpty) {
    EXPECT_TRUE(IsValidUtf8(""));
    EXPECT_TRUE(IsValidUtf8("instruction cache parity error corrected"));
}

TEST(Utf8Test, AcceptsMultiByteCharacters) {
    EXPECT_TRUE(IsValidUtf8("caf\xC3\xA9"));       // é (2 bytes)
    EXPECT_TRUE(IsValidUtf8("\xE2\x82\xAC"));      // € (3 bytes)
    EXPECT_TRUE(IsValidUtf8("\xF0\x9F\x98\x80"));  // emoji (4 bytes)
}

TEST(Utf8Test, RejectsMalformedSequences) {
    EXPECT_FALSE(IsValidUtf8("\xFF"));              // never valid in UTF-8
    EXPECT_FALSE(IsValidUtf8("\x80"));              // continuation byte without a lead
    EXPECT_FALSE(IsValidUtf8("\xC3"));              // truncated sequence
    EXPECT_FALSE(IsValidUtf8("\xC0\xAF"));          // overlong encoding of '/'
    EXPECT_FALSE(IsValidUtf8("\xED\xA0\x80"));      // UTF-16 surrogate
    EXPECT_FALSE(IsValidUtf8("\xF4\x90\x80\x80"));  // above U+10FFFF
}

}  // namespace
}  // namespace logguard
