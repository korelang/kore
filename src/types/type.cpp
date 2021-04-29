#include "bool_type.hpp"
#include "char_type.hpp"
#include "integer_type.hpp"
#include "str_type.hpp"
#include "type.hpp"
#include "unknown_type.hpp"

Type::Type() : Type(TypeCategory::Unknown) {}

Type::Type(TypeCategory type_category) : _type(type_category) {}

Type::Type(const Token& token) {}

Type::~Type() {}

Type* Type::from_token(const Token& token) {
    if (!token.is_type()) {
        throw std::runtime_error("Cannot create type from non-type token '%s'");
    }

    if (token.value() == "i32") {
        return new IntegerType(32);
    } else if (token.value() == "byte") {
        return new IntegerType(8);
    } else if (token.value() == "char") {
        return new CharType();
    } else if (token.value() == "str") {
        return new StrType();
    } else if (token.value() == "bool") {
        return new BoolType();
    }

    return new UnknownType();
}
