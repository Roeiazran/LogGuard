#include "logguard/io/LineReader.hpp"

#include <streambuf>

namespace logguard::io {

LineReader::LineReader(std::istream& input, std::size_t max_line_bytes)
    : input_(&input), max_line_bytes_(max_line_bytes) {}

std::optional<Line> LineReader::Next() {
    using Traits = std::istream::traits_type;
    std::streambuf* buffer = input_->rdbuf();

    Line line;
    bool read_anything = false;
    for (auto ch = buffer->sbumpc(); !Traits::eq_int_type(ch, Traits::eof());
         ch = buffer->sbumpc()) {
        read_anything = true;
        const char c = Traits::to_char_type(ch);
        if (c == '\n') {
            break;
        }
        if (line.text.size() < max_line_bytes_) {
            line.text.push_back(c);
        } else {
            line.too_long = true;  // keep reading to find the end, but store nothing more
        }
    }

    if (!read_anything) {
        return std::nullopt;
    }
    if (!line.too_long && line.text.ends_with('\r')) {
        line.text.pop_back();  // Windows line ending
    }
    return line;
}

}  // namespace logguard::io
