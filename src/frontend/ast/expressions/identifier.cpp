#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "ast/expressions/identifier.hpp"
#include "string-utils.hpp"

namespace kore {
    Identifier::Identifier(const Token& token)
        : Expression(ExpressionType::Identifier, token.location()),
        _value(token.value()) {
        split_on(token.value(), '.', _parts);
    }

    Identifier::Identifier(const std::string& value)
        : Expression(ExpressionType::Identifier, Location::unknown),
        _value(value) {
        split_on(value, '.', _parts);
    }

    Identifier::Identifier(const std::string& value, const Location& location)
        : Expression(ExpressionType::Identifier, location),
        _value(value) {
        split_on(value, '.', _parts);
    }

    Identifier::Identifier(const std::vector<std::string>& parts, const Location& location)
        : Expression(ExpressionType::Identifier, location),
        _value(""),
        _parts(parts) {
    }

    Identifier::~Identifier() {}

    std::string Identifier::name() const {
        if (size()) {
            return _parts.back();
        } else if (!_value.empty()) {
            return _value;
        }

        return "<missing>";
    }

    std::string Identifier::qualified_name() const {
        return join_on(_parts, ".");
    }

    bool Identifier::is_qualified() const noexcept {
        return size() > 1;
    }

    std::size_t Identifier::size() const noexcept {
        return _parts.size();
    }

    const Type* Identifier::type() const {
        return _type;
    }

    void Identifier::accept(AstVisitor& visitor) {
        visitor.visit(*this);
    }

    void Identifier::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }

    Identifier::Identifier(const Token& token, Type* type)
        : Expression(ExpressionType::Parameter, token.location()),
          _value(token.value()) {
        set_type(type);
    }
}
