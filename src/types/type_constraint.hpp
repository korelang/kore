#ifndef KORE_TYPE_CONSTRAINT_HPP
#define KORE_TYPE_CONSTRAINT_HPP

#include <variant>

namespace kore {
    class Type;

    struct EqualityConstraint {
        const Type* type1;
        const Type* type2;
    };
    
    /// A type constraint used for type inference
    using TypeConstraint = std::variant<EqualityConstraint>;

    std::ostream& operator<<(std::ostream& os, const TypeConstraint type_constraint);
}

#endif // KORE_TYPE_CONSTRAINT_HPP
