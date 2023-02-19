#include <iostream>

#include "logging/logging.hpp"
#include "options.hpp"

namespace kore {
    const std::string _USAGE_MESSAGE
        = R"(kore - Compiler for the kore programming language

        kore [options] filename

    Options:

        Defaults in brackets after option description if relevant.

        -h, --help                  Show this help message.
        --version                   Show the current version information.

        -t, --timings               Show timings from scanning + parsing
                                    and typechecking.
        -d, --disassemble           Disassemble a compiled bytecode file.
        -x, --execute=<expr>        Parse and run an expression or statement.
        -b, --backend=<bytecode>    Select a code generation backend [bytecode].
        -v, --verbose               Increase the amount of program debugging
                                    output. Can be supplied multiple times to a
                                    level of three. Repetitions beyond three are
                                    ignored.
        --env-vars                  List all available environment variables.
        --colors, --colours         Control colored output.
        --c, --compile-only         Only compile to bytecode, do not run. Only
                                    applicable when --backend is 'bytecode'.
        --typecheck-only            Only perform type checking, then exit.
        -m, --mem-stats             Show useful memory statistics while running.

        Debugging options:

            --dump-scan       Only perform a scan of the input file for tokens and
                              dump the tokens to stderr.
            --dump-parse      Dump the parse tree elements to stderr.
            --dump-ast        Dump the parsed abstract syntax tree to stderr.
            --dump-codegen    Dump generated code to stderr.
    )";

    void validate_args(int argc, ParsedCommandLineArgs& args) {
        if (argc < 2) {
            args.error_message = "At least one argument is required";
        } else if (!args.path.has_filename()) {
            args.error_message = "Source file must be a file";
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
                } else if (arg == "-x" || arg == "--execute") {
                    parsed_args.execute = true;

                    if (i + 1 < argc) {
                        parsed_args.expr = args[i + 1];
                    } else {
                        error("Expected argument after -x, --execute");
                    }
                } else if (arg == "--dump-parse") {
                    parsed_args.dump_parse = true;
                } else if (arg == "--dump-ast") {
                    parsed_args.dump_ast = true;
                } else if (arg == "--dump-scan") {
                    parsed_args.dump_scan = true;
                } else if (arg == "--dump-codegen") {
                    parsed_args.dump_codegen = true;
                } else if (arg == "--version") {
                    parsed_args.version = true;
                } else if (arg == "--version-only") {
                    parsed_args.version_only = true;
                } else if (arg == "--colors" || arg == "--colours") {
                    parsed_args.colors = true;
                } else if (arg == "--typecheck-only") {
                    parsed_args.typecheck_only = true;
                } else if (arg == "-v" || arg == "--verbose") {
                    ++parsed_args.verbosity;
                } else {
                    parsed_args.error_message = "Unrecognised commandline option: '" + arg + "'";
                    return parsed_args;
                }
            } else {
                if (i != 0) {
                    if (parsed_args.path.empty()) {
                        parsed_args.path = arg;
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
}
