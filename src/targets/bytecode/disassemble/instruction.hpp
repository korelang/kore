#ifndef KOREDIS_INSTRUCTION_HPP
#define KOREDIS_INSTRUCTION_HPP

#include <ostream>
#include <vector>

#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/codegen/kir/instruction.hpp"
#include "targets/bytecode/register.hpp"

namespace koredis {
    struct Instruction {
        int pos;
        int byte_pos;

        kore::kir::Instruction value;
    };

    std::ostream& format_registers(std::ostream& os, Instruction instruction);

    std::ostream& operator<<(std::ostream& os, const Instruction instruction);
}

#endif // KOREDIS_INSTRUCTION_HPP
