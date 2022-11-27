#ifndef KOREDIS_OPTIONS_HPP
#define KOREDIS_OPTIONS_HPP

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace koredis {
    struct ParsedCommandLineArgs {
        std::string error_message;

        bool colors;
        bool porcelain;
        bool version;
        bool version_only;
        bool help;
        int verbosity;

        std::vector<fs::path> paths;
    };

    ParsedCommandLineArgs parse_commandline(int argc, char** args);
    void print_help_message();
}

#endif // KOREDIS_OPTIONS_HPP
