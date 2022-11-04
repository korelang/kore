#ifndef KORE_FUNCTION_TYPE_HPP
#define KORE_FUNCTION_TYPE_HPP

#include <vector>

#include "ast/expressions/expression.hpp"
#include "pointer_types.hpp"
#include "types/type.hpp"

namespace kore {
    class Parameter;

    using ParameterList = std::vector<Owned<Identifier>>;

    class FunctionType : public Type {
        public:
            FunctionType();
            virtual ~FunctionType();

            std::string name() const override;
            void add_parameter(Parameter* parameter);
            int arity() const;
            const Identifier* parameter(int param_index) const;
            const Type* return_type() const;
            void set_return_type(Type* type);

            const Type* unify(const Type* other_type) const override;
            const Type* unify(const FunctionType* func_type) const override;

            void add_parameter();
            void set_return_type();

            void write(AstWriter* const writer) const override;

        private:
            Type::pointer _return_type;
            ParameterList _parameters;

            std::string create_name(bool with_arg_names) const;
    };
}

#endif // KORE_FUNCTION_TYPE_HPP
