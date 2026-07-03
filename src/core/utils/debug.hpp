#pragma once

#include <iostream>
#include <source_location>
#include <string_view>


#define DEBUG_FLAG true;

template <typename... Args>
void log(std::format_string<Args...> fmt, Args&&... args) {
#ifdef DEBUG_FLAG
    std::cout << "[DEBUG]";
    std::cout << std::format(fmt, std::forward<Args>(args)...) << std::endl;
#endif
}
