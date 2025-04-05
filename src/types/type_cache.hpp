#ifndef KORE_TYPE_CACHE_HPP
#define KORE_TYPE_CACHE_HPP

#include "pointer_types.hpp"
#include "type_category.hpp"

#include <unordered_map>
#include <vector>

namespace kore {
    class Type;
    class ArrayType;
    class Optional;
    class FunctionType;

    /// A class to manage shared type definitions across type-related
    /// operations (e.g. inference and checking)
    class TypeCache final {
        public:
            using TypeId = int;

        public:
            TypeCache();
            virtual ~TypeCache();

            const Type* get_type(TypeCategory category);
            ArrayType* get_array_type(const Type* element_type);
            const Optional* get_optional_type(const Type* contained_type);
            const FunctionType* get_function_type(
                const std::vector<const Type*>& parameter_types,
                const std::vector<const Type*>& return_types
            );
            void set_function_type(std::unique_ptr<FunctionType>&& func_type);

        private:
            void _add_type(Owned<Type>&& type);
            void define_basic_builtin_types();

            template<typename T>
            T* get_non_simple_type(const Type* contained_type, std::unordered_map<const Type*, Owned<T>>& cache) {
                auto it = cache.find(contained_type);

                if (it == cache.end()) {
                    auto result = cache.insert({contained_type, std::make_unique<T>(contained_type)});

                    it = result.first;
                }

                return it->second.get();
            }

        private:
            std::unordered_map<TypeCategory, Owned<Type>> _type_cache;
            std::unordered_map<std::string, Owned<FunctionType>> _function_type_cache;
            std::unordered_map<const Type*, Owned<ArrayType>> _array_type_cache;
            std::unordered_map<const Type*, Owned<const Optional>> _optional_type_cache;
    };
}

#endif // KORE_TYPE_CACHE_HPP
