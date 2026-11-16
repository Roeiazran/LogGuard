#pragma once

#include <iosfwd>
#include <string>

namespace logguard::cli {

struct InspectOptions {
    std::string input;   // path of the log file
    std::string format;  // only "bgl" for now
};

// Reads the whole input, prints a JSON summary to `out` and returns the exit code.
[[nodiscard]] int RunInspect(const InspectOptions& options, std::ostream& out, std::ostream& err);

}  // namespace logguard::cli
