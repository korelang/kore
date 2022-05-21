#include "ast/ast_writer.hpp"
#include "types/float_type.hpp"
#include "types/unknown_type.hpp"
#include "utils/unused_parameter.hpp"

#include <sstream>

namespace kore {
    FloatType::FloatType(int num_bits)
        : Type(TypeCategory::Float),
        _num_bits(num_bits) {
    }

    FloatType::~FloatType() {}

    std::string FloatType::name() const {
        // TODO: Support both float32 and float64
        return "float";
    }

    int FloatType::num_bits() const noexcept {
        return _num_bits;
    }

    const Type* FloatType::unify(const Type* other_type) const {
        switch (other_type->category()) {
            case TypeCategory::Float:
                return other_type->unify(this);

            default:
                return Type::unknown();
        }
    }

    /* const Type* FloatType::unify(const FloatType* int_type) const { */
    /*     // Return the type of the float with the most bits */
    /*     return int_type->num_bits() > this->num_bits() ? int_type : this; */
    /* } */

    void FloatType::write(AstWriter* const writer) const {
        writer->write(name());
    }
}
