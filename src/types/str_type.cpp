#include "ast/ast_writer.hpp"
#include "types/str_type.hpp"
#include "types/unknown_type.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    StrType::StrType() : Type(TypeCategory::Str) {}

    StrType::~StrType() {}

    std::string StrType::name() const {
        return "str";
    }

    const Type* StrType::unify(const Type* other_type) const {
        switch (other_type->category()) {
            case TypeCategory::Str:
            case TypeCategory::Char:
                return other_type->unify(this);

            default:
                return Type::unknown();
        }
    }

    const Type* StrType::unify(const StrType* str_type) const {
        UNUSED_PARAM(str_type);
        return this;
    }

    void StrType::write(AstWriter* const writer) {
        writer->write("str");
    }
}
