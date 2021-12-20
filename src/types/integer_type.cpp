#include "ast/ast_writer.hpp"
#include "types/integer_type.hpp"

#include <sstream>

IntegerType::IntegerType(int num_bits)
    : Type(TypeCategory::Integer),
      _num_bits(num_bits) {
}

IntegerType::~IntegerType() {}

std::string IntegerType::name() const {
    if (_num_bits == 8) {
        return "byte";
    }

    std::ostringstream oss;
    oss << "i" << _num_bits;

    return oss.str();
}

void IntegerType::write(AstWriter* const writer) {
    writer->write(name());
}
