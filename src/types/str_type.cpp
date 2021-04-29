#include "ast/ast_writer.hpp"
#include "types/str_type.hpp"

StrType::StrType() : Type(TypeCategory::Str) {}

StrType::~StrType() {}

void StrType::write(AstWriter* const writer) {
    writer->write("str");
}
