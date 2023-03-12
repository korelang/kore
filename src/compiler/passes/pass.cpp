#include "compiler/passes/pass.hpp"

namespace kore {
    Pass::Pass(const std::string& name) : name(name) {}

    Pass::~Pass() {}

    std::string Pass::name() const {
        return name;
    }
}
