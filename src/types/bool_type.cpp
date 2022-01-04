#include "ast/ast_writer.hpp"
#include "types/bool_type.hpp"
#include "types/unknown_type.hpp"
#include "utils/unused_parameter.hpp"

BoolType::BoolType() : Type(TypeCategory::Bool) {}

BoolType::~BoolType() {}

std::string BoolType::name() const {
    return "bool";
}

const Type* BoolType::unify(const Type* other_type) const {
    if (other_type->category() == category()) {
        return other_type->unify(this);
    }
    
    return Type::unknown();
}

const Type* BoolType::unify(const BoolType* bool_type) const {
    UNUSED_PARAM(bool_type);
    return this;
}

void BoolType::write(AstWriter* const writer) {
    writer->write("bool");
}
