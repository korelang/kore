#ifndef KORE_CHAR_TYPE_HPP
#define KORE_CHAR_TYPE_HPP

#include "types/type.hpp"

namespace kore {
    class CharType : public Type {
        public:
            CharType();
            virtual ~CharType();
            
            std::string name() const override;
            const Type* unify(const Type* other_type) const override;
            const Type* unify(const CharType* str_type) const override;
            void write(AstWriter* const writer) const override;
    };
}

#endif // KORE_CHAR_TYPE_HPP
