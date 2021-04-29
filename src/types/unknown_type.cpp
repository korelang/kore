#include "ast/ast_writer.hpp"
#include "types/unknown_type.hpp"

UnknownType::UnknownType() : Type(TypeCategory::Unknown) {}

UnknownType::~UnknownType() {}

void UnknownType::write(AstWriter* const writer) {
    writer->write("unknown");
}
