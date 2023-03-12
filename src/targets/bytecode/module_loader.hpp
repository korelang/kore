#ifndef KORE_MODULE_LOADER_HPP
#define KORE_MODULE_LOADER_HPP

#include <filesystem>

#include "module.hpp"

namespace fs = std::filesystem;

namespace kore {
    kore::Module load_module_from_path(const fs::path& path);
    kore::Module load_module_from_stream(std::istream& is);
}

#endif // KORE_MODULE_LOADER_HPP
