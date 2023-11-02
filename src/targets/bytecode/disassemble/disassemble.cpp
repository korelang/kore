#include <array>
#include <fstream>

#include "disassemble.hpp"
#include "disassemble_error.hpp"
#include "targets/bytecode/register.hpp"
#include "utils/endian.hpp"

// TODO: Add checks for stream reads
namespace koredis {
    using BytecodeMagic = std::array<char, 4>;
    using BytecodeVersion = std::array<char, 3>;

    BytecodeMagic bytecode_magic {'k', 'o', 'r', 'e'};

    std::string read_string(std::istream& is) {
        auto size = kore::read_be32(is);

        // NOTE: Use std::byte if we end up using C++17
        std::vector<char> bytes(size);

        is.read(bytes.data(), size);

        //data.resize(is.gcount());

        return std::string(bytes.cbegin(), bytes.cend());
    } 

    void read_magic(std::istream& is) {
        BytecodeMagic magic_header;

        is.read(magic_header.data(), 4);

        if (magic_header != bytecode_magic) {
            throw DisassembleError("not a kore bytecode file", is.tellg());
        }
    }

    BytecodeVersion read_compiler_version(std::istream& is) {
        BytecodeVersion compiler_version;

        is.read(compiler_version.data(), 3);

        return compiler_version;
    }

    BytecodeVersion read_bytecode_version(std::istream& is) {
        BytecodeVersion bytecode_version;

        is.read(bytecode_version.data(), 3);

        return bytecode_version;
    }

    void disassemble_constant_table(std::istream& is, kore::Module& module) {
        auto constant_table_tag = kore::read_be32(is);
        auto constant_table_size = kore::read_be32(is);

        for (decltype(constant_table_size) i = 0; i < constant_table_size; ++i) {
            // TODO: Fix signedness of values
            switch (static_cast<kore::ConstantTableTag>(constant_table_tag)) {
                case kore::ConstantTableTag::I32: {
                    module.add_i32_constant(kore::read_be32(is));
                    break;
                }

                /* case kore::ConstantTableTag::I64: */
                /*     module.add_i64_constant(kore::read_be32(ifs)); */
                /*     break; */

                default:
                    break;
            }
        }
    }

    void disassemble_global_indices_count(std::istream& is, kore::Module& module) {
        module.set_global_indices_count(kore::read_be32(is));
    }

    void disassemble_constant_tables(std::istream& is, kore::Module& module) {
        disassemble_constant_table(is, module);
        /* disassemble_constant_table(ifs, module); */
    }

    int closest_n(int value, int n) {
        return ((value + (n - 1)) >> 5) << 5;
    }

    // TODO: Create a byte_io.hpp file for safe reading of bytes
    void read_bytes(
        std::istream& is,
        std::size_t count,
        std::vector<std::uint32_t>& buffer
    ) {
        int bytes_to_read = count;
        std::array<char, 4> data;

        while (bytes_to_read > 0) {
            data.fill(0);
            is.read(data.data(), bytes_to_read >= 4 ? 4 : bytes_to_read);

            buffer.push_back(
                (data[0] & 0xff) << 24 |
                (data[1] & 0xff) << 16 |
                (data[2] & 0xff) << 8  |
                (data[3] & 0xff)
            );

            bytes_to_read -= 4;
        }
    }

    void disassemble_instruction(
        std::istream& is,
        std::vector<kore::bytecode_type>& instructions
    ) {
        auto instruction = kore::read_be32(is);
        auto opcode = GET_OPCODE(instruction);

        if (!kore::is_variable_length(opcode)) {
            instructions.push_back(instruction);
        } else {
            // Handle variable-length instructions
            switch (opcode) {
                case kore::Bytecode::Call: {
                    instructions.push_back(instruction);
                    int arg_count = GET_REG2(instruction);
                    int ret_count = GET_REG3(instruction);
                    auto total_count = arg_count + ret_count;

                    // Read the argument and return registers
                    if (total_count > 0) {
                        read_bytes(is, total_count, instructions);
                    }

                    break;
                }

                case kore::Bytecode::Ret: {
                    int reg_count = GET_REG1(instruction);
                    instructions.push_back(instruction);

                    // Read the return registers accounting for the two
                    // registers that are part of the first 32 bits of the
                    // instruction
                    if (reg_count > 2) {
                        read_bytes(is, reg_count, instructions);
                    }

                    break;
                }

                default:
                    // TODO: Also provide the unknown code
                    throw DisassembleError("Unknown opcode", is.tellg(), opcode);
            }
        }
    }

    void disassemble_function(std::istream& is, kore::Module& module) {
        std::string name = read_string(is);

        auto lnum = kore::read_be32(is);
        auto start = kore::read_be32(is);
        auto end = kore::read_be32(is);
        auto func_index = kore::read_be32(is);
        /* auto locals_count = kore::read_be32(is); */
        auto reg_count = kore::read_be32(is);
        auto code_size = kore::read_be32(is);
        std::vector<kore::bytecode_type> instructions;

        for (decltype(code_size) i = 0; i < code_size; ++i) {
            disassemble_instruction(is, instructions);
        }

        module.add_function(name, lnum, start, end, func_index, 0, reg_count, instructions);
    }

    void disassemble_functions(std::istream& is, kore::Module& module) {
        std::uint32_t function_count = kore::read_be32(is);

        for (std::uint32_t i = 0; i < function_count; ++i) {
            disassemble_function(is, module);
        }
    }

    kore::Module disassemble_module(std::istream& is) {
        auto module_index = kore::read_be32(is);
        auto module_path = read_string(is);
        kore::Module module{module_index, module_path};

        disassemble_constant_table(is, module);
        disassemble_functions(is, module);

        return module;
    }

    kore::Module disassemble_modules_from_path(const fs::path& path) {
        std::ifstream ifs{path};

        if (!ifs.is_open()) {
            throw DisassembleError("failed to open file");
        }

        // Read magic header
        read_magic(ifs);

        // Read compiler/bytecode versions
        BytecodeVersion compiler_version = read_compiler_version(ifs);
        BytecodeVersion bytecode_version = read_bytecode_version(ifs);

        // Read global indices count
        kore::read_be32(ifs);
        /* koredis::disassemble_global_indices_count(ifs, module); */

        // Read main module index
        kore::read_be32(ifs);

        // Read main function index
        kore::read_be32(ifs);

        auto module_count = kore::read_be32(ifs);

        if (module_count == 0) {
            throw DisassembleError("zero modules in file", ifs.tellg());
        } else if (module_count > 1) {
            throw DisassembleError("only one module is currently supported", ifs.tellg());
        }

        auto module = disassemble_module(ifs);
        module.set_compiler_version(compiler_version);
        module.set_bytecode_version(bytecode_version);

        return module;
    }
}
