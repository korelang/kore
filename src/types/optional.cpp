#include "ast/ast_writer.hpp"
#include "types/optional.hpp"
#include "types/unknown_type.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    Optional::Optional(Type* contained_type)
        : Type(TypeCategory::Optional),
        _contained_type(std::move(contained_type)) {
    }

    Optional::~Optional() {}

    std::string Optional::name() const {
        return _contained_type->name() + "?";
    }

    const Type* Optional::unify(const Type* other_type) const {
        switch (other_type->category()) {
            case TypeCategory::Optional:
                return other_type->unify(this);

            default:
                if (!other_type->unify(_contained_type.get())->is_unknown()) {
                    // If we can unify the contained type of this optional
                    // with the other type, we're good
                    return this;
                }

                return Type::unknown();
        }
    }

    const Type* Optional::unify(const Optional* optional) const {
        return _contained_type->unify(optional->_contained_type.get());
    }

    void Optional::write(AstWriter* const writer) const {
        _contained_type->write(writer);
        writer->write("?");
    }
}
