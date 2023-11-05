#include <iostream>
#include <optional>
#include <map>

#include "logging/logging.hpp"
#include "bin/korec/options.hpp"

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
        -t, --target=<bytecode>     Select a code generation target [bytecode].
        -v, --verbose               Increase the amount of program debugging
                                    output. Can be supplied multiple times to a
                                    level of three. Repetitions beyond three are
                                    ignored.
        --env-vars                  List all available environment variables.
        --colors, --colours         Control colored output.
        --c, --compile-only         Only compile to bytecode, do not run. Only
                                    applicable when --target is 'bytecode'.
        --typecheck-only            Only perform type checking, then exit.
        -m, --mem-stats             Show useful memory statistics while running.

        Debugging options:

            --trace=<pass>    Output trace information for a compiler pass. Accepted
                              values are:

                                * scan REMOVE THIS?
                                * parse
                                * type_inference
                                * type_check
                                * codegen

            --dump=<pass>     Stop at and dump information from various steps of
                              the compilation process to stderr. Accepted values are:

                                * scan     Only perform scanning and dump scanned tokens
                                * parse    Only parse and dump parsed ast elements
                                * ast      Only parse and dump parsed ast
                                * kir      Dump intermediate representation
                                * codegen  Dump generated code for target
    )";

    std::map<std::string, TraceOption> arg_to_trace_option{
        { "scan",           TraceOption::Scan },
        { "parse",          TraceOption::Parse },
        { "type_inference", TraceOption::TypeInference },
        { "type_check",     TraceOption::TypeCheck },
        { "kir",            TraceOption::Kir },
        { "codegen",        TraceOption::Codegen },
    };

    std::map<std::string, DumpOption> arg_to_dump_option{
        { "scan",    DumpOption::Scan },
        { "parse",   DumpOption::Parse },
        { "kir",     DumpOption::Kir },
        { "codegen", DumpOption::Codegen },
    };

    void validate_args(int argc, ParsedCommandLineArgs& args) {
        if (argc < 2) {
            args.error_message = "At least one argument is required";
        } else if (args.target != "bytecode") {
            if (args.target == "") {
                args.target = "bytecode";
            } else {
                args.error_message = "Only 'bytecode' is currently supported as a target";
            }
        }

        for (const auto& path : args.paths) {
            if (!path.has_filename()) {
                args.error_message = "Source file must be a file: '" + path.string() + "'";
            }
        }

        return;
    }

    std::optional<std::string> get_argument(
        std::size_t pos,
        const std::vector<std::string>& args
    ) {
        if (pos + 1 < args.size()) {
            return args[pos + 1];
        } else {
            error("Expected argument after %s", args[pos].c_str());
            return std::nullopt;
        }
    }

    ParsedCommandLineArgs parse_commandline(int argc, char** argv) {
        auto parsed_args = ParsedCommandLineArgs{};

        if (!argv) {
            return parsed_args;
        }

        std::vector<std::string> args(argv + 1, argv + argc);

        for (std::size_t i = 0; i < args.size();) {
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
                    auto option_arg = get_argument(i, args);
                    parsed_args.expr = option_arg.value_or("");

                    if (!option_arg) {
                        break;
                    }
                    ++i;
                } else if (arg == "--trace") {
                    auto option_arg = get_argument(i, args);
                    auto trace_option = option_arg.value_or("");

                    if (!option_arg) {
                        break;
                    }

                    if (arg_to_trace_option.find(trace_option) == arg_to_trace_option.end()) {
                        parsed_args.error_message = "Unknown trace option '" + trace_option + "'";
                        break;
                    }

                    parsed_args.trace = arg_to_trace_option[trace_option];
                    ++i;
                } else if (arg == "--dump") {
                    auto option_arg = get_argument(i, args);
                    auto dump_option = option_arg.value_or("");

                    if (!option_arg) {
                        break;
                    }

                    if (arg_to_trace_option.find(dump_option) == arg_to_trace_option.end()) {
                        parsed_args.error_message = "Unknown dump option '" + dump_option + "'";
                        break;
                    }

                    parsed_args.dump = arg_to_dump_option[dump_option];
                    ++i;
                } else if (arg == "--version") {
                    parsed_args.version = true;
                    return parsed_args;
                } else if (arg == "--version-only") {
                    parsed_args.version_only = true;
                    return parsed_args;
                } else if (arg == "--colors" || arg == "--colours") {
                    parsed_args.colors = true;
                } else if (arg == "--typecheck-only") {
                    parsed_args.typecheck_only = true;
                } else if (arg == "--target") {
                    if (i + 1 < args.size()) {
                        parsed_args.target = args[i + 1];
                        ++i;
                    } else {
                        error("Expected argument after -t, --target");
                    }
                } else if (arg == "-v" || arg == "--verbose") {
                    ++parsed_args.verbosity;
                } else {
                    parsed_args.error_message = "Unrecognised commandline option: '" + arg + "'";
                    return parsed_args;
                }
            } else {
                if (i != 0) {
                    parsed_args.paths.push_back(arg);
                }
            }

            ++i;
        }

        validate_args(argc, parsed_args);

        return parsed_args;
    }

    void print_help_message() {
        std::cout << _USAGE_MESSAGE << std::endl;
    }
}
