#ifndef KORE_OPTIONAL_HPP
#define KORE_OPTIONAL_HPP

#include "types/type.hpp"

/// Represents an optional type that may or may not be present
class Optional : public Type {
    public:
        Optional(Type* contained_type);
        virtual ~Optional();

        std::string name() const override;

        void write(AstWriter* const writer) override;

    private:
        std::unique_ptr<Type> _contained_type;
};

#endif // KORE_OPTIONAL_HPP
