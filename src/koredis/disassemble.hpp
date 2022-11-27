#ifndef KOREDIS_DISASSEMBLE_HPP
#define KOREDIS_DISASSEMBLE_HPP

#include <array>
#include <filesystem>
#include <fstream>

#include "module.hpp"

namespace fs = std::filesystem;

namespace koredis {
    void read_magic(std::istream& is);

    std::array<char, 3> read_compiler_version(std::istream& is);

    std::array<char, 3> read_bytecode_version(std::istream& is);

    kore::Module disassemble_module_from_path(const fs::path& path);
}

#endif // KOREDIS_DISASSEMBLE_HPP
