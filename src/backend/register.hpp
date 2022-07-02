#ifndef KORE_REGISTER_HPP
#define KORE_REGISTER_HPP

namespace kore {
    // Type alias for registers ('register' is a reserved keyword)
    using Reg = int;

    // Handy macros for getting different registers in an instruction
    #define GET_REG1(instruction) instruction >> 16 & 0xff
    #define GET_REG2(instruction) instruction >> 8 & 0xff
    #define GET_REG3(instruction) instruction & 0xff
    #define GET_REG(instruction, pos) (instruction >> ((3 - pos) * 8)) & 0xff
    #define GET_OFFSET(instruction) instruction & 0xffff

    #define GET_VALUE32(instruction) instruction & 0xffff
}

#endif // KORE_REGISTER_HPP
