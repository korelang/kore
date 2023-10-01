#ifndef KORE_OPTIONS_HPP
#define KORE_OPTIONS_HPP

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace kore {
    struct ParsedCommandLineArgs {
        // Why parsing failed, empty if successful
        std::string error_message;

        // TODO: Change dump args to --dump (scan|parse|ast|kir)?
        bool timings;
        bool execute;
        bool dump_scan;
        bool dump_parse;
        bool dump_ast;
        std::string dump_kir;
        bool dump_codegen;
        bool disassemble;
        std::string target;
        bool version;
        bool version_only;
        bool help;
        int verbosity;
        bool env_vars;
        bool colors;
        bool compile_only;
        bool typecheck_only;
        bool mem_stats;

        std::string expr;
        std::vector<fs::path> paths;
    };

    ParsedCommandLineArgs parse_commandline(int argc, char** args);

    void print_help_message();
}

#endif // KORE_OPTIONS_HPP
