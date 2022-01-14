#include "ast/ast_writer.hpp"
#include "types/char_type.hpp"
#include "types/unknown_type.hpp"
#include "utils/unused_parameter.hpp"

CharType::CharType() : Type(TypeCategory::Char) {}

CharType::~CharType() {}

std::string CharType::name() const {
    return "char";
}

const Type* CharType::unify(const Type* other_type) const {
    switch (other_type->category()) {
        case TypeCategory::Char:
            return other_type->unify(this);

        default:
            return Type::unknown();
    }
}

const Type* CharType::unify(const CharType* char_type) const {
    UNUSED_PARAM(char_type);
    return this;
}

void CharType::write(AstWriter* const writer) {
    writer->write("char");
}
