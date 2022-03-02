#include <iostream>

#include "codegen/bytecode/bytecode_dumper.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    BytecodeDumper::BytecodeDumper() : BytecodeDumper(std::cout) {}

    BytecodeDumper::BytecodeDumper(std::ostream& out) : _out(&out) {}

    BytecodeDumper::~BytecodeDumper() {}

    void BytecodeDumper::write_header() {
        *_out << "Byte position" << "\t" << "Bytecode" << "\t" << "Operands" << std::endl;
    }

    void BytecodeDumper::write(Bytecode bytecode) {
        switch (bytecode) {
            case Bytecode::Noop:
                *_out << _pos << "\t" << bytecode_to_string(bytecode) << std::endl;
                _pos += sizeof(bytecode);
                break;

            default:
                *_out << "nope" << std::endl;
                break;
        }
    }

    void BytecodeDumper::write(Bytecode bytecode, i32 value) {
        switch (bytecode) {
            case Bytecode::StoreI32Global:
                *_out << _pos << "\t"
                      << bytecode_to_string(bytecode) << " "
                      << value << std::endl;
                _pos += sizeof(bytecode) + sizeof(i32);
                break;

            default:
                *_out << "nope" << std::endl;
                break;
        }
    }

    void BytecodeDumper::write(Bytecode bytecode, int reg, i32 value) {
        switch (bytecode) {
            case Bytecode::LoadI32:
                *_out << _pos << "\t"
                      << bytecode_to_string(bytecode) << " "
                      << "r" << reg << " "
                      << value << std::endl;
                _pos += sizeof(bytecode) + sizeof(i32);
                break;

            default:
                *_out << "nope" << std::endl;
                break;
        }
    }

    void BytecodeDumper::write(Bytecode bytecode, int destination_reg, int reg_operand1, int reg_operand2) {
        switch (bytecode) {
            case Bytecode::AddI32:
                *_out << _pos << "\t"
                      << bytecode_to_string(bytecode) << " "
                      << "r" << destination_reg << " "
                      << "r" << reg_operand1 << " "
                      << "r" << reg_operand2 << std::endl;
                _pos += sizeof(bytecode) + sizeof(i32) * 3;
                break;

            default:
                *_out << "nope" << std::endl;
                break;
        }
    }

    /* void BytecodeDumper::write(Bytecode bytecode, bool value) { */
    /*     switch (bytecode) { */
    /*         case Bytecode::ConstantBool: */
    /*             *_out << _pos << "\t\t" */
    /*                   << bytecode_to_string(bytecode) << "\t" */
    /*                   << value << std::endl; */
    /*             _pos += sizeof(bytecode) + sizeof(bool); */
    /*             break; */

    /*         default: */
    /*             *_out << "nope" << std::endl; */
    /*             break; */
    /*     } */
    /* } */

    /* void BytecodeDumper::write(const char* const bytes, std::size_t count) { */
    /*     UNUSED_PARAM(bytes); */
    /*     UNUSED_PARAM(count); */

    /*     return; */
    /* } */

    bool BytecodeDumper::is_dumper() const noexcept {
        return true;
    }
}
