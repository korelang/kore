#include "ast/ast_writer.hpp"
#include "types/integer_type.hpp"

#include <sstream>

IntegerType::IntegerType(int num_bits)
    : Type(TypeCategory::Integer),
      _num_bits(num_bits) {
}

IntegerType::~IntegerType() {}

void IntegerType::write(AstWriter* const writer) {
    std::ostringstream oss;
    
    if (_num_bits == 8) {
        oss << "byte";
    } else {
        oss << "i" << _num_bits;
    }

    writer->write(oss.str());
}
