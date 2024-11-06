#include "utils/index_to_ordinal.hpp"

namespace kore {
    namespace utils {
        std::string index_to_ordinal(int index) {
            switch (index) {
                case 0: return "0th";
                case 1: return "1st";
                case 2: return "2nd";
                case 3: return "3rd";
                default: return "th";
            }
        }
    }
}
