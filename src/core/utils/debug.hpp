#pragma once

#include <iostream>
#include <source_location>
#include <string_view>


#define DEBUG_FLAG

template <typename... Args>
void log(Args&&... args) {
#ifdef DEBUG_FLAG
    std::cout << "[DEBUG] ";
    (std::cout << ... << std::forward<Args>(args)) << std::endl;
#endif
}
