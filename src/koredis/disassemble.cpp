#include <array>

#include "disassemble.hpp"
#include "disassemble_error.hpp"

namespace koredis {
    std::array<char, 4> bytecode_magic {'k', 'o', 'r', 'e'};

    void read_magic(std::istream& is) {
        std::array<char, 4> magic_header;

        is.read(magic_header.data(), 4);

        if (magic_header != bytecode_magic) {
            throw DisassembleError("not a kore bytecode file", is.tellg());
        }
    }

    std::array<char, 3> read_compiler_version(std::istream& is) {
        std::array<char, 3> compiler_version;

        is.read(compiler_version.data(), 3);

        return compiler_version;
    }

    std::array<char, 3> read_bytecode_version(std::istream& is) {
        std::array<char, 3> bytecode_version;

        is.read(bytecode_version.data(), 3);

        return bytecode_version;
    }

    kore::Module disassemble_module_from_path(const std::string& path) {
        std::ifstream ifs{path};

        if (!ifs.is_open()) {
            throw DisassembleError("failed to open file");
        }

        // Read magic header and versions
        read_magic(ifs);

        kore::Module module;
        module.set_compiler_version(read_compiler_version(ifs));
        module.set_bytecode_version(read_bytecode_version(ifs));

        // Read constant tables

        // Read functions

        return module;
    }
}
