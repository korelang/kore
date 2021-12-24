#ifndef KORE_OPTIONS_HPP
#define KORE_OPTIONS_HPP

#include <string>

struct ParsedCommandLineArgs {
    // Why parsing failed, empty if successful
    std::string error_message;

    bool timings;
    bool execute;
    bool dump_scan;
    bool dump_parse;
    bool dump_codegen;
    bool disassemble;
    bool backend;
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
    std::string filename;
};

ParsedCommandLineArgs parse_commandline(int argc, char** args);

void print_help_message();

#endif // KORE_OPTIONS_HPP
