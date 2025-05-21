#ifndef KORE_BYTECODE_GENERATOR_HPP
#define KORE_BYTECODE_GENERATOR_HPP

#include <map>
#include <vector>

#include "targets/bytecode/codegen/kir/block_id.hpp"
#include "targets/bytecode/codegen/kir/function.hpp"
#include "targets/bytecode/codegen/kir/kir.hpp"
#include "targets/bytecode/register.hpp"
#include "targets/bytecode/vm/value_type.hpp"

namespace kore {
    class Module;

    /// Class for generating bytecode from KIR graphs
    class BytecodeGenerator final {
        public:
            using reg_iterator = std::vector<Reg>::iterator;

        public:
            BytecodeGenerator();
            virtual ~BytecodeGenerator();

            std::vector<std::uint8_t> generate(kir::Kir& kir);

        private:
            std::vector<std::uint8_t> _buffer;
            std::map<kir::BlockId, std::size_t> _block_offsets;
            std::vector<std::pair<std::size_t, kir::BlockId>> _patch_locations;

        private:
            void generate_for_module(const kir::Module& module);
            void generate_for_function(const kir::Function& function);
            void generate_for_block(kir::BasicBlock& block);
            void generate_for_instruction(kir::Instruction& instruction);

            void save_patch_location(kir::BlockId target_block_id);
            void patch_jumps();

            void write_value(const vm::Value& value);
            void write_constant_table(const ConstantTable& table);
            void write_bytes(const std::string& str);
            void write_bytes(std::vector<std::uint8_t>& bytes);
            void write_bytes(std::initializer_list<std::uint8_t> bytes);
            void write_bytes();
            void write_aligned_bytes(std::vector<std::uint8_t>& bytes, int alignment);
            void write_string(const std::string& str);
            void write_raw(std::uint32_t value);
            void write_be32(std::uint32_t value);

            /* template<typename... Args> */
            /* void write_bytes(Args&&... args) { */
            /*     _buffer.emplace_back(std::forward(args...)); */
            /* } */
    };
}

#endif // KORE_BYTECODE_GENERATOR_HPP
