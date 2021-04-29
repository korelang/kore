#include "ast/ast_writer.hpp"
#include "types/bool_type.hpp"

BoolType::BoolType() : Type(TypeCategory::Bool) {}

BoolType::~BoolType() {}

void BoolType::write(AstWriter* const writer) {
    writer->write("bool");
}
