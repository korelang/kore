#include "keywords.hpp"
#include <map>

/* A 1 in the table indicates that this ASCII character is the first character
 * in some keyword, 0 means that it is not.
 *
 * Index 0 contains the value for 'a' and so on.
 */
const int _KEYWORD_FIRST_CHAR_TABLE[] = {
    0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
};

std::map<std::string, Keyword> _STRING_TO_KEYWORDS{
    {"bool",   Keyword::Bool},
    {"byte",   Keyword::Byte},
    {"char",   Keyword::Char},
    {"export", Keyword::Export},
    {"enum",   Keyword::Enum},
    {"f32",    Keyword::F32},
    {"f64",    Keyword::F64},
    {"false",  Keyword::False},
    {"for",    Keyword::For},
    {"func",   Keyword::Func},
    {"i8",     Keyword::I8},
    {"i16",    Keyword::I16},
    {"i32",    Keyword::I32},
    {"i64",    Keyword::I64},
    {"if",     Keyword::If},
    {"import", Keyword::Import},
    {"in",     Keyword::In},
    {"is",     Keyword::Is},
    {"match",  Keyword::Match},
    {"module", Keyword::Module},
    {"None",   Keyword::None},
    {"Some",   Keyword::Some},
    {"str",    Keyword::Str},
    {"struct", Keyword::Struct},
    {"true",   Keyword::True},
    {"try",    Keyword::Try},
    {"type",   Keyword::Type},
    {"u8",     Keyword::U8},
    {"u16",    Keyword::U16},
    {"u32",    Keyword::U32},
    {"u64",    Keyword::U64},
};

bool is_keyword(const std::string& identifier) {
    char byte = identifier[0];
    int idx = static_cast<int>(byte - (byte <= 'Z' ? 'A' : 'a'));

    if (idx >= 0) {
        if (_KEYWORD_FIRST_CHAR_TABLE[idx]) {
            return _STRING_TO_KEYWORDS.count(identifier);
        }
    }

    return false;
}

Keyword keyword_from_string(const std::string& value) {
    return _STRING_TO_KEYWORDS[value];
}
