#include "InspectCommand.hpp"

#include "ExitCodes.hpp"
#include "logguard/bgl/BglParser.hpp"
#include "logguard/bgl/BglTimestamp.hpp"
#include "logguard/bgl/InspectStats.hpp"
#include "logguard/core/Limits.hpp"
#include "logguard/io/LineReader.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <ostream>
#include <string>

namespace logguard::cli {
namespace {

bgl::InspectStats CollectStats(std::istream& input) {
    bgl::InspectStats stats;
    io::LineReader reader{input, kDefaultMaxLineBytes};
    while (auto line = reader.Next()) {
        if (line->too_long) {
            stats.AddError(bgl::ParseError::kTooLong);
            continue;
        }
        const auto event = bgl::ParseBglLine(line->text);
        if (event) {
            stats.AddEvent(event.Value());
        } else {
            stats.AddError(event.Error());
        }
    }
    return stats;
}

// ordered_json keeps keys in insertion order, so the output is byte-identical on every run.
nlohmann::ordered_json ToJson(const bgl::InspectStats& stats) {
    nlohmann::ordered_json json;
    json["format"] = "bgl";
    json["lines"]["total"] = stats.TotalLines();
    json["lines"]["valid"] = stats.ValidLines();
    json["lines"]["invalid"] = stats.InvalidLines();

    json["invalid_by_reason"] = nlohmann::ordered_json::object();
    for (const bgl::ParseError error : bgl::kAllParseErrors) {
        json["invalid_by_reason"][std::string{bgl::ToString(error)}] = stats.ErrorCount(error);
    }

    json["unique_sources"] = stats.UniqueSources();

    json["time_range"] = nullptr;  // stays null when there are no valid lines
    const auto earliest = stats.Earliest();
    const auto latest = stats.Latest();
    if (earliest && latest) {
        json["time_range"]["earliest"] = bgl::FormatTimestamp(*earliest);
        json["time_range"]["latest"] = bgl::FormatTimestamp(*latest);
    }
    return json;
}

}  // namespace

int RunInspect(const InspectOptions& options, std::ostream& out, std::ostream& err) {
    std::ifstream input{options.input, std::ios::binary};
    if (!input) {
        err << "error: cannot open input file: " << options.input << '\n';
        return kExitRuntimeError;
    }

    const bgl::InspectStats stats = CollectStats(input);
    out << ToJson(stats).dump(2) << '\n';
    return kExitSuccess;
}

}  // namespace logguard::cli
