#include <iostream>

#include "logging/logging.hpp"
#include "options.hpp"

namespace koredis {
    const std::string _USAGE_MESSAGE
        = R"(koredis - Disassembler for kore programming language bytecode

        koredis [options] <paths>...

    Options:

        Defaults in brackets after option description if relevant.

        -h, --help                  Show this help message.
        --version                   Show the current version information.
        --version-only              Show the current version number only.
        -p, --porcelain             Output only disassembled bytecode. Takes
                                    precedence over verbosity.
        -v, --verbose               Increase the amount of program debugging
                                    output. Can be supplied multiple times to a
                                    level of three. Repetitions beyond three are
                                    ignored.
        -c, --colors, --colours     Enable colored output.
    )";

    void validate_args(int argc, ParsedCommandLineArgs& args) {
        if (argc < 2) {
            args.error_message = "At least one argument is required";
        } else if (args.paths.size() < 1) {
            if (!args.version && !args.version_only && !args.help) {
                args.error_message = "At least one path is required";
            }
        }

        return;
    }

    ParsedCommandLineArgs parse_commandline(int argc, char** args) {
        auto parsed_args = ParsedCommandLineArgs{};

        if (!args) {
            return parsed_args;
        }

        bool seen_double_dash = false;

        for (int i = 0; i < argc; ++i) {
            std::string arg = args[i];

            if (!seen_double_dash && arg[0] == '-') {
                if (arg == "-h" || arg == "--help") {
                    parsed_args.help = true;

                    return parsed_args;
                } else if (arg == "--version") {
                    parsed_args.version = true;
                } else if (arg == "--version-only") {
                    parsed_args.version_only = true;
                } else if (arg == "-c" || arg == "--colors" || arg == "--colours") {
                    parsed_args.colors = true;
                } else if (arg == "-v" || arg == "--verbose") {
                    ++parsed_args.verbosity;
                } else if (arg == "-p" || arg == "--porcelain") {
                    parsed_args.porcelain = true;
                } else if (arg == "--") {
                    seen_double_dash = true;
                } else {
                    parsed_args.error_message = "Unrecognised option '" + arg + "'";
                    return parsed_args;
                }
            } else {
                if (i != 0) {
                    parsed_args.paths.push_back(arg);
                }
            }
        }

        validate_args(argc, parsed_args);

        return parsed_args;
    }

    void print_help_message() {
        std::cout << _USAGE_MESSAGE << std::endl;
    }
}
