#ifndef KOREDIS_DISASSEMBLE_HPP
#define KOREDIS_DISASSEMBLE_HPP

#include <array>
#include <fstream>

#include "module.hpp"

namespace koredis {
    void read_magic(std::istream& is);

    std::array<char, 3> read_compiler_version(std::istream& is);

    std::array<char, 3> read_bytecode_version(std::istream& is);

    kore::Module disassemble_module_from_path(const std::string& path);
}

#endif // KOREDIS_DISASSEMBLE_HPP
