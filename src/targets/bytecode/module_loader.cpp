#include <stdexcept>
#include <fstream>

#include "targets/bytecode/module_load_error.hpp"
#include "targets/bytecode/module_loader.hpp"
#include "utils/endian.hpp"

namespace kore {
    using BytecodeMagic = std::array<char, 4>;
    using BytecodeVersion = std::array<char, 3>;

    BytecodeMagic bytecode_magic {'k', 'o', 'r', 'e'};

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

    void read_magic(std::istream& is) {
        BytecodeMagic magic_header;

        is.read(magic_header.data(), 4);

        if (magic_header != bytecode_magic) {
            throw ModuleLoadError("not a kore bytecode file", is.tellg());
        }
    }

    BytecodeVersion read_version(std::istream& is) {
        BytecodeVersion version;

        is.read(version.data(), 3);

        return version;
    }

    std::string read_string(std::istream& is) {
        auto size = kore::read_be32(is);
        std::vector<char> bytes(size);

        is.read(bytes.data(), size);

        return std::string(bytes.cbegin(), bytes.cend());
    } 

    void load_constant_table(std::istream& is, kore::Module& module) {
        auto constant_table_tag = kore::read_be32(is);
        auto constant_table_size = kore::read_be32(is);

        for (decltype(constant_table_size) i = 0; i < constant_table_size; ++i) {
            // TODO: Fix signedness of values
            switch (static_cast<kore::ConstantTableTag>(constant_table_tag)) {
                case kore::ConstantTableTag::I32: {
                    module.add_i32_constant(kore::read_be32(is));
                    break;
                }

                default:
                    break;
            }
        }
    }

    void load_instruction(
        std::istream& is,
        std::vector<kore::bytecode_type>& instructions
    ) {
        auto instruction = kore::read_be32(is);
        auto opcode = GET_OPCODE(instruction);

        if (!kore::is_variable_length_opcode(opcode)) {
            instructions.push_back(instruction);
        } else {
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
                    throw ModuleLoadError("Unknown opcode", is.tellg(), opcode);
            }
        }
    }

    void load_function(std::istream& is, kore::Module& module) {
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
            load_instruction(is, instructions);
        }

        module.add_function(
            name,
            lnum,
            start,
            end,
            func_index,
            0,
            reg_count,
            instructions
        );
    }

    void load_functions(std::istream& is, kore::Module& module) {
        std::uint32_t function_count = kore::read_be32(is);

        for (std::uint32_t i = 0; i < function_count; ++i) {
            load_function(is, module);
        }
    }

    kore::Module load_module(std::istream& is) {
        auto module_index = kore::read_be32(is);
        auto module_path = read_string(is);
        kore::Module module{ module_index, module_path };

        load_constant_table(is, module);
        load_functions(is, module);

        return module;
    }

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
        read_magic(is);

        // Read compiler/bytecode versions
        BytecodeVersion compiler_version = read_version(is);
        BytecodeVersion bytecode_version = read_version(is);

        // Read global indices count
        auto global_indices_count = kore::read_be32(is);

        // Read main module index
        kore::read_be32(is);

        // Read main function index
        kore::read_be32(is);

        auto module_count = kore::read_be32(is);

        if (module_count == 0) {
            throw ModuleLoadError("zero modules in file", is.tellg());
        } else if (module_count > 1) {
            throw ModuleLoadError("only one module is currently supported", is.tellg());
        }

        auto module = load_module(is);

        module.set_compiler_version(compiler_version);
        module.set_bytecode_version(bytecode_version);
        module.set_global_indices_count(global_indices_count);

        return module;
    }
}
