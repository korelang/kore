#include "ast/ast_writer.hpp"
#include "types/integer_type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    IntegerType::IntegerType(int num_bits)
        : Type(num_bits == 32 ? TypeCategory::Integer32 : TypeCategory::Integer64),
        _num_bits(num_bits) {
    }

    IntegerType::~IntegerType() {}

    std::string IntegerType::name() const {
        if (_num_bits == 8) {
            return "byte";
        }

        return "i" + std::to_string(_num_bits);
    }

    int IntegerType::num_bits() const noexcept {
        return _num_bits;
    }

    const Type* IntegerType::unify(const Type* other_type) const {
        switch (other_type->category()) {
            case TypeCategory::Integer32:
            case TypeCategory::Integer64:
                return other_type->unify(this);

            default:
                return Type::unknown();
        }
    }

    const Type* IntegerType::unify(const IntegerType* int_type) const {
        // Return the type of the integer with the most bits
        return int_type->num_bits() > num_bits() ? int_type : this;
    }

    void IntegerType::write(AstWriter* const writer) const {
        writer->write(name());
    }
}
