#include "keywords.hpp"
#include <set>

/* A 1 in the table indicates that this ASCII character is the first character
 * in some keyword, 0 means that it is not.
 *
 * Index 0 contains the value for 'a' and so on.
 */
const int _KEYWORD_FIRST_CHAR_TABLE[] = {
    0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
};

std::set<std::string> _KEYWORDS{
    "bool",
    "byte",
    "export",
    "enum",
    "f32",
    "f64",
    "false",
    "for",
    "func",
    "i8",
    "i16",
    "i32",
    "i64",
    "if",
    "import",
    "in",
    "is",
    "match",
    "module",
    "None",
    "Some",
    "str",
    "struct",
    "true",
    "try",
    "type",
    "u8",
    "u16",
    "u32",
    "u64",
};

bool is_keyword(const std::string& identifier) {
    char byte = identifier[0];
    int idx = static_cast<int>(byte - (byte <= 'Z' ? 'A' : 'a'));

    if (idx >= 0) {
        if (_KEYWORD_FIRST_CHAR_TABLE[idx]) {
            return _KEYWORDS.count(identifier);
        }
    }

    return false;
}
