#include <stdexcept>
#include <fstream>

#include "targets/bytecode/disassemble/disassemble.hpp"
#include "targets/bytecode/module_loader.hpp"

namespace kore {
    kore::Module load_module_from_path(const fs::path& path) {
        std::ifstream ifs(path);

        if (!ifs) {
            throw std::runtime_error("failed to open file");
        }

        return load_module_from_stream(ifs);
    }

    kore::Module load_module_from_stream(std::istream& is) {
        if (!is) {
            throw std::runtime_error("failed to read from stream");
        }

        // Read magic header
        koredis::read_magic(is);

        // Read compiler/bytecode versions
        kore::Module module;
        module.set_compiler_version(koredis::read_compiler_version(is));
        module.set_bytecode_version(koredis::read_bytecode_version(is));

        koredis::disassemble_global_indices_count(is, module);

        // Read constant tables
        koredis::disassemble_constant_tables(is, module);

        // Read functions
        koredis::disassemble_functions(is, module);

        return module;
    }
}
