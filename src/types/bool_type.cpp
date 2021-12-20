#include "ast/ast_writer.hpp"
#include "types/bool_type.hpp"

BoolType::BoolType() : Type(TypeCategory::Bool) {}

BoolType::~BoolType() {}

std::string BoolType::name() const {
    return "bool";
}

void BoolType::write(AstWriter* const writer) {
    writer->write("bool");
}
