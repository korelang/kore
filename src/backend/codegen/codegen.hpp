#ifndef KORE_CODEGEN_HPP
#define KORE_CODEGEN_HPP

#include <ostream>

#include "frontend/ast/ast.hpp"
#include "frontend/ast/ast_visitor.hpp"
#include "backend/codegen/opcode_writer.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    template<typename Opcode>
    class Codegen : public AstVisitor {
        public:
            Codegen() {}

            ~Codegen() {}

            void set_writer(OpcodeWriter<Opcode>* writer) {
                this->_writer = writer;
            }

            void compile(const Ast& ast) {
                if (!_writer) {
                    throw std::runtime_error("Output writer not set for compilation");
                }

                emit_header();
                emit_version();

                for (auto const& statement : ast) {
                    statement->accept(this);
                }
            }

            virtual void emit_version() {}
            virtual void emit_header() {}
            virtual void emit(Opcode opcode) {
                UNUSED_PARAM(opcode);
            }

            /* void write(const std::string& value); */
            /* void write(Opcode opcode, int reg, ); */

        protected:
            OpcodeWriter<Opcode>* _writer;
    };
}

#endif // KORE_CODEGEN_HPP
