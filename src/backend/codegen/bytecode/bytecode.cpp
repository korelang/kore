#include "backend/codegen/bytecode/bytecode.hpp"

namespace kore {
    std::string bytecode_to_string(Bytecode bytecode) {
        switch (bytecode) {
            case Noop:           return "noop";
            case Move:           return "move";
            case LoadI32Global:  return "gloadi32";
            case StoreI32Global: return "gstorei32";
            case LoadBool:       return "loadbool";
            case LoadI32:        return "loadi32";
            case LoadI64:        return "loadi64";
            case LoadF32:        return "loadf32";
            case LoadF64:        return "loadf64";
            case LoadLocal:      return "load";
            case StoreLocal:     return "store";
            case AddI32:         return "addi32";
            case AddI64:         return "addi64";
            case AddF32:         return "addf32";
            case AddF64:         return "addf64";
            case Jump:           return "jump";
            case JumpIf:         return "jumpif";
            case JumpIfNot:      return "jumpifnot";
            case Call:           return "call";
            case Ret:            return "return";
        }
    }

    std::ostream& operator<<(std::ostream& os, const Bytecode bytecode) {
        return os << static_cast<bytecode_type>(bytecode);
    }
}
