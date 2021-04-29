#include "ast/ast_writer.hpp"
#include "types/char_type.hpp"

CharType::CharType() {}

CharType::~CharType() {}

void CharType::write(AstWriter* const writer) {
    writer->write("char");
}
