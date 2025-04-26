#include <sstream>

#include "ast/ast_writer.hpp"
#include "ast/expressions/identifier.hpp"
#include "types/function_type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    FunctionType::FunctionType() : Type(TypeCategory::Function), _name(create_name()) {
    }

    FunctionType::~FunctionType() {
    }

    std::string FunctionType::name() const {
        return _name;
    }

    void FunctionType::add_parameter_type(const Type* parameter_type) {
        _parameter_types.push_back(parameter_type);
    }

    int FunctionType::arity() const noexcept {
        return _parameter_types.size();
    }

    int FunctionType::return_arity() const noexcept {
        return _return_types.size();
    }

    const Type* FunctionType::get_parameter_type(int idx) const {
        return _parameter_types[idx];
    }

    void FunctionType::set_parameter_types(const std::vector<const Type*> parameter_types) {
        _parameter_types = parameter_types;
    }

    const Type* FunctionType::return_type(int idx) const {
        return _return_types[idx];
    }

    void FunctionType::set_return_types(const std::vector<const Type*> return_types) {
        _return_types = return_types;
    }

    void FunctionType::add_return_type(const Type* type) {
        _return_types.push_back(type);
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
            auto param_type1 = _parameter_types[i];
            auto param_type2 = func_type->get_parameter_type(i);
            auto unified_type = param_type1->unify(param_type2);

            if (unified_type->is_unknown()) {
                return unified_type;
            }
        }

        return this;
    }

    void FunctionType::write(AstWriter* const writer) const {
        writer->write(_name);
    }

    std::string FunctionType::create_name() {
        if (!_name.empty()) {
            return _name;
        }

        std::vector<const Type*> parameter_types;

        for (const auto& parameter_type : _parameter_types) {
            parameter_types.push_back(parameter_type);
        }

        _name = FunctionType::create_function_type_name(parameter_types, _return_types);

        return _name;
    }

    std::string FunctionType::create_function_type_name() const {
        return create_function_type_name(_parameter_types, _return_types);
    }

    // TODO: Probably want to cache this
    std::string FunctionType::create_function_type_name(
        const std::vector<const Type*>& parameter_types,
        const std::vector<const Type*>& return_types
    ) {
        std::ostringstream oss;

        oss << "(";

        for (size_t idx = 0; idx < parameter_types.size(); ++idx) {
            auto& param = parameter_types[idx];

            oss << param->name();

            if (idx != parameter_types.size() - 1) {
                oss << ", ";
            }
        }

        oss << ") => ";

        for (size_t idx = 0; idx < return_types.size(); ++idx) {
            auto& return_type = return_types[idx];

            oss << return_type->name();

            if (idx != return_types.size() - 1) {
                oss << ", ";
            }
        }

        return oss.str();
    }
}
