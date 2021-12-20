#include "ast/ast_writer.hpp"
#include "types/optional.hpp"

Optional::Optional(Type* contained_type)
    : Type(TypeCategory::Optional),
      _contained_type(std::move(contained_type)) {
}

Optional::~Optional() {}

std::string Optional::name() const {
    return _contained_type->name() + "?";
}

void Optional::write(AstWriter* const writer) {
    _contained_type->write(writer);
    writer->write("?");
}
