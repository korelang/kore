#include "bool_type.hpp"
#include "char_type.hpp"
#include "integer_type.hpp"
#include "str_type.hpp"
#include "type.hpp"
#include "unknown_type.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    Type::Type() : Type(TypeCategory::Unknown) {}

    Type::Type(TypeCategory type_category) : _category(type_category) {}

    Type::~Type() {}

    void Type::set_optional(bool value) {
        UNUSED_PARAM(value);
    }

    bool Type::is_optional() const noexcept {
        return _category == TypeCategory::Optional;
    }

    bool Type::is_numeric() const noexcept {
        switch (_category) {
            case TypeCategory::Float:
            case TypeCategory::Integer:
                return true;

            default:
                return false;
        }
    }

    bool Type::is_unknown() const noexcept {
        return _category == TypeCategory::Unknown;
    }

    bool Type::is_simple() const noexcept {
        // TODO: Handle arrays/optionals by checking if their element types are
        // simple as well
        switch (_category) {
            case TypeCategory::Bool:
            case TypeCategory::Char:
            case TypeCategory::Float:
            case TypeCategory::Integer:
            case TypeCategory::Str:
            case TypeCategory::Unknown:
            case TypeCategory::Void:
                return true;

            default:
                return false;
        }
    }

    bool Type::is_function() const noexcept {
        return category() == TypeCategory::Function;
    }

    TypeCategory Type::category() const noexcept {
        return _category;
    }

    const Type* Type::unify(const ArrayType* array_type) const {
        UNUSED_PARAM(array_type);
        
        return Type::unknown();
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

    const Type* Type::unify(const FunctionType* func_type) const {
        UNUSED_PARAM(func_type);

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
}
