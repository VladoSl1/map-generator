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

#ifdef DEBUG_FLAG
inline std::unordered_map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> g_debug_clocks;
#endif

inline void startClock(const std::string& name = "default") {
#ifdef DEBUG_FLAG
    g_debug_clocks[name] = std::chrono::high_resolution_clock::now();
#endif
}

inline void stopClock(const std::string& name = "default") {
#ifdef DEBUG_FLAG
    auto end_time = std::chrono::high_resolution_clock::now();

    auto it = g_debug_clocks.find(name);
    if (it != g_debug_clocks.end()) {
        auto start_time = it->second;
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        log("Clock [", name, "] took ", duration.count(), " ms");

        g_debug_clocks.erase(it);
    } else {
        log("Warning: Clock [", name, "] stopped without being started.");
    }
#endif
}
