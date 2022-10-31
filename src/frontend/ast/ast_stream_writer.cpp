#include "ast/ast_stream_writer.hpp"

namespace kore {
    AstStreamWriter::AstStreamWriter() : _out(nullptr) {}

    AstStreamWriter::AstStreamWriter(std::ostream& out) {
        _out = &out;
    }

    AstStreamWriter::~AstStreamWriter() {
        _out = nullptr;
    }

    void AstStreamWriter::visit(ArrayExpression& expr) {
        write("[");

        for (int i = 0; i < expr.size(); ++i) {
            expr[i]->accept_visit_only(*this);
        }

        write("]");
    }

    void AstStreamWriter::visit(ArrayFillExpression& fill) {
        write("[");
        fill.size_expr()->accept_visit_only(*this);
        write(": ");
        fill.expr()->accept_visit_only(*this);
        write("]");
    }

    void AstStreamWriter::visit(ArrayRangeExpression& expr) {
        expr.start_expr()->accept_visit_only(*this);
        write(" .. ");
        expr.end_expr()->accept_visit_only(*this);
    }

    void AstStreamWriter::visit(BinaryExpression& expr) {
        expr.left()->accept_visit_only(*this);
        write(" " + expr.op_string() + " ");
        expr.right()->accept_visit_only(*this);
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
        expr.expr()->accept_visit_only(*this);
    }

    void AstStreamWriter::visit(Branch& branch) {
        // Only "if" and "else if" branches have conditions
        if (branch.condition()) {
            branch.condition()->accept_visit_only(*this);
        }

        write("{\n");

        auto it = branch.begin();
        
        for (; it != branch.end(); ++it) {
            (*it)->accept_visit_only(*this);
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
            (*it)->accept_visit_only(*this);
        }

        dedent();
        write("}");
        newline();
    }

    void AstStreamWriter::visit(class Call& call) {
        write(call.name());
    }

    void AstStreamWriter::visit(IfStatement& ifstatement) {
        write("if ");
        ifstatement[0]->accept_visit_only(*this);

        for (int i = 1; i < ifstatement.branch_count() - 1; ++i) {
            write("else if ");
            indent();
            ifstatement[i]->accept_visit_only(*this);
            dedent();
        }

        if (ifstatement.has_else_branch()) {
            write("else ");
            indent();
            ifstatement.last_branch()->accept_visit_only(*this);
            dedent();
        }
    }

    void AstStreamWriter::visit(ImportStatement& import) {
        write("import ");
        import.identifier()->accept_visit_only(*this);
        newline();
    }

    void AstStreamWriter::visit(ModuleStatement& module) {
        write("module ");
        write(module.name());
        newline();
    }

    void AstStreamWriter::visit(Return& ret) {
        write("return");

        if (ret.expr()) {
            write(" ");
            ret.expr()->accept_visit_only(*this);
        }

        newline();
    }

    void AstStreamWriter::visit(VariableAssignment& assignment) {
        if (assignment.identifier()->is_mutable()) {
            write("var ");
        }

        write(assignment.identifier()->name());
        write(" ");
        write(assignment.declared_type()->name());
        write(" = ");
        assignment.expression()->accept_visit_only(*this);
        newline();
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
            expr->accept_visit_only(*this);
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

    void AstStreamWriter::newline() {
        if (_out) {
            *_out << std::endl;
        }
    }
}
