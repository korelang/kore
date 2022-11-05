#include "passes/pass.hpp"

namespace kore {
    Pass::Pass(const std::string& name) : _name(name) {}

    Pass::~Pass() {}

    std::string Pass::name() const {
        return _name;
    }
}
