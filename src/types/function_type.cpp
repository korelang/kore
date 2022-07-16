#include <sstream>

#include "ast/ast_writer.hpp"
#include "ast/expressions/identifier.hpp"
#include "ast/expressions/parameter.hpp"
#include "types/function_type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    // TODO
    FunctionType::FunctionType() : Type(TypeCategory::Function) {
    }

    FunctionType::~FunctionType() {
    }

    std::string FunctionType::name() const {
        return create_name(false);
    }

    void FunctionType::add_parameter(Parameter* parameter) {
        _parameters.emplace_back(parameter);
    }

    int FunctionType::arity() const {
        return _parameters.size();
    }

    const Identifier* FunctionType::parameter(int param_index) const {
        return _parameters[param_index].get();
    }

    const Type* FunctionType::return_type() const {
        return _return_type.get();
    }

    void FunctionType::set_return_type(Type* type) {
        _return_type.reset(type);
    }

    const Type* FunctionType::unify(const Type* other_type) const {
        switch (other_type->category()) {
            case TypeCategory::Function:
                return other_type->unify(this);

            default:
                return Type::unknown();
        }
    }

    const Type* FunctionType::unify(const FunctionType* func_type) const {
        if (arity() != func_type->arity()) {
            return Type::unknown();
        }

        // TODO: Do we handle co- and contravariance here?
        for (int i = 0; i < arity(); ++i) {
            auto param1 = _parameters[i]->type();
            auto param2 = func_type->parameter(i)->type();
            auto unified_type = param1->unify(param2);

            if (unified_type->is_unknown()) {
                return unified_type;
            }
        }

        return this;
    }

    void FunctionType::write(AstWriter* const writer) const {
        writer->write(create_name(true));
    }

    std::string FunctionType::create_name(bool with_arg_names) const {
        std::ostringstream oss;

        oss << "(";

        for (int i = 0; i < arity(); ++i) {
            auto& param = _parameters[i];

            oss << param->type()->name();

            if (with_arg_names) {
                oss << " " << param->name();
            }

            if (i != arity() - 1) {
                oss << ", ";
            }
        }

        oss << ") => " << _return_type->name();

        return oss.str();
    }
}
