#ifndef KORE_ARRAY_TYPE_HPP
#define KORE_ARRAY_TYPE_HPP

#include "types/type.hpp"

class ArrayType : public Type {
    public:
        ArrayType(Type* element_type);
        virtual ~ArrayType();

        void set_rank(int rank);
        void increase_rank();
        int rank() const noexcept;

        void write(AstWriter* const writer) override;

    private:
        int _rank;
        std::unique_ptr<Type> _element_type;
};

#endif // KORE_ARRAY_TYPE_HPP
