#ifndef KORE_OPTIONAL_HPP
#define KORE_OPTIONAL_HPP

#include "types/type.hpp"

namespace kore {
    /// Represents an optional type that may or may not be present
    class Optional : public Type {
        public:
            Optional(const Type* contained_type);
            virtual ~Optional();

            std::string name() const override;
            const Type* unify(const Type* other_type) const override;
            const Type* unify(const Optional* optional) const override;
            void write(AstWriter* const writer) const override;

        private:
            const Type* _contained_type;
    };
}

#endif // KORE_OPTIONAL_HPP
