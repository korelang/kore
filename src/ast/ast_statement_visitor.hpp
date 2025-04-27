#ifndef KORE_AST_VISITOR_HPP
#define KORE_AST_VISITOR_HPP

namespace kore {
    enum class ValueContext;

    class Branch;
    class Function;
    class Call;
    class IfStatement;
    class ImportStatement;
    class ModuleStatement;
    class Return;
    class VariableAssignment;
    class VariableDeclaration;
    class ExpressionStatement;

    class AstStatementVisitor {
        public:
            AstStatementVisitor();
            virtual ~AstStatementVisitor();

            virtual void visit(Branch& statement);
            virtual void visit(Function& statement);
            virtual void visit(class Call& statement);
            virtual void visit(IfStatement& statement);
            virtual void visit(ImportStatement& statement);
            virtual void visit(ModuleStatement& statement);
            virtual void visit(Return& statement);
            virtual void visit(VariableAssignment& statement);
            virtual void visit(VariableDeclaration& statement);
            virtual void visit(ExpressionStatement& exprstmt);
    };
}

#endif // KORE_AST_VISITOR_HPP
