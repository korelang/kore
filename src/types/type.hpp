#ifndef KORE_TYPE_HPP
#define KORE_TYPE_HPP

#include "token.hpp"

class AstWriter;
class BoolType;
class CharType;
class Integer32Type;
class IntegerType;
class Optional;
class StrType;
class UnknownType;

// Use this approach or make subclasses?
enum class TypeCategory {
    Array,
    Bool,
    Char,
    Float,
    Function,
    Integer,
    Map,
    Optional,
    Str,
    Struct,
    Unknown,
    Void,
};

/// Represents the different types
class Type {
    public:
        Type();
        virtual ~Type();

        virtual std::string name() const = 0;
        void set_optional(bool value);
        bool is_optional() const noexcept;
        bool is_numeric() const noexcept;
        TypeCategory category() const noexcept;
        virtual const Type* unify(const Type* other_type) const;
        virtual const Type* unify(const IntegerType* int_type) const;
        virtual const Type* unify(const CharType* str_type) const;
        virtual const Type* unify(const StrType* str_type) const;
        virtual const Type* unify(const BoolType* bool_type) const;
        virtual const Type* unify(const Optional* optional) const;

        virtual void write(AstWriter* const writer) = 0;

        static const UnknownType* unknown();
        static Type* from_token(const Token& token);

    protected:
        Type(TypeCategory type_type);

    private:
        TypeCategory _category;
        static const UnknownType* _unknown_type;
};

#endif // KORE_TYPE_HPP
