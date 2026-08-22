#pragma once

#include <array>
#include <cassert>
#include <cstddef>

namespace math
{
    /* wrapper around std::array that allows comfortable inserting of elements */
    template <typename T, size_t Capacity>
    struct StaticVector
    {
        std::array<T, Capacity> data{};
        size_t count = 0;

        constexpr void push_back(T value)
        {
            assert(count < Capacity && "StaticVector: capacity exceeded");
            data[count++] = value;
        }

        constexpr void resize(size_t n) { count = n; }
        constexpr bool empty() const    { return count == 0; }
        constexpr size_t size() const   { return count; }

        constexpr T&       operator[](size_t i)       { return data[i]; }
        constexpr const T& operator[](size_t i) const { return data[i]; }

        constexpr auto begin()        { return data.begin(); }
        constexpr auto end()          { return data.begin() + count; }
        constexpr auto begin()  const { return data.begin(); }
        constexpr auto end()    const { return data.begin() + count; }
    };
}
