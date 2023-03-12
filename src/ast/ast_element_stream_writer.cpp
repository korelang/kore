#include "ast/ast_element_stream_writer.hpp"
#include "logging/colors.hpp"
#include "logging/color_attributes.hpp"

namespace kore {
    int AstElementStreamWriter::indent_in_spaces = 2;

    AstElementStreamWriter::AstElementStreamWriter() : _out(nullptr) {}

    AstElementStreamWriter::AstElementStreamWriter(std::ostream& out) {
        _out = &out;
    }

    AstElementStreamWriter::~AstElementStreamWriter() {
        // The class does not own the output stream
        _out = nullptr;
    }

    void AstElementStreamWriter::visit(ArrayExpression& expr) {
        write("[");

        for (int i = 0; i < expr.size(); ++i) {
            expr[i]->accept_visit_only(*this);
        }

        write("]");
    }

    void AstElementStreamWriter::visit(ArrayFillExpression& fill) {
        write_node_name("array fill");
        indent();
        write_indent();
        fill.size_expr()->accept_visit_only(*this);
        fill.expr()->accept_visit_only(*this);
        dedent();
    }

    void AstElementStreamWriter::visit(ArrayRangeExpression& expr) {
        write_node_name("array range");
        indent();
        write_indent();
        expr.start_expr()->accept_visit_only(*this);
        expr.end_expr()->accept_visit_only(*this);
        dedent();
    }

    void AstElementStreamWriter::visit(BinaryExpression& expr) {
        write_node_name("+");
        write_location(expr);

        newline();
        indent();
        write_indent();
        expr.left()->accept_visit_only(*this);
        write_indent();
        expr.right()->accept_visit_only(*this);
        dedent();
    }

    void AstElementStreamWriter::visit(BoolExpression& expr) {
        write_node_name("bool ");
        write(expr.value());
        write_location(expr);
    }

    void AstElementStreamWriter::visit(CharExpression& expr) {
        write_node_name("bool ");
        write(std::to_string(expr.value()));
        write_location(expr);
    }

    void AstElementStreamWriter::visit(FloatExpression& expr) {
        write_node_name("f32 ");
        write(std::to_string(expr.value()));
        write_location(expr);
    }

    void AstElementStreamWriter::visit(Identifier& expr) {
        write_node_name("identifier ");
        write(expr.qualified_name());
        write_location(expr);
        newline();
    }

    void AstElementStreamWriter::visit(Parameter& expr) {
        write(expr.name());
        write_location(expr);
    }

    void AstElementStreamWriter::visit(IntegerExpression& expr) {
        write_node_name("i32 ");
        write(std::to_string(expr.value()));
        write_location(expr);
    }

    void AstElementStreamWriter::visit(StringExpression& expr) {
        write_node_name("str ");
        write(expr.value());
        write_location(expr);
    }

    void AstElementStreamWriter::visit(UnaryExpression& expr) {
        write_node_name("unary " + expr.op());
        write_location(expr);

        newline();
        indent();
        write_indent();
        expr.expr()->accept_visit_only(*this);
        dedent();
    }

    void AstElementStreamWriter::visit(Branch& branch) {
        // Only "if" and "else if" branches have conditions
        if (branch.condition()) {
            branch.condition()->accept_visit_only(*this);
        }

        auto it = branch.begin();
        
        for (; it != branch.end(); ++it) {
            (*it)->accept_visit_only(*this);
        }
    }

    void AstElementStreamWriter::visit(Function& func) {
        write_node_name("func ");

        if (func.exported()) {
            write("exported ");
        }

        write(func.name());
        write_location(func);
        newline();
        indent();

        // Function body
        for (auto it = func.begin(); it != func.end(); ++it) {
            write_indent();
            (*it)->accept_visit_only(*this);
        }

        dedent();
    }

    void AstElementStreamWriter::visit(class Call& call) {
        write_node_name("call ");
        write(std::to_string(call.arg_count()) + " " + call.name());
        write_location(call);

        int arg_count = call.arg_count();
        indent();

        for (int i = 0; i < arg_count; ++i) {
            newline();
            write_indent();
            call.arg(i)->accept_visit_only(*this);
        }

        dedent();
    }

    void AstElementStreamWriter::visit(IfStatement& ifstatement) {
        write("if " + ifstatement.location().colon_format());
        ifstatement[0]->accept_visit_only(*this);

        for (int i = 1; i < ifstatement.branch_count() - 1; ++i) {
            write("else if " + ifstatement[i]->location().colon_format());
            indent();
            ifstatement[i]->accept_visit_only(*this);
            dedent();
        }

        if (ifstatement.has_else_branch()) {
            write("else " + ifstatement.last_branch()->location().colon_format());
            indent();
            ifstatement.last_branch()->accept_visit_only(*this);
            dedent();
        }
    }

    void AstElementStreamWriter::visit(ImportStatement& import) {
        write("import " + import.identifier()->name() + " " + import.location().colon_format());
        import.identifier()->accept_visit_only(*this);
        newline();
    }

    void AstElementStreamWriter::visit(ModuleStatement& module) {
        write_node_name("module ");
        write(module.name());
        write_location(module);
        newline(2);
    }

    void AstElementStreamWriter::visit(Return& ret) {
        write_node_name("return");
        write_location(ret);

        if (ret.expr()) {
            newline();
            indent();
            write_indent();
            ret.expr()->accept_visit_only(*this);
            dedent();
        }

        newline();
    }

    void AstElementStreamWriter::visit(VariableAssignment& assignment) {
        write_node_name("assignment ");

        if (assignment.identifier()->is_mutable()) {
            write("var ");
        }

        write(assignment.identifier()->name());
        write(" ");
        write(assignment.declared_type()->name());
        write_location(assignment);

        newline();
        indent();
        write_indent();
        assignment.expression()->accept_visit_only(*this);
        dedent();
        newline();
    }

    void AstElementStreamWriter::visit(VariableDeclaration& decl) {
        write(decl.identifier());

        /* if (decl.type()) { */
        /*     write(" " + decl.type()->name()); */
        /* } */

        newline();
    }

    void AstElementStreamWriter::visit(ExpressionStatement& exprstmt) {
        auto expr = exprstmt.expression();

        if (expr) {
            expr->accept_visit_only(*this);
        }
    }

    void AstElementStreamWriter::write(const std::string& value) {
        if (_out) {
            *_out << value;
        }
    }

    void AstElementStreamWriter::write(const Ast& ast) {
        if (_out) {
            ast.write(*this);
            *_out << std::endl;
        }
    }

    void AstElementStreamWriter::write_indent() {
        if (_out) {
            *_out << std::string(_indent * indent_in_spaces, ' ');
        }
    }

    void AstElementStreamWriter::newline(int count) {
        if (_out) {
            for (int i = 0; i < count; ++i) {
                *_out << std::endl;
            }
        }
    }

    void AstElementStreamWriter::write_node_name(const std::string& node_name) {
        if (_out) {
            *_out << Color::Yellow << node_name << ColorAttribute::Reset;
        }
    }

    void AstElementStreamWriter::write_location(const AstNode& node) {
        auto location = node.location();

        if (location.is_unknown()) {
            write(" (unknown location)");
        } else {
            write(" (" + node.location().colon_format() + ")");
        }
    }
}
