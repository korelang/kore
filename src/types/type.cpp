#include "array_type.hpp"
#include "bool_type.hpp"
#include "char_type.hpp"
#include "integer_type.hpp"
#include "str_type.hpp"
#include "type.hpp"
#include "optional.hpp"
#include "float_type.hpp"
#include "unknown_type.hpp"
#include "void_type.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    Type::Type() : Type(TypeCategory::Unknown) {}

    Type::Type(TypeCategory type_category) : _category(type_category) {}

    Type::~Type() {}

    std::string Type::name() const {
        return unknown()->name();
    }

    void Type::set_optional(bool value) {
        UNUSED_PARAM(value);
    }

    bool Type::is_optional() const noexcept {
        return _category == TypeCategory::Optional;
    }

    bool Type::is_numeric() const noexcept {
        switch (_category) {
            case TypeCategory::Float32:
            case TypeCategory::Float64:
            case TypeCategory::Integer32:
            case TypeCategory::Integer64:
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
            case TypeCategory::Float32:
            case TypeCategory::Float64:
            case TypeCategory::Integer32:
            case TypeCategory::Integer64:
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

    bool Type::is_void() const noexcept {
        return category() == TypeCategory::Void;
    }

    bool Type::is_value_type() const noexcept {
        switch (category()) {
            case TypeCategory::Bool:
            case TypeCategory::Byte:
            case TypeCategory::Char:
            case TypeCategory::Float32:
            case TypeCategory::Float64:
            case TypeCategory::Integer32:
            case TypeCategory::Integer64:
            case TypeCategory::Unknown:
            case TypeCategory::Void: {
                return true;
            }

            default:
                return false;
        }
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

    const Type* Type::unify(const FloatType* float_type) const {
        UNUSED_PARAM(float_type);

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

    const Type* Type::unify(const VoidType* void_type) const {
        UNUSED_PARAM(void_type);

        return Type::unknown();
    }

    TypeCache Type::_type_cache;

    const UnknownType* Type::unknown() {
        return static_cast<const UnknownType*>(_type_cache.get_type(TypeCategory::Unknown));
    }

    const VoidType* Type::void_type() {
        return static_cast<const VoidType*>(_type_cache.get_type(TypeCategory::Void));
    }

    ArrayType* Type::make_array_type(const Type* element_type) {
        return _type_cache.get_array_type(element_type);
    }

    const FunctionType* Type::make_function_type(
        const std::vector<const Type*>& parameter_types,
        const std::vector<const Type*>& return_types
    ) {
        return _type_cache.get_function_type(parameter_types, return_types);
    }

    void Type::set_function_type(Owned<FunctionType>&& func_type) {
        _type_cache.set_function_type(std::move(func_type));
    }

    const Optional* Type::make_optional_type(const Type* contained_type) {
        return _type_cache.get_optional_type(contained_type);
    }

    const Type* Type::get_type_from_category(TypeCategory category) {
        return _type_cache.get_type(category);
    }

    const Type* Type::from_token(const Token& token) {
        if (!token.is_type()) {
            throw std::runtime_error("Cannot create type from non-type token '%s'");
        }

        if (token.value() == "i32") {
            return _type_cache.get_type(TypeCategory::Integer32);
        } else if (token.value() == "i64") {
            return _type_cache.get_type(TypeCategory::Integer64);
        } else if (token.value() == "f32") {
            return _type_cache.get_type(TypeCategory::Float32);
        } else if (token.value() == "f64") {
            return _type_cache.get_type(TypeCategory::Float64);
        } else if (token.value() == "byte") {
            return _type_cache.get_type(TypeCategory::Byte);
        } else if (token.value() == "char") {
            return _type_cache.get_type(TypeCategory::Char);
        } else if (token.value() == "str") {
            return _type_cache.get_type(TypeCategory::Str);
        } else if (token.value() == "bool") {
            return _type_cache.get_type(TypeCategory::Bool);
        } else if (token.value() == "void") {
            return Type::void_type();
        } else if (token.value() == "unknown") {
            return Type::unknown();
        }

        return Type::unknown();
    }
}
