#ifndef KORE_BYTECODE_FORMAT_WRITER_HPP
#define KORE_BYTECODE_FORMAT_WRITER_HPP

#include "targets/bytecode/constant_table.hpp"
#include "targets/bytecode/vm/value_type.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace kore {
    class Module;
    class CompiledObject;

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
            void write_header(const Module* const module, std::ostream& os);
            void write_global_indices_count(const Module* const module, std::ostream& os);
            void write_constant_table(const ConstantTable& table, vm::ValueTag tag, std::ostream& os);
            void write_constant_tables(const Module* const module, std::ostream& os);
            void write_value(vm::Value& value, std::ostream& os);
            void write_string(const std::string& s, std::ostream& os);
            void write_functions(const Module* const module, std::ostream& os);
            void write_main_code(Module* const module, std::ostream& os);
            void write_object(const CompiledObject* const object, std::ostream& os);
    };
}

#endif // KORE_BYTECODE_FORMAT_WRITER_HPP
