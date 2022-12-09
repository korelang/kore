#ifndef KOREDIS_DISASSEMBLE_HPP
#define KOREDIS_DISASSEMBLE_HPP

#include <array>
#include <filesystem>

#include "module.hpp"

namespace fs = std::filesystem;

namespace koredis {
    void read_magic(std::istream& is);

    std::array<char, 3> read_compiler_version(std::istream& is);

    std::array<char, 3> read_bytecode_version(std::istream& is);

    void disassemble_constant_tables(std::istream& is, kore::Module& module);

    void disassemble_functions(std::istream& is, kore::Module& module);

    kore::Module disassemble_module_from_path(const fs::path& path);
}

#endif // KOREDIS_DISASSEMBLE_HPP
