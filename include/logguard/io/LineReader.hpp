#pragma once

#include <cstddef>
#include <istream>
#include <optional>
#include <string>

namespace logguard::io {

struct Line {
    std::string text;      // without the trailing "\n" or "\r\n"
    bool too_long{false};  // true if the line exceeded the limit; `text` holds only its start
};

// Reads a stream one line at a time. Memory use is bounded by `max_line_bytes`,
// even if the input contains a huge line (or no newline at all).
class LineReader {
public:
    LineReader(std::istream& input, std::size_t max_line_bytes);

    // The next line, or std::nullopt at end of input.
    [[nodiscard]] std::optional<Line> Next();

private:
    std::istream* input_;
    std::size_t max_line_bytes_;
};

}  // namespace logguard::io
