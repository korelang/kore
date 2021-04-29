#include "ast/ast_writer.hpp"
#include "types/array_type.hpp"

ArrayType::ArrayType(Type* element_type)
    : Type(TypeCategory::Array),
      _rank(1),
      _element_type(std::move(element_type)) {
}

ArrayType::~ArrayType() {}

void ArrayType::set_rank(int rank) {
    _rank = rank;
}

void ArrayType::increase_rank() {
    ++_rank;
}

int ArrayType::rank() const noexcept {
    return _rank;
}

void ArrayType::write(AstWriter* const writer) {
    _element_type->write(writer);

    for (int i = 0; i < _rank; ++i) {
        writer->write("[]");
    }
}
