#ifndef KORE_BYTECODE_GENERATOR_HPP
#define KORE_BYTECODE_GENERATOR_HPP

#include <map>
#include <memory>
#include <set>
#include <vector>

#include "ast/expressions/string_expression.hpp"
#include "ast/ast.hpp"
#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/codegen/bytecode_array_writer.hpp"
#include "targets/bytecode/codegen/kir/function.hpp"
#include "targets/bytecode/codegen/kir/kir.hpp"
#include "targets/bytecode/compiled_object.hpp"
#include "targets/bytecode/module.hpp"
#include "targets/bytecode/register.hpp"
#include "types/scope.hpp"

namespace kore {
    class Module;

    /// Class for generating bytecode from KIR graphs
    class BytecodeGenerator2 final {
        public:
            using reg_iterator = std::vector<const Reg>::iterator;

        public:
            BytecodeGenerator2();
            virtual ~BytecodeGenerator2();

            std::vector<std::uint8_t> generate(kir::Kir& kir);

        private:
            std::vector<std::uint8_t> _buffer;
            std::vector<std::size_t> _block_offsets;
            std::vector<std::size_t> _patch_locations;

        private:
            void generate_for_module(const kir::Module& module);
            void generate_for_function(const kir::Function& function);
            void generate_for_block(kir::BasicBlock& block);
            void generate_for_instruction(kir::Instruction& instruction);

            void patch_jumps();

            void write_bytes(const std::string& str);
            void write_bytes(std::initializer_list<std::uint8_t> bytes);
            void write_bytes();
            void write_string(const std::string& str);
            void write_le32(std::uint32_t value);

            /* template<typename... Args> */
            /* void write_bytes(Args&&... args) { */
            /*     _buffer.emplace_back(std::forward(args...)); */
            /* } */

            template<typename T>
            void write_constant_table(const ConstantTable<T>& table) {
                write_le32(static_cast<T>(table.tag()));

                if (table.size() > UINT32_MAX) {
                    // TOOD: How do we handle errors in code generation?
                }

                write_le32(static_cast<T>(table.size()));

                for (auto it = table.sorted_cbegin(); it != table.sorted_cend(); ++it) {
                    write_le32(*it);
                }
            }
    };
}

#endif // KORE_BYTECODE_GENERATOR_HPP
