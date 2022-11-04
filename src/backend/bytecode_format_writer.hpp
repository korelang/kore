#ifndef KORE_BYTECODE_FORMAT_WRITER_HPP
#define KORE_BYTECODE_FORMAT_WRITER_HPP

#include <fstream>

#include "module.hpp"

namespace kore {
    // Tags to identify entries in the constant table
    enum class ConstantTableTag {
        Bool = 0,
        I32,
        I64,
        F32,
        F64,
        Str,
    };

    // Tags to identify objects
    enum class ObjectTag {
        Function,
    };

    // TODO: Break class into separate functions instead? There is no state
    class BytecodeFormatWriter final {
        public:
            BytecodeFormatWriter();
            virtual ~BytecodeFormatWriter();

            // TODO: Make it possible to write this to a bytecode stream that can be directly
            // executed by the vm or should the vm just be able to execute module's directly?
            void write(Module* const module, const std::string& path);

        private:
            void write_header(const Module* const module, std::ofstream& ofs);
            void write_constant_table(const Module* const module, std::ofstream& ofs);
            void write_value(const vm::Value& value, std::ofstream& ofs);
            void write_string(const std::string& s, std::ofstream& ofs);
            void write_functions(const Module* const module, std::ofstream& ofs);
            void write_main_code(Module* const module, std::ofstream& ofs);
            void write_object(const CompiledObject* const object, std::ofstream& ofs);
    };
}

#endif // KORE_BYTECODE_FORMAT_WRITER_HPP
