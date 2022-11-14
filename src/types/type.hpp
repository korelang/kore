#ifndef KORE_TYPE_HPP
#define KORE_TYPE_HPP

#include "pointer_types.hpp"
#include "token.hpp"

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

    // Use this approach or make subclasses?
    enum class TypeCategory {
        Array,
        Bool,
        Char,
        Float32,
        Float64,
        Function,
        Integer32,
        Integer64,
        Map,
        Optional,
        Str,
        Struct,
        Unknown,
        Void,
    };

    struct TypeDeleter;

    /// Represents the different types
    class Type {
        public:
            using pointer = Owned<const Type, TypeDeleter>;

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
            TypeCategory category() const noexcept;
            virtual const Type* unify(const Type* other_type) const;
            virtual const Type* unify(const IntegerType* int_type) const;
            virtual const Type* unify(const FloatType* int_type) const;
            virtual const Type* unify(const CharType* str_type) const;
            virtual const Type* unify(const StrType* str_type) const;
            virtual const Type* unify(const BoolType* bool_type) const;
            virtual const Type* unify(const Optional* optional) const;
            virtual const Type* unify(const ArrayType* array_type) const;
            virtual const Type* unify(const FunctionType* func_type) const;

            virtual void write(AstWriter* const writer) const = 0;

            static const UnknownType* unknown();
            static Type* from_token(const Token& token);

        protected:
            Type(TypeCategory type_type);

        private:
            TypeCategory _category;
            /* bool _immutable = false; */
            static const UnknownType* _unknown_type;
    };

    // Custom deleter for unique_ptr of types that only deletes
    // non-shared complex types so simple types (i32, bool
    struct TypeDeleter {
        TypeDeleter() {}

        void operator()(Type* type) const {
            if (!type->is_simple()) {
                delete type;
            }
        }

        void operator()(const Type* type) const {
            if (!type->is_simple()) {
                delete type;
            }
        }
    };

}

#endif // KORE_TYPE_HPP
