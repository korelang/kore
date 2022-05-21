#ifndef KORE_UNKNOWN_TYPE_HPP
#define KORE_UNKNOWN_TYPE_HPP

#include "type.hpp"

namespace kore {
    /// An unknown type to be inferred by the typechecker
    class UnknownType : public Type {
        public:
            UnknownType();
            virtual ~UnknownType();

            std::string name() const override;
            void write(AstWriter* const writer) const override;
    };
}

#endif // KORE_UNKNOWN_TYPE_HPP
