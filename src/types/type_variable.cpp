#include <string>

#include "ast/ast_writer.hpp"
#include "types/type_variable.hpp"

namespace kore {
    unsigned int TypeVariable::_typevar_id = 0;

    TypeVariable::TypeVariable() {}

    TypeVariable::TypeVariable(unsigned int typevar_id)
        : _name("t" + std::to_string(typevar_id)) {
    }

    TypeVariable::~TypeVariable() {}

    TypeVariable TypeVariable::fresh() {
        return TypeVariable(_typevar_id++);
    }

    std::string TypeVariable::name() const {
        return _name;
    }

    void TypeVariable::write(AstWriter* const writer) const {
        writer->write(name());
    }
}
