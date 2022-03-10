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
            case SubI32:         return "subi32";
            case SubI64:         return "subi64";
            case SubF32:         return "subf32";
            case SubF64:         return "subf64";
            case MultI32:        return "multi32";
            case MultI64:        return "multi64";
            case MultF32:        return "multf32";
            case MultF64:        return "multf64";
            case PowI32:         return "powi32";
            case PowI64:         return "powi64";
            case PowF32:         return "powf32";
            case PowF64:         return "powf64";
            case DivI32:         return "divi32";
            case DivI64:         return "divi64";
            case DivF32:         return "divf32";
            case DivF64:         return "divf64";
            case LtI32:          return "lti32";
            case LtI64:          return "lti64";
            case LtF32:          return "ltf32";
            case LtF64:          return "ltf64";
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
