#ifndef KORE_BOOL_TYPE_HPP
#define KORE_BOOL_TYPE_HPP

#include "types/type.hpp"

class BoolType : public Type {
    public:
        BoolType();
        virtual ~BoolType();

        std::string name() const override;

        void write(AstWriter* const writer) override;
};

#endif // KORE_BOOL_TYPE_HPP
