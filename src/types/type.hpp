#ifndef KORE_TYPE_HPP
#define KORE_TYPE_HPP

#include "pointer_types.hpp"
#include "ast/scanner/token.hpp"
#include "type_cache.hpp"
#include "type_category.hpp"

namespace kore {
    class AstWriter;
    class ArrayType;
    class BoolType;
    class CharType;
    class FloatType;
    class FunctionType;
    class IntegerType;
    class Optional;
    class StrType;
    class UnknownType;
    class VoidType;

    /// Represents the different types
    class Type {
        public:
            Type();
            virtual ~Type();

            virtual std::string name() const = 0;
            void set_optional(bool value);
            bool is_optional() const noexcept;
            bool is_numeric() const noexcept;
            bool is_unknown() const noexcept;
            bool is_simple() const noexcept;
            bool is_function() const noexcept;
            bool is_void() const noexcept;
            bool is_value_type() const noexcept;
            TypeCategory category() const noexcept;
            virtual const Type* unify(const Type* other_type) const;
            virtual const Type* unify(const IntegerType* int_type) const;
            virtual const Type* unify(const FloatType* int_type) const;
            virtual const Type* unify(const CharType* char_type) const;
            virtual const Type* unify(const StrType* str_type) const;
            virtual const Type* unify(const BoolType* bool_type) const;
            virtual const Type* unify(const Optional* optional) const;
            virtual const Type* unify(const ArrayType* array_type) const;
            virtual const Type* unify(const FunctionType* func_type) const;
            virtual const Type* unify(const VoidType* void_type) const;

            virtual void write(AstWriter* const writer) const = 0;

            static const UnknownType* unknown();
            static const VoidType* void_type();
            static ArrayType* make_array_type(const Type* element_type);
            static const Optional* make_optional_type(const Type* contained_type);
            static const FunctionType* make_function_type(
                const std::vector<const Type*>& parameter_types,
                const std::vector<const Type*>& return_types
            );
            static const Type* get_type_from_category(TypeCategory category);
            static const Type* from_token(const Token& token);

            template<typename T>
            const T* as() const {
                return static_cast<const T*>(this);
            }

        protected:
            Type(TypeCategory type_type);

        private:
            TypeCategory _category;
            /* bool _inferred = false; */

            // Static methods for getting common types
            static TypeCache _type_cache;
    };
}

#endif // KORE_TYPE_HPP
