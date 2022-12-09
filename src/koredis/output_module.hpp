#ifndef KOREDIS_OUTPUT_MODULE_HPP
#define KOREDIS_OUTPUT_MODULE_HPP

#include <ostream>

#include "constant_table.hpp"
#include "logging/color_attributes.hpp"
#include "logging/colors.hpp"
#include "logging/logging.hpp"
#include "module.hpp"

namespace koredis {
    std::string constant_table_tag_to_string(kore::ConstantTableTag tag);

    template<typename T>
    void output_constant_table(std::ostream& os, kore::Color color, const kore::ConstantTable<T>& table) {
        kore::section("constant table", kore::Color::Magenta, kore::ColorAttribute::Bold, 0, "%s", constant_table_tag_to_string(table.tag()).c_str());

        int table_index = 0;

        for (auto it = table.sorted_cbegin(); it != table.sorted_cend(); ++it) {
            os << color << "| " << kore::ColorAttribute::Reset
               << "#" << table_index++ << " => " << (*it) << std::endl;
        }
    }

    void output_module(std::ostream& os, kore::Module& module, bool colors, bool porcelain, int verbosity);
}

#endif // KOREDIS_OUTPUT_MODULE_HPP
