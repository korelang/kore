#include <iostream>

#include "logging/logging.hpp"
#include "options.hpp"

namespace kore {
    const std::string _USAGE_MESSAGE
        = R"(kore - Virtual machine for the kore programming language

        kore [options] [path]

    Usage:

        Execute a compiled kore source file. Start a REPL if no file is given.

    Options:

        Defaults in brackets after option description if relevant.

        -h, --help        Show this help message.
        --version         Show the current version information.
        --version-only    Show the current version number only.
        --dump-registers  Dump all registers of the last call frame once
                          the vm is done executing
    )";

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
                } else if (arg == "--dump-registers") {
                    parsed_args.dump_registers = true;
                } else if (arg == "--") {
                    seen_double_dash = true;
                } else {
                    parsed_args.error_message = "Unrecognised option '" + arg + "'";
                    return parsed_args;
                }
            } else {
                if (i != 0) {
                    if (parsed_args.path.empty()) {
                        parsed_args.path = fs::path(arg);
                    } else {
                        parsed_args.error_message = "At most one path can be given";
                    }
                }
            }
        }

        if (parsed_args.path.empty()) {
            parsed_args.repl_mode = true;
        }

        return parsed_args;
    }

    void print_help_message() {
        std::cout << _USAGE_MESSAGE << std::endl;
    }
}
