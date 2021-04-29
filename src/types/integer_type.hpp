#ifndef KORE_INTEGER_TYPE_HPP
#define KORE_INTEGER_TYPE_HPP

#include "types/type.hpp"

// Type for i32, a 32-bit integer
class IntegerType : public Type {
    public:
        IntegerType(int num_bits);
        virtual ~IntegerType();

        void write(AstWriter* const writer) override;

    private:
        int _num_bits;
};

#endif // KORE_INTEGER_TYPE_HPP
