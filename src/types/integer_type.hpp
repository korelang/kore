#ifndef KORE_INTEGER_TYPE_HPP
#define KORE_INTEGER_TYPE_HPP

#include "types/type.hpp"

namespace kore {
    // Type for i32, a 32-bit integer
    class IntegerType : public Type {
        public:
            IntegerType(int num_bits);
            virtual ~IntegerType();

            std::string name() const override;
            int num_bits() const noexcept;
            const Type* unify(const Type* other_type) const override;
            const Type* unify(const IntegerType* int_type) const override;
            void write(AstWriter* const writer) const override;

        private:
            int _num_bits;
    };
}

#endif // KORE_INTEGER_TYPE_HPP
