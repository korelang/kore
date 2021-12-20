#ifndef KORE_STR_TYPE_HPP
#define KORE_STR_TYPE_HPP

#include "types/type.hpp"

class StrType : public Type {
    public:
        StrType();
        virtual ~StrType();

        std::string name() const override;
        void write(AstWriter* const writer) override;
};

#endif // KORE_STR_TYPE_HPP
