#ifndef KORE_BYTECODE_ARRAY_WRITER_HPP
#define KORE_BYTECODE_ARRAY_WRITER_HPP

#include <map>
#include <vector>

#include "codegen/bytecode/bytecode.hpp"
#include "codegen/compiled_object.hpp"
#include "register.hpp"

namespace kore {
    using Label = int;

    class BytecodeArrayWriter final {
        public:
            BytecodeArrayWriter();
            ~BytecodeArrayWriter();

            void write_header();
            void write_opcode(Bytecode opcode, CompiledObject* target);
            void write_load(Bytecode opcode, Reg reg, i32 value, CompiledObject* target);
            void write_1address(Bytecode opcode, Reg reg, CompiledObject* target);
            void write_2address(Bytecode opcode, Reg destination_reg, Reg reg_operand, CompiledObject* target);
            void write_3address(Bytecode opcode, Reg destination_reg, Reg reg_operand1, Reg reg_operand2, CompiledObject* target);

            template<typename InputIterator>
            void write_variable_length(
                Bytecode opcode,
                int size,
                Reg retreg,
                InputIterator first,
                InputIterator last,
                CompiledObject* target
            ) {
                std::vector<std::uint8_t> bytes{
                    static_cast<std::uint8_t>(opcode),
                    static_cast<std::uint8_t>(size),
                    static_cast<std::uint8_t>(retreg)
                };
                bytes.insert(bytes.end(), first, last);
                std::size_t i = 0;

                // Pack every four bytes into a 32-bit big-endian integer
                for (; i < bytes.size() / 4; ++i) {
                    int idx = i * 4;

                    bytecode_type instruction =
                          (bytes[idx + 0] << 24)
                        | (bytes[idx + 1] << 16)
                        | (bytes[idx + 2] << 8)
                        | (bytes[idx + 3] << 0);

                    target->add_instruction(instruction);
                }

                int remaining_bytes = bytes.size() % 4;

                // Pack any remaining (<4) bytes into a 32-bit big-endian integer
                // padded with zeroes
                if (remaining_bytes > 0) {
                    int idx = i * 4;

                    bytecode_type instruction = 0;
                    instruction |= bytes[idx] << 24;

                    if (remaining_bytes > 1) {
                        instruction |= bytes[idx + 1] << 16;
                    }

                    if (remaining_bytes > 2) {
                        instruction |= bytes[idx + 2] << 8;
                    }

                    target->add_instruction(instruction);
                }
            }

            void save_patch_location(CompiledObject* target);
            Label write_jump(Bytecode opcode, CompiledObject* target);
            Label write_jump(Bytecode opcode, Reg reg, CompiledObject* target);
            void patch_jump(Label label, CompiledObject* target);

        private:
            Label _label_count = 0;
            std::map<Label, int> _patch_locations;
    };
}

#endif // KORE_BYTECODE_ARRAY_WRITER_HPP
