#ifndef KORE_IDENTIFIER_HPP
#define KORE_IDENTIFIER_HPP

#include <vector>

#include "ast/expressions/expression.hpp"

namespace kore {
    class Identifier : public Expression {
        public:
            Identifier(const Token& token);
            Identifier(const Token& token, bool is_mutable);
            Identifier(const std::string& value);
            Identifier(const std::string& value, const SourceLocation& location);
            Identifier(const std::vector<std::string>& value, const SourceLocation& location);
            virtual ~Identifier();

            std::string name() const;
            std::string qualified_name() const;
            bool is_qualified() const noexcept;
            std::size_t size() const noexcept;
            const Type* type() const override;
            bool is_mutable() const;

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        protected:
            // Used by function parameters that get constructed with a
            // type and a different ExpressionType
            Identifier(const Token& token, Type* type);

        private:
            std::string _value;
            bool _is_mutable = false;
            std::vector<std::string> _parts;
    };
}

#endif // KORE_IDENTIFIER_HPP
