#ifndef KORE_VOID_TYPE_HPP
#define KORE_VOID_TYPE_HPP

#include "types/type.hpp"

namespace kore {
    class Type;

    class VoidType : public Type {
        public:
            VoidType();
            virtual ~VoidType();

            std::string name() const override;
            const Type* unify(const Type* other_type) const override;
            const Type* unify(const VoidType* void_type) const override;
            void write(AstWriter* const writer) const override;
    };
}

#endif // KORE_VOID_TYPE_HPP
