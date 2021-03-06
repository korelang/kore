#include <iostream>
#include <string>

#include "disassemble.hpp"
#include "disassemble_error.hpp"
#include "module.hpp"
#include "output_module.hpp"
#include "logging/logging.hpp"
#include "options.hpp"
#include "version.hpp"

namespace koredis {
    const std::string PROGRAM_NAME = "koredis";

    void print_version(const std::string& name, kore::Version version, bool version_only) {
        if (version_only) {
            std::cout << version.major << "."
                      << version.minor << "."
                      << version.patch
                      << std::endl;
        } else {
            kore::info(
                "%s v%d.%d.%d",
                name.c_str(),
                version.major,
                version.minor,
                version.patch
            );
        }
    }

    int run(int argc, char** argv) {
        const std::string prog_name = "koredis";
        const kore::Version CURRENT_VERSION{ 0, 1, 0 };
        auto args = parse_commandline(argc, argv);

        if (args.error_message.size() > 0) {
            kore::error("%s", args.error_message.c_str());
            return 1;
        } else if (args.version) {
            print_version(prog_name, CURRENT_VERSION, false);
            return 0;
        } else if (args.version_only) {
            print_version(prog_name, CURRENT_VERSION, true);
            return 0;
        } else if (args.help) {
            print_help_message();
            return 0;
        }

        for (auto& path : args.paths) {
            if (args.paths.size() > 1 || args.verbosity > 0) {
                kore::info("disassembling '%s'", path.c_str());
            }

            try {
                kore::Module module = disassemble_module_from_path(path);

                output_module(std::cout, module, args.colors, args.porcelain, args.verbosity);
            } catch (DisassembleError& ex) {
                kore::error("disassemble failed: %s", path.c_str(), ex.what());
            }
        }

        return 0;
    }
}

int main(int argc, char** argv) {
    return koredis::run(argc, argv);
}
