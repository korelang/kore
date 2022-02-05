#ifndef KORE_BOOL_TYPE_HPP
#define KORE_BOOL_TYPE_HPP

#include "types/type.hpp"

namespace kore {
    class BoolType : public Type {
        public:
            BoolType();
            virtual ~BoolType();

            std::string name() const override;
            const Type* unify(const Type* other_type) const override;
            const Type* unify(const BoolType* bool_type) const override;

            void write(AstWriter* const writer) override;
    };
}

#endif // KORE_BOOL_TYPE_HPP
