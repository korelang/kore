#ifndef KORE_BYTECODE_DUMPER_HPP
#define KORE_BYTECODE_DUMPER_HPP

#include <ostream>

#include "codegen/bytecode/bytecode.hpp"
#include "codegen/opcode_writer.hpp"

namespace kore {
    class BytecodeDumper final : public OpcodeWriter<Bytecode> {
        public:
            BytecodeDumper();
            BytecodeDumper(std::ostream& out);
            virtual ~BytecodeDumper();
            void write_header() override;
            void write(Bytecode bytecode) override;
            void write(Bytecode bytecode, i32 value) override;
            void write(Bytecode bytecode, int reg, i32 value) override;
            void write(Bytecode bytecode, int destination_reg, int reg_operand1, int reg_operand2) override;
            /* void write(Bytecode bytecode, bool value) override; */
            bool is_dumper() const noexcept override;

        private:
            int _pos = 0;
            std::ostream* _out;
    };
}

#endif // KORE_BYTECODE_DUMPER_HPP
