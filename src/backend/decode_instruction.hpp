#ifndef KORE_DECODE_INSTRUCTION_HPP
#define KORE_DECODE_INSTRUCTION_HPP

#include "codegen/bytecode/bytecode_codegen.hpp"
#include "codegen/compiled_object.hpp"

namespace kore {
    void decode_instructions(const CompiledObject* const compiled_code);
}

#endif // KORE_DECODE_INSTRUCTION_HPP
