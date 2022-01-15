#ifndef KORE_DEBUG_TIME_HPP
#define KORE_DEBUG_TIME_HPP

#include <chrono>
#include <functional>

#include "logging/logging.hpp"

void debug_time(const std::string& what, const std::function<void (void)>& task) {
    auto start = std::chrono::high_resolution_clock::now();
    task();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed);

    debug_group("timing", "%s took %.2f ms", what.c_str(), elapsed_time.count() / 1000000.0);
}

#endif // KORE_DEBUG_TIME_HPP
