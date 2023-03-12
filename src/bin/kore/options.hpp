#ifndef KORE_OPTIONS_HPP
#define KORE_OPTIONS_HPP

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace kore {
    struct ParsedCommandLineArgs {
        std::string error_message;

        bool version;
        bool version_only;
        bool help;
        bool repl_mode;
        bool dump_registers;

        fs::path path;
    };

    ParsedCommandLineArgs parse_commandline(int argc, char** args);
    void print_help_message();
}

#endif // KORE_OPTIONS_HPP
