#include <limits>

#include "bytecode_format_writer.hpp"
#include "utils/endian.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    BytecodeFormatWriter::BytecodeFormatWriter() {}

    BytecodeFormatWriter::~BytecodeFormatWriter() {}

    void BytecodeFormatWriter::write(
        Module* const module,
        const std::string& path
    ) {
        std::ofstream ofs(path, std::ofstream::out | std::ofstream::binary);

        if (!ofs.is_open()) {
            throw std::runtime_error("Failed to open path");
        }

        write_header(module, ofs);
        write_constant_table(module, ofs);
        write_functions(module, ofs);
        write_main_code(module, ofs);

        ofs.close();
    }

    void BytecodeFormatWriter::write_header(
        const Module* const module,
        std::ofstream& ofs
    ) {
        UNUSED_PARAM(module);
        ofs.write("kore", 4);

        // Write compiler and bytecode versions
        char compiler_version[] = {1, 0, 0};
        ofs.write(compiler_version, 3);

        char bytecode_version[] = {1, 0, 0};
        ofs.write(bytecode_version, 3);
    }

    void BytecodeFormatWriter::write_constant_table(
        const Module* const module,
        std::ofstream& ofs
    ) {
        write_big_endian(module->constants_count(), ofs);

        for (auto it = module->i32_constants_begin(); it != module->i32_constants_end(); ++it) {
            write_big_endian(*it, ofs);
        }
    }

    void BytecodeFormatWriter::write_value(const Value& value, std::ofstream& ofs) {
        write_big_endian(value.tag, ofs);

        switch (value.tag) {
            case ValueTag::Bool:
                write_big_endian(value.as_bool() ? 1 : 0, ofs);
                break;

            case ValueTag::I32:
                write_big_endian(value.as_i32(), ofs);
                break;

            default:
                break;
        }
    }

    void BytecodeFormatWriter::write_string(const std::string& s, std::ofstream& ofs) {
        if (s.empty()) {
            throw std::runtime_error("Attempt to write empty string");
        }

        if (s.size() >= std::numeric_limits<short>::max()) {
            // TODO: Move to common errors
            throw std::runtime_error("Can only write strings less than");
        }

        write_big_endian(static_cast<short>(s.size()), ofs);

        // Do not include the null-terminator
        ofs.write(s.data(), sizeof(std::string::value_type) * s.size() - 1);
    }

    void BytecodeFormatWriter::write_functions(const Module* const module, std::ofstream& ofs) {
        write_big_endian(module->objects_count(), ofs);

        for (auto it = module->objects_begin(); it != module->objects_end(); ++it) {
            write_object(it->get(), ofs);
        }
    }

    void BytecodeFormatWriter::write_main_code(Module* const module, std::ofstream& ofs) {
        write_object(module->main_object(), ofs);
    }

    void BytecodeFormatWriter::write_object(const CompiledObject* const object, std::ofstream& ofs) {
        write_string(object->name(), ofs);

        if (!object->is_main_object()) {
            auto location = object->location();

            /* write_string(location.path(), ofs); */
            write_big_endian(location.lnum(), ofs);
            write_big_endian(location.start(), ofs);
            write_big_endian(location.end(), ofs);
        }

        write_big_endian(object->locals_count(), ofs);
        write_big_endian(object->reg_count(), ofs);

        write_big_endian(object->code_size(), ofs);

        for (auto instruction : *object) {
            write_big_endian(instruction, ofs);
        }
    }
}
