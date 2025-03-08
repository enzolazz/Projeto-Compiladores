#ifndef NDEBUG
#include "debug.hpp"

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Don't use this namespace directly in your code
namespace __debug {
void varsNames(std::size_t i, const std::vector<std::string>& names) {
    std::cerr << std::endl;
}

void varsNamesPrefix(const char*&& prefix, const char*&& msg) {
    std::cerr << prefix << ' ' << std::quoted(msg) << std::endl;
}
}  // namespace __debug
#endif
