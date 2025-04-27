#include "ast/ast_stream_writer.hpp"
#include "ast/expressions/expressions.hpp"
#include "ast/statements/statements.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    int AstStreamWriter::indent_in_spaces = 4;

    AstStreamWriter::AstStreamWriter() : _out(nullptr) {}

    AstStreamWriter::AstStreamWriter(std::ostream& out) {
        _out = &out;
    }

    AstStreamWriter::~AstStreamWriter() {
        // The class does not own the output stream
        _out = nullptr;
    }

    void AstStreamWriter::visit(ArrayExpression& expr) {
        write("[");

        for (int i = 0; i < expr.size(); ++i) {
            expr[i]->accept(*this);
        }

        write("]");
    }

    void AstStreamWriter::visit(ArrayFillExpression& fill) {
        write("[");
        fill.size_expr()->accept(*this);
        write(": ");
        fill.expr()->accept(*this);
        write("]");
    }

    void AstStreamWriter::visit(ArrayRangeExpression& expr) {
        expr.start_expr()->accept(*this);
        write(" .. ");
        expr.end_expr()->accept(*this);
    }

    void AstStreamWriter::visit(BinaryExpression& expr) {
        expr.left()->accept(*this);
        write(" " + expr.op_string() + " ");
        expr.right()->accept(*this);
    }

    void AstStreamWriter::visit(BoolExpression& expr) {
        write(expr.value());
    }

    void AstStreamWriter::visit(CharExpression& expr) {
        write(std::to_string(expr.value()));
    }

    void AstStreamWriter::visit(FloatExpression& expr) {
        write(std::to_string(expr.value()));
    }

    void AstStreamWriter::visit(Identifier& expr) {
        write(expr.qualified_name());
    }

    void AstStreamWriter::visit(Parameter& expr) {
        write(expr.name());
    }

    void AstStreamWriter::visit(IntegerExpression& expr) {
        write(std::to_string(expr.value()));
    }

    void AstStreamWriter::visit(StringExpression& expr) {
        write(expr.value());
    }

    void AstStreamWriter::visit(UnaryExpression& expr) {
        write(expr.op());
        expr.expr()->accept(*this);
    }

    void AstStreamWriter::visit(Branch& branch) {
        // Only "if" and "else if" branches have conditions
        if (branch.condition()) {
            branch.condition()->accept(*this);
        }

        write("{\n");

        auto it = branch.begin();
        
        for (; it != branch.end(); ++it) {
            (*it)->accept(*this);
        }

        write("}\n");
    }

    void AstStreamWriter::visit(Function& func) {
        if (func.exported()) {
            write("exported ");
        }

        write("func " + func.name() + "(");

        // TODO: parameters

        write(") {");
        newline();
        indent();

        // Function body
        for (auto it = func.begin(); it != func.end(); ++it) {
            write_indent();
            (*it)->accept(*this);
        }

        dedent();
        write("}");
        newline(2);
    }

    void AstStreamWriter::visit(class Call& call) {
        write(call.name() + "(");

        int arg_count = call.arg_count();

        for (int i = 0; i < arg_count; ++i) {
            call.arg(i)->accept(*this);

            if (i < arg_count - 1) {
                write(", ");
            }
        }

        write(")");
    }

    void AstStreamWriter::visit(IfStatement& ifstatement) {
        write("if ");
        ifstatement[0]->accept(*this);

        for (int i = 1; i < ifstatement.branch_count() - 1; ++i) {
            write("else if ");
            indent();
            ifstatement[i]->accept(*this);
            dedent();
        }

        if (ifstatement.has_else_branch()) {
            write("else ");
            indent();
            ifstatement.last_branch()->accept(*this);
            dedent();
        }
    }

    void AstStreamWriter::visit(ImportStatement& import) {
        write("import ");
        import.identifier()->accept(*this);
        newline();
    }

    void AstStreamWriter::visit(ModuleStatement& module) {
        write("module " + module.name());
        newline(2);
    }

    void AstStreamWriter::visit(Return& ret) {
        write("return");

        if (ret.expr_count() > 0) {
            write(" ");

            for (auto& expr : ret) {
                expr->accept(*this);
            }
        }

        newline();
    }

    void AstStreamWriter::visit(VariableAssignment& assignment) {
        UNUSED_PARAM(assignment);
        // TODO:
        /* if (assignment.is_mutable()) { */
        /*     write("var "); */
        /* } */

        /* /1* auto lhs = assignment.lhs(); *1/ */

        /* /1* write(lol); *1/ */
        /* write(" "); */
        /* write(assignment.declared_type()->name()); */
        /* write(" = "); */
        /* assignment.rhs()->accept(*this); */
        /* newline(); */
    }

    void AstStreamWriter::visit(VariableDeclaration& decl) {
        write(decl.identifier());

        /* if (decl.type()) { */
        /*     write(" " + decl.type()->name()); */
        /* } */

        newline();
    }

    void AstStreamWriter::visit(ExpressionStatement& exprstmt) {
        auto expr = exprstmt.expression();

        if (expr) {
            expr->accept(*this);
        }
    }

    void AstStreamWriter::write(const std::string& value) {
        if (_out) {
            *_out << value;
        }
    }

    void AstStreamWriter::write(const Ast& ast) {
        if (_out) {
            ast.write(*this);
            *_out << std::endl;
        }
    }

    void AstStreamWriter::write_indent() {
        if (_out) {
            *_out << std::string(_indent * indent_in_spaces, ' ');
        }
    }

    void AstStreamWriter::newline(int count) {
        if (_out) {
            for (int i = 0; i < count; ++i) {
                *_out << std::endl;
            }
        }
    }
}
