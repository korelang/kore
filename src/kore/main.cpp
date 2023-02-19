#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include "logging/logging.hpp"
#include "module.hpp"
#include "module_loader.hpp"
#include "options.hpp"
#include "repl.hpp"
#include "version.hpp"
#include "vm/vm.hpp"

namespace fs = std::filesystem;

namespace kore {
    const std::string PROGRAM_NAME = "kore";

    int run_vm(const ParsedCommandLineArgs& args) {
        vm::Vm vm;

        if (args.repl_mode) {
            Repl repl;
            repl.run();
        } else {
            std::error_code error_code;

            if (!fs::exists(args.path, error_code)) {
                if (error_code) {
                    kore::error("file '%s' does not exist", args.path.c_str());
                    return 1;
                } else {
                    kore::error(
                        "file '%s' does not exist (os: %s, code: %d)",
                        args.path.c_str(),
                        error_code.message().c_str(),
                        error_code.value()
                    );
                    return error_code.value();
                }
            }

            vm.run_path(args.path);

            if (args.dump_registers) {
                info_group("vm", "dumping registers");
                vm.dump_registers(std::cerr);
            }
        }

        return 0;
    }

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

        try {
            return run_vm(args);
        } catch (std::runtime_error& ex) {
            kore::error("vm error: %s", ex.what());
            return 1;
        }

        return 0;
    }
}

int main(int argc, char** argv) {
    return kore::run(argc, argv);
}
