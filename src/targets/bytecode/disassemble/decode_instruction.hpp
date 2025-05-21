#ifndef KOREDIS_DECODE_INSTRUCTION_HPP
#define KOREDIS_DECODE_INSTRUCTION_HPP

#include "targets/bytecode/compiled_object.hpp"
#include "targets/bytecode/disassemble/instruction.hpp"
#include "targets/bytecode/module_load_error.hpp"

namespace koredis {
    struct DecodeResult {
        std::vector<Instruction> instructions;
        std::vector<kore::ModuleLoadError> errors;
    };

    Instruction decode_instruction(kore::bytecode_type instruction);

    std::vector<Instruction> decode_instructions(const kore::CompiledObject compiled_object);

    DecodeResult decode_compiled_object(const kore::CompiledObject* const compiled_code);

    /* DecodedModule decode_module(std::istream& is); */
}

#endif // KOREDIS_DECODE_INSTRUCTION_HPP
