#include "ast/ast_writer.hpp"
#include "types/integer_type.hpp"
#include "types/unknown_type.hpp"
#include "utils/unused_parameter.hpp"

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

int IntegerType::num_bits() const noexcept {
    return _num_bits;
}

const Type* IntegerType::unify(const Type* other_type) const {
    if (other_type->category() == category()) {
        return other_type->unify(this);
    }
    
    return Type::unknown();
}

const Type* IntegerType::unify(const IntegerType* int_type) const {
    // Return the type of the integer with the most bits
    return int_type->num_bits() > this->num_bits() ? int_type : this;
}

void IntegerType::write(AstWriter* const writer) {
    writer->write(name());
}
