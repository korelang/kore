#include <sstream>

#include "string-utils.hpp"

namespace kore {
    void split_on(const std::string& value, const char delimiter, std::vector<std::string>& result) {
        std::istringstream iss(value);
        std::string token;

        while (std::getline(iss, token, delimiter)) {
            if (!token.empty()) {
                result.push_back(token);
            }
        }
    }
}
