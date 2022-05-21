#ifndef KORE_FLOAT_TYPE_HPP
#define KORE_FLOAT_TYPE_HPP

#include "types/type.hpp"

namespace kore {
    // Type for i32, a 32-bit float
    class FloatType : public Type {
        public:
            FloatType(int num_bits);
            virtual ~FloatType();

            std::string name() const override;
            int num_bits() const noexcept;
            const Type* unify(const Type* other_type) const override;
            /* const Type* unify(const FloatType* float_type) const override; */
            void write(AstWriter* const writer) const override;

        private:
            int _num_bits;
    };
}

#endif // KORE_FLOAT_TYPE_HPP
