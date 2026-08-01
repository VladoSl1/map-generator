#pragma once

#include <iostream>
#include <source_location>
#include <string_view>


// #define DEBUG_FLAG

template <typename... Args>
void log(Args&&... args) {
#ifdef DEBUG_FLAG
    std::cout << "[DEBUG] ";

    // C++17 fold expression: expands to std::cout << arg1 << arg2 << ...
    (std::cout << ... << std::forward<Args>(args)) << std::endl;
#endif
}
