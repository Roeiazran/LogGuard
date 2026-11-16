#include "ExitCodes.hpp"
#include "InspectCommand.hpp"
#include "logguard/core/Version.hpp"

#include <CLI/CLI.hpp>

#include <exception>
#include <iostream>

// main's signature is fixed by the C++ standard.
int main(int argc, char** argv) {  // NOLINT(misc-const-correctness)
    using namespace logguard::cli;
    try {
        CLI::App app{"LogGuard: log anomaly detection engine", "logguard"};

        bool show_version = false;
        app.add_flag("--version", show_version, "Print the version and exit");

        InspectOptions inspect_options;
        CLI::App* inspect = app.add_subcommand("inspect", "Print statistics about a log file");
        inspect->add_option("--input", inspect_options.input, "Log file to read")->required();
        inspect->add_option("--format", inspect_options.format, "Input format")
            ->required()
            ->check(CLI::IsMember({"bgl"}));

        try {
            app.parse(argc, argv);
        } catch (const CLI::ParseError& error) {
            // Prints --help, or the usage error. Every real usage error is exit code 2.
            return app.exit(error) == 0 ? kExitSuccess : kExitUsageError;
        }

        if (show_version) {
            std::cout << "logguard " << logguard::Version() << '\n';
            return kExitSuccess;
        }
        if (inspect->parsed()) {
            return RunInspect(inspect_options, std::cout, std::cerr);
        }

        std::cerr << app.help();  // no command given
        return kExitUsageError;
    } catch (const std::exception& error) {
        // Exceptions mean environment or programming failures; caught once, here (ADR-0007).
        std::cerr << "error: " << error.what() << '\n';
        return kExitRuntimeError;
    }
}
