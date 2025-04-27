#ifndef KORE_KEYWORDS_HPP
#define KORE_KEYWORDS_HPP

#include <string>

namespace kore {
    enum class Keyword {
        Bool,
        Byte,
        Char,
        Else,
        Enum,
        Export,
        F32,
        F64,
        False,
        For,
        Func,
        I16,
        I32,
        I64,
        I8,
        If,
        Import,
        In,
        Is,
        Match,
        Module,
        None,
        Return,
        Some,
        Str,
        Struct,
        True,
        Try,
        Type,
        U16,
        U32,
        U64,
        U8,
        Var
    };

    bool is_keyword(const std::string& identifier);

    Keyword keyword_from_string(const std::string& value);
}

#endif // KORE_KEYWORDS_HPP
