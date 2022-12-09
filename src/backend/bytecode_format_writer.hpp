#ifndef KORE_BYTECODE_FORMAT_WRITER_HPP
#define KORE_BYTECODE_FORMAT_WRITER_HPP

#include <filesystem>

#include "codegen/compiled_object.hpp"
#include "constant_table.hpp"
#include "utils/endian.hpp"
#include "vm/value_type.hpp"

namespace fs = std::filesystem;

namespace kore {
    class Module;

    // Tags to identify objects
    enum class ObjectTag {
        Function,
    };

    // TODO: Break class into separate functions instead? There is no state
    class BytecodeFormatWriter final {
        public:
            BytecodeFormatWriter();
            virtual ~BytecodeFormatWriter();

            // TODO: Make it possible to write this to a bytecode stream that
            // can be directly executed by the vm or should the vm just be able
            // to execute module's directly?
            void write(Module* const module, const fs::path& path);

        private:
            void write_header(const Module* const module, std::ostream& ofs);
            void write_constant_tables(const Module* const module, std::ostream& ofs);
            void write_value(const vm::Value& value, std::ostream& ofs);
            void write_string(const std::string& s, std::ostream& ofs);
            void write_functions(const Module* const module, std::ostream& ofs);
            void write_main_code(Module* const module, std::ostream& ofs);
            void write_object(const CompiledObject* const object, std::ostream& ofs);

            template<typename T>
            void write_constant_table(const ConstantTable<T>& table, std::ostream& ofs) {
                // TODO: Make sure this is not a size_t since platform-dependent sizes
                // won't work if the code is compiled on a 32-bit platform and
                // disassembled on a 64-bit platform or vice versa
                //
                // TODO: Ensure that the constant table cannot exceed unsigned 32-bit
                //
                // TODO: Check max table values for all tables
                write_be32(static_cast<T>(table.tag()), ofs);
                write_be32(static_cast<T>(table.size()), ofs);

                for (auto it = table.sorted_cbegin(); it != table.sorted_cend(); ++it) {
                    // TODO: Does not work for 64-bit integers!
                    write_be32(*it, ofs);
                }
            }
    };
}

#endif // KORE_BYTECODE_FORMAT_WRITER_HPP
