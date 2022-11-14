#include "ast/ast_writer.hpp"
#include "types/float_type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    FloatType::FloatType(int num_bits)
        : Type(num_bits == 32 ? TypeCategory::Float32 : TypeCategory::Float64),
        _num_bits(num_bits) {
    }

    FloatType::~FloatType() {}

    std::string FloatType::name() const {
        return "f" + std::to_string(_num_bits);
    }

    int FloatType::num_bits() const noexcept {
        return _num_bits;
    }

    const Type* FloatType::unify(const Type* other_type) const {
        switch (other_type->category()) {
            case TypeCategory::Float32:
            case TypeCategory::Float64:
                return other_type->unify(this);

            default:
                return Type::unknown();
        }
    }

    const Type* FloatType::unify(const FloatType* float_type) const {
        // Return the type of the float with the most bits
        return float_type->num_bits() > num_bits() ? float_type : this;
    }

    void FloatType::write(AstWriter* const writer) const {
        writer->write(name());
    }
}
