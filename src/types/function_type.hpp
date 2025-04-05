#ifndef KORE_FUNCTION_TYPE_HPP
#define KORE_FUNCTION_TYPE_HPP

#include <vector>

#include "types/type.hpp"

namespace kore {
    class FunctionType : public Type {
        public:
            FunctionType();
            virtual ~FunctionType();

            std::string name() const override;
            void add_parameter_type(const Type* parameter_type);
            int arity() const noexcept;
            int return_arity() const noexcept;
            const Type* get_parameter_type(int idx) const;
            void set_parameter_types(const std::vector<const Type*> parameter_types);
            const Type* return_type(int idx) const;
            void set_return_types(const std::vector<const Type*> return_types);
            void add_return_type(const Type* type);

            const Type* unify(const Type* other_type) const override;
            const Type* unify(const FunctionType* func_type) const override;

            void add_parameter();
            void set_return_type();

            void write(AstWriter* const writer) const override;

            std::string create_function_type_name();

            static std::string create_function_type_name(
                const std::vector<const Type*>& parameter_types,
                const std::vector<const Type*>& return_types
            );

        private:
            std::string _name;
            std::vector<const Type*> _parameter_types;
            std::vector<const Type*> _return_types;

            std::string create_name();
    };
}

#endif // KORE_FUNCTION_TYPE_HPP
