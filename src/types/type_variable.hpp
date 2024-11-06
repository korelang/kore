#ifndef KORE_TYPE_VARIABLE_HPP
#define KORE_TYPE_VARIABLE_HPP

// namespace kore {
//  struct TypeVariable {
//      Type* type;
//      
//      std::string name() const;
//  
//      static TypeVariable fresh_type_variable();
//      
//      private:
//          std::string _name;
//      
//          static unsigned int _typevar_id = 0;
//  };
// }

#include "types/type.hpp"

namespace kore {
    /// A type variable representing an as of yet unknown type. It isn't
    /// useable by users but used during the compiler's type inference passes
    class TypeVariable : public Type {
        public:
            TypeVariable();
            virtual ~TypeVariable();

            /// Generate a fresh (unique) type variable
            static TypeVariable fresh();           

            std::string name() const override;
            void write(AstWriter* const writer) const override;

      private:
          std::string _name;
      
          static unsigned int _typevar_id;

          TypeVariable(unsigned int typevar_id);
    };
}

#endif // KORE_TYPE_VARIABLE_HPP
