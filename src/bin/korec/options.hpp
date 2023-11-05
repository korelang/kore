#ifndef KORE_OPTIONS_HPP
#define KORE_OPTIONS_HPP

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace kore {
    enum class TraceOption {
        None = 0,
        Scan,
        Parse,
        TypeInference,
        TypeCheck,
        Kir,
        Codegen,
    };

    enum class DumpOption {
        None = 0,
        Scan,
        Parse,
        Kir,
        Codegen,
    };

    struct ParsedCommandLineArgs {
        // Why parsing failed, empty if successful
        std::string error_message;

        // TODO: Change dump args to --dump (scan|parse|ast|kir)?
        bool timings = false;
        bool execute = false;
        TraceOption trace = TraceOption::None;
        DumpOption dump = DumpOption::None;
        bool disassemble = false;
        std::string target;
        bool version = false;
        bool version_only = false;
        bool help = false;
        int verbosity = 0;
        bool env_vars = false;
        bool colors = false;
        bool compile_only = false;
        bool typecheck_only = false;
        bool mem_stats = false;

        std::string expr;
        std::vector<fs::path> paths;
    };

    ParsedCommandLineArgs parse_commandline(int argc, char** args);

    void print_help_message();
}

#endif // KORE_OPTIONS_HPP
