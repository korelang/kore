#ifndef KORE_AST_ELEMENT_STREAM_WRITER_HPP
#define KORE_AST_ELEMENT_STREAM_WRITER_HPP

#include <ostream>

#include "ast/ast_writer.hpp"

namespace kore {
    /// Writes the elements of an abstract syntax tree to an output stream
    class AstElementStreamWriter : public AstWriter {
        public:
            AstElementStreamWriter();
            AstElementStreamWriter(std::ostream& out);
            virtual ~AstElementStreamWriter();

            void visit(ArrayExpression& expr) override;
            void visit(ArrayFillExpression& expr) override;
            void visit(ArrayRangeExpression& expr) override;
            void visit(BinaryExpression& expr) override;
            void visit(BoolExpression& expr) override;
            void visit(CharExpression& expr) override;
            void visit(FloatExpression& expr) override;
            void visit(Identifier& expr) override;
            void visit(Parameter& expr) override;
            void visit(IntegerExpression& expr) override;
            void visit(StringExpression& expr) override;
            void visit(UnaryExpression& expr) override;

            void visit(Branch& statement) override;
            void visit(Function& statement) override;
            void visit(class Call& statement) override;
            void visit(IfStatement& statement) override;
            void visit(ImportStatement& statement) override;
            void visit(ModuleStatement& statement) override;
            void visit(Return& statement) override;
            void visit(VariableAssignment& statement) override;
            void visit(VariableDeclaration& statement) override;
            void visit(ExpressionStatement& exprstmt) override;

            void write(const std::string& value) override;
            void write(const Ast& ast) override;
            void write_indent();
            void newline(int count = 1) override;

        private:
            static int indent_in_spaces;
            std::ostream* _out;

            void write_node_name(const std::string& node_name);
            void write_location(const AstNode& node);
    };
}

#endif // KORE_AST_ELEMENT_STREAM_WRITER_HPP
