#ifndef KORE_OPERATOR_HPP
#define KORE_OPERATOR_HPP

#include <string>

enum class Operator {
    invalid,
    bor,
    band,
    equal,
    not_equal,
    lt,
    le,
    gt,
    ge,
    plus,
    minus,
    mult,
    div,
    mod,
    exp,
    lshift,
    rshift,
};

int operator_precedence(const std::string& op);

int operator_base_precedence();

#endif // KORE_OPERATOR_HPP
