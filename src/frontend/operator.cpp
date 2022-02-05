#include <map>

#include "operator.hpp"

namespace kore {
    const static std::map<std::string, int> _OPERATOR_PRECEDENCE_MAP{
        // Binary arithmetic
        {"&", 0},
        {"|", 0},
        {"=", 0},
        {"!=", 0},
        {">>", 0},
        {"<<", 0},
        {">>=", 0},
        {"<<=", 0},

        // Comparison
        {">=", 1},
        {">", 1},
        {"<", 1},
        {"<=", 1},

        // Range
        {"..", 1},

        // Arithmetic
        {"+", 2},
        {"-", 2},
        {"/", 3},
        {"*", 3},
        {"%", 3},
        {"**", 4},

        // Other
        {"=>", 0},
        {".", 4},
        {"?", 4},
    };

    int operator_precedence(const std::string& op) {
        const auto iter = _OPERATOR_PRECEDENCE_MAP.find(op);

        if (iter != _OPERATOR_PRECEDENCE_MAP.cend()) {
            return iter->second;
        }

        return -1;
    }

    int operator_base_precedence() {
        return 0;
    }
}
