#ifndef KORE_TYPE_HPP
#define KORE_TYPE_HPP

#include "token.hpp"

class AstWriter;
class Integer32Type;
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
        Type(const Token& token);
        virtual ~Type();

        void set_optional(bool value);
        bool is_optional() const noexcept;

        virtual void write(AstWriter* const writer) = 0;

        static Type* from_token(const Token& token);

    protected:
        Type(TypeCategory type_type);

    private:
        TypeCategory _type;
};

#endif // KORE_TYPE_HPP
