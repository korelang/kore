#include <iostream>

#include "logging/logging.hpp"
#include "options.hpp"

const std::string _USAGE_MESSAGE
    = R"(kore - Compiler for the kore programming language

    kore [options] filename

Options:

    Defaults in brackets after option description if relevant.

    -h, --help                  Show this help message.
    --version                   Show the current version information.

    -t, --timings               Show timings from scanning + parsing
                                and typechecking.
    --dump-parse                Dump the parse tree.
    --dump-codegen              Dump generated code.
    -d, --disassemble           Disassemble a compiled bytecode file.
    -b, --backend=<bytecode>    Select a code generation backend [bytecode].
    -v, --verbose               Increase the amount of program debugging
                                output. Can be supplied multiple times to a
                                level of three. Repetitions beyond three are
                                ignored.
    --env-vars                  List all available environment variables.
    --colors, --colours         Control colored output.
    --c, --compile-only         Only compile to bytecode, do not run. Only
                                applicable when --backend is 'bytecode'.
    -m, --mem-stats             Show useful memory statistics while running.
)";

void validate_args(int argc, ParsedCommandLineArgs& args) {
    if (argc < 2) {
        args.error_message = "At least one argument is required";
    }

    return;
}

ParsedCommandLineArgs parse_commandline(int argc, char** args) {
    auto parsed_args = ParsedCommandLineArgs{};

    if (!args) {
        return parsed_args;
    }

    for (int i = 0; i < argc; ++i) {
        std::string arg = args[i];

        if (arg[0] == '-') {
            if (arg == "-h" || arg == "--help") {
                parsed_args.help = true;
                parsed_args.error_message = "";

                return parsed_args;
            } else if (arg == "-t" || arg == "--timings") {
                parsed_args.timings = true;
            } else if (arg == "--dump-parse") {
                parsed_args.dump_parse = true;
            } else if (arg == "--version") {
                parsed_args.version = true;
            } else if (arg == "--version-only") {
                parsed_args.version_only = true;
            } else if (arg == "--colors" || arg == "--colours") {
                parsed_args.colors = true;
            } else if (arg == "-v" || arg == "--verbose") {
                ++parsed_args.verbosity;
            } else {
                parsed_args.error_message = "Unrecognised commandline option";
                return parsed_args;
            }
        } else {
            if (i != 0) {
                if (parsed_args.filename.size() == 0) {
                    parsed_args.filename = arg;
                } else {
                    parsed_args.error_message = "Only one filename can be supplied";
                }
            }
        }
    }

    validate_args(argc, parsed_args);

    return parsed_args;
}

void print_help_message() {
    std::cout << _USAGE_MESSAGE << std::endl;
}
