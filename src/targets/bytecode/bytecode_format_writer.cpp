#include "bytecode_format_writer.hpp"
#include "module.hpp"
#include "targets/bytecode/vm/value_type.hpp"
#include "targets/bytecode/vm/values/function_value.hpp"
#include "utils/unused_parameter.hpp"
#include "utils/endian.hpp"

#include <fstream>
#include <limits>
#include <type_traits>

namespace kore {
    BytecodeFormatWriter::BytecodeFormatWriter() {}

    BytecodeFormatWriter::~BytecodeFormatWriter() {}

    void BytecodeFormatWriter::write(Module* const module, const fs::path& path) {
        std::ofstream os(path, std::ofstream::out | std::ofstream::binary);

        if (!os.is_open()) {
            throw std::runtime_error("Failed to open path");
        }

        write_header(module, os);
        write_global_indices_count(module, os);
        write_constant_tables(module, os);
        write_functions(module, os);
        write_main_code(module, os);

        os.close();
    }

    void BytecodeFormatWriter::write_header(const Module* const module, std::ostream& os) {
        UNUSED_PARAM(module);
        os.write("kore", 4);

        // Write compiler and bytecode versions
        char compiler_version[] = {1, 0, 0};
        os.write(compiler_version, 3);

        char bytecode_version[] = {1, 0, 0};
        os.write(bytecode_version, 3);
    }

    void BytecodeFormatWriter::write_global_indices_count(const Module* const module, std::ostream& os) {
        write_be32(module->global_indices_count(), os);
    }

    void BytecodeFormatWriter::write_constant_table(
        const ConstantTable& table,
        vm::ValueTag tag,
        std::ostream& os
    ) {
        // TODO: Make sure this is not a size_t since platform-dependent sizes
        // won't work if the code is compiled on a 32-bit platform and
        // disassembled on a 64-bit platform or vice versa
        //
        // TODO: Ensure that the constant table cannot exceed unsigned 32-bit
        //
        // TODO: Check max table values for all tables
        write_be32(static_cast<std::uint32_t>(tag), os);
        write_be32(static_cast<std::uint32_t>(table.size()), os);

        for (auto it = table.tagged_cbegin(tag); it != table.tagged_cend(tag); ++it) {
            auto value = table.get(*it);
            write_value(value, os);
        }
    }

    void BytecodeFormatWriter::write_constant_tables(
        const Module* const module,
        std::ostream& os
    ) {
        write_constant_table(module->constant_table(), vm::ValueTag::I32, os);
    }

    void BytecodeFormatWriter::write_value(vm::Value& value, std::ostream& os) {
        write_be32(static_cast<std::uint32_t>(value.tag), os);

        switch (value.tag) {
            case vm::ValueTag::Bool: {
                write_be32(value.as_bool() ? 1 : 0, os);
                break;
            }

            case vm::ValueTag::I32: {
                write_be32(value.as_i32(), os);
                break;
            }

            case vm::ValueTag::I64: {
                write_be32(value.as_i32(), os);
                break;
            }

            case vm::ValueTag::F32: {
                write_be32(value.as_i32(), os);
                break;
            }

            case vm::ValueTag::F64: {
                write_be32(value.as_i32(), os);
                break;
            }

            default:
                break;
        }
    }

    void BytecodeFormatWriter::write_string(const std::string& s, std::ostream& os) {
        if (s.empty()) {
            throw std::runtime_error("Attempt to write empty string");
        }

        if (s.size() >= std::numeric_limits<short>::max()) {
            // TODO: Move to common errors
            throw std::runtime_error("Can only write strings less than");
        }

        write_be32(static_cast<std::uint32_t>(s.size()), os);

        // TODO: Will this work for utf-8 strings?
        // Do not include the null-terminator
        os.write(s.data(), sizeof(std::string::value_type) * s.size());
    }

    void BytecodeFormatWriter::write_functions(const Module* const module, std::ostream& os) {
        write_be32(module->objects_count(), os);

        for (auto it = module->objects_begin(); it != module->objects_end(); ++it) {
            if (it->get()->is_main_object()) {
                continue;
            }

            write_object(it->get(), os);
        }
    }

    void BytecodeFormatWriter::write_main_code(Module* const module, std::ostream& os) {
        write_object(module->main_object(), os);
    }

    void BytecodeFormatWriter::write_object(const CompiledObject* const object, std::ostream& os) {
        write_string(object->name(), os);

        auto location = object->location();

        /* write_string(location.path(), os); */
        write_be32(location.lnum(), os);
        write_be32(location.start(), os);
        write_be32(location.end(), os);
        write_be32(object->func_index(), os);
        write_be32(object->locals_count(), os);
        write_be32(object->max_regs_used(), os);

        write_be32(object->code_size(), os);

        for (auto instruction : *object) {
            write_be32(instruction, os);
        }
    }
}
