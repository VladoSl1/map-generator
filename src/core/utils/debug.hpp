#pragma once

#include <iostream>
#include <source_location>
#include <string_view>


#define DEBUG_FLAG true;


void debug_log(const std::string_view message,
               const std::source_location location = std::source_location::current()) {
#ifdef DEBUG_FLAG
    std::clog << "file: "
              << location.file_name() << '('
              << location.line() << ':'
              << location.column() << ") `"
              << location.function_name() << "`: "
              << message << '\n';
#endif
}
