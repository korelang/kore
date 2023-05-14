#include <iostream>
#include <string>

#include "logging/logging.hpp"
#include "options.hpp"
#include "output_module.hpp"
#include "targets/bytecode/disassemble/disassemble.hpp"
#include "targets/bytecode/disassemble/disassemble_error.hpp"
#include "targets/bytecode/module.hpp"
#include "version.hpp"

namespace koredis {
    const std::string PROGRAM_NAME = "koredis";

    int run(int argc, char** argv) {
        const kore::Version CURRENT_VERSION{ 0, 1, 0 };
        auto args = parse_commandline(argc, argv);

        if (args.error_message.size() > 0) {
            kore::error("%s", args.error_message.c_str());
            return 1;
        } else if (args.version || args.version_only) {
            print_version(PROGRAM_NAME, CURRENT_VERSION, args.version_only);
            return 0;
        } else if (args.help) {
            print_help_message();
            return 0;
        }

        for (auto& path : args.paths) {
            if (args.paths.size() > 1 || args.verbosity > 0) {
                kore::info("disassembling '%s'\n", path.c_str());
            }

            try {
                kore::Module module = disassemble_modules_from_path(path);

                output_module(std::cout, module, args.colors, args.porcelain, args.verbosity);
            } catch (DisassembleError& ex) {
                kore::error("disassembly of '%s' failed: %s", path.c_str(), ex.what());
            }
        }

        return 0;
    }
}

int main(int argc, char** argv) {
    return koredis::run(argc, argv);
}
