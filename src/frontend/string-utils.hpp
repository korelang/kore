#ifndef KORE_STRING_UTILS_HPP
#define KORE_STRING_UTILS_HPP

#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>

void split_on(const std::string& value, const char delimiter, std::vector<std::string>& result);

template<typename Range, typename Value = typename Range::value_type>
std::string join_on(const Range& range, const char* const delimiter) {
    std::ostringstream os;
    auto begin = std::cbegin(range), end = std::cend(range);

    if (begin != end) {
        std::copy(begin, std::prev(end), std::ostream_iterator<Value>(os, delimiter));
        begin = std::prev(end);
    }

    if (begin != end) {
        os << *begin;
    }

    return os.str();
}

#endif // KORE_STRING_UTILS_HPP
