#ifndef KORE_IDENTIFIER_HPP
#define KORE_IDENTIFIER_HPP

#include <vector>

#include "expression.hpp"

namespace kore {
    class Identifier : public Expression {
        public:
            Identifier(const Token& token);
            Identifier(const std::string& value, const Location& location);
            Identifier(const std::vector<std::string>& value, const Location& location);
            virtual ~Identifier();

            std::string name() const;
            std::string qualified_name() const;
            bool is_qualified() const noexcept;
            std::size_t size() const noexcept;

            void write(AstWriter* const writer) override;

        private:
            std::string _value;
            std::vector<std::string> _parts;
    };
}

#endif // KORE_IDENTIFIER_HPP
