#ifndef KORE_BYTECODE_HPP
#define KORE_BYTECODE_HPP

#include <cstdint>
#include <ostream>

namespace kore {
    /// Bytecodes in kore are 32-bit integer with a
    /// 2- or 3-address layout:
    ///
    ///    8 bits     8 bits           16 bits
    /// +----------+-----------+-----------------------+
    /// |  opcode  | operand 1 |      operand 2        |
    /// +----------+-----------+-----------------------+
    ///
    ///    8 bits     8 bits      8 bits      8 bits
    /// +----------+-----------+-----------+-----------+
    /// |  opcode  | operand 1 | operand 2 | operand 3 |
    /// +----------+-----------+-----------+-----------+
    using bytecode_type = std::uint32_t;

    /// For example the following statement
    ///
    /// a = b + c
    ///
    /// where a, b, and c are 32-bit integers, a is stored
    /// in register 1 (r1), b in r2, / and c in r3, is compiled as
    ///
    /// addi32 r1 r2 r3

    enum Bytecode {
        Noop,
        Move,
        LoadI32Global,
        LoadBool,
        LoadI32,
        LoadI64,
        LoadF32,
        LoadF64,
        LoadLocal,
        StoreI32Global,
        StoreLocal,
        AddI32,
        AddI64,
        AddF32,
        AddF64,
        SubI32,
        SubI64,
        SubF32,
        SubF64,
        MultI32,
        MultI64,
        MultF32,
        MultF64,
        PowI32,
        PowI64,
        PowF32,
        PowF64,
        DivI32,
        DivI64,
        DivF32,
        DivF64,
        LtI32,
        LtI64,
        LtF32,
        LtF64,
        GtI32,
        GtI64,
        GtF32,
        GtF64,
        LeI32,
        LeI64,
        LeF32,
        LeF64,
        GeI32,
        GeI64,
        GeF32,
        GeF64,
        EqI32,
        EqI64,
        EqF32,
        EqF64,
        NeqI32,
        NeqI64,
        NeqF32,
        NeqF64,
        Jump,
        JumpIf,
        JumpIfNot,
        Call,
        Ret,
    };

    std::string bytecode_to_string(Bytecode bytecode);

    std::ostream& operator<<(std::ostream& os, const Bytecode bytecode);
}

#endif // KORE_BYTECODE_HPP
