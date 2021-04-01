#include "ast/ast_writer.hpp"
#include "ast/expressions/identifier.hpp"
#include "string-utils.hpp"

Identifier::Identifier(const Token& token)
    : Expression(ExpressionType::identifier, token.location()),
      _value(token.value()) {
    split_on(token.value(), '.', _parts);
}

Identifier::Identifier(const std::string& value, const Location& location)
    : Expression(ExpressionType::identifier, location),
      _value(value) {
    split_on(value, '.', _parts);
}

Identifier::Identifier(const std::vector<std::string>& parts, const Location& location)
    : Expression(ExpressionType::identifier, location),
      _value(""),
      _parts(parts) {
}

Identifier::~Identifier() {}

std::string Identifier::name() const {
    if (size()) {
        return _parts.back();
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

void Identifier::write(AstWriter* const writer) {
    writer->write("identifier<");
    writer->write(qualified_name());
    writer->write(">");
}
