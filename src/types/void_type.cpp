#include "ast/ast_writer.hpp"
#include "utils/unused_parameter.hpp"
#include "void_type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    VoidType::VoidType() : Type(TypeCategory::Void) {}

    VoidType::~VoidType() {}

    std::string VoidType::name() const {
        return "void";
    }

    const Type* VoidType::unify(const Type* other_type) const {
        return other_type->category() == TypeCategory::Void ? other_type->unify(this) : Type::unknown();
    }

    const Type* VoidType::unify(const VoidType* void_type) const {
        UNUSED_PARAM(void_type);

        return this;
    }

    void VoidType::write(AstWriter* const writer) const {
        writer->write("void");
    }
}
