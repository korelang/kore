#include "bool_type.hpp"
#include "char_type.hpp"
#include "integer_type.hpp"
#include "str_type.hpp"
#include "type.hpp"
#include "unknown_type.hpp"
#include "utils/unused_parameter.hpp"

Type::Type() : Type(TypeCategory::Unknown) {}

Type::Type(TypeCategory type_category) : _category(type_category) {}

Type::~Type() {}

bool Type::is_numeric() const noexcept {
    switch (_category) {
        case TypeCategory::Float:
        case TypeCategory::Integer:
            return true;

        default:
            return false;
    }
}

TypeCategory Type::category() const noexcept {
    return _category;
}

const Type* Type::unify(const Type* other_type) const {
    UNUSED_PARAM(other_type);
    
    return Type::unknown();
}

const Type* Type::unify(const IntegerType* int_type) const {
    UNUSED_PARAM(int_type);

    return Type::unknown();
}

const Type* Type::unify(const CharType* char_type) const {
    UNUSED_PARAM(char_type);

    return Type::unknown();
}

const Type* Type::unify(const StrType* str_type) const {
    UNUSED_PARAM(str_type);

    return Type::unknown();
}

const Type* Type::unify(const BoolType* bool_type) const {
    UNUSED_PARAM(bool_type);

    return Type::unknown();
}

const Type* Type::unify(const Optional* optional) const {
    UNUSED_PARAM(optional);

    return Type::unknown();
}

const UnknownType* Type::_unknown_type = new UnknownType();

const UnknownType* Type::unknown() {
    return UnknownType::_unknown_type;
}

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
