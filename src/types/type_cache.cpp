#include "array_type.hpp"
#include "bool_type.hpp"
#include "char_type.hpp"
#include "float_type.hpp"
#include "integer_type.hpp"
#include "optional.hpp"
#include "str_type.hpp"
#include "type.hpp"
#include "types/type_cache.hpp"
#include "unknown_type.hpp"
#include "void_type.hpp"

#include <memory>

namespace kore {
    TypeCache::TypeCache() {
        define_basic_builtin_types();
    }

    TypeCache::~TypeCache() {}

    const Type* TypeCache::get_type(TypeCategory category) {
        return _type_cache[category].get();
    }

    ArrayType* TypeCache::get_array_type(const Type* contained_type) {
        return get_non_simple_type(contained_type, _array_type_cache);
    }

    const Optional* TypeCache::get_optional_type(const Type* contained_type) {
        return get_non_simple_type(contained_type, _optional_type_cache);
    }

    void TypeCache::define_basic_builtin_types() {
        _type_cache.emplace(TypeCategory::Unknown, std::make_unique<UnknownType>());
        _type_cache.emplace(TypeCategory::Void, std::make_unique<VoidType>());
        _type_cache.emplace(TypeCategory::Byte, std::make_unique<IntegerType>(8));
        _type_cache.emplace(TypeCategory::Integer32, std::make_unique<IntegerType>(32));
        _type_cache.emplace(TypeCategory::Integer64, std::make_unique<IntegerType>(64));
        _type_cache.emplace(TypeCategory::Float32, std::make_unique<FloatType>(32));
        _type_cache.emplace(TypeCategory::Float64, std::make_unique<FloatType>(64));
        _type_cache.emplace(TypeCategory::Char, std::make_unique<CharType>());
        _type_cache.emplace(TypeCategory::Str, std::make_unique<StrType>());
        _type_cache.emplace(TypeCategory::Bool, std::make_unique<BoolType>());

        // Array types of all basic types
        get_array_type(get_type(TypeCategory::Byte));
        get_array_type(get_type(TypeCategory::Integer32));
        get_array_type(get_type(TypeCategory::Integer64));
        get_array_type(get_type(TypeCategory::Float32));
        get_array_type(get_type(TypeCategory::Float64));
        get_array_type(get_type(TypeCategory::Char));
        get_array_type(get_type(TypeCategory::Str));
        get_array_type(get_type(TypeCategory::Bool));

        // Optional types of all basic types
        get_optional_type(get_type(TypeCategory::Byte));
        get_optional_type(get_type(TypeCategory::Integer32));
        get_optional_type(get_type(TypeCategory::Integer64));
        get_optional_type(get_type(TypeCategory::Float32));
        get_optional_type(get_type(TypeCategory::Float64));
        get_optional_type(get_type(TypeCategory::Char));
        get_optional_type(get_type(TypeCategory::Str));
        get_optional_type(get_type(TypeCategory::Bool));
    }
}
