#include <vector>

#include "ast/statements/statement.hpp"

namespace kore {
    using StatementList = std::vector<std::unique_ptr<Statement>>;
}
