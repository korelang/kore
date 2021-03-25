#include "ast/ast_writer.hpp"
#include "ast/expressions/identifier.hpp"
#include "string-utils.hpp"

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
    return *_parts.end();
}

std::string Identifier::qualified_name() const {
    return "";
}

std::size_t Identifier::size() const noexcept {
    return _parts.size();
}

void Identifier::write(AstWriter* const writer) {
    writer->write("identifier<");

    for (const auto& part : _parts) {
        writer->write(part);
        writer->write(".");
    }

    writer->write(">");
}
