#include "topology.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>


namespace math
{
    // assumes that the points are sorted
    bool doesShareEdge(const TriangleI& triangleA, const TriangleI& triangleB)
    {
        assert(std::ranges::is_sorted(triangleA.indices) && "TriangleA is not sorted");
        assert(std::ranges::is_sorted(triangleB.indices) && "TriangleB is not sorted");

        std::array<size_t, 3> shared_buffer;

        auto result = std::ranges::set_intersection(
            triangleA.indices,
            triangleB.indices,
            shared_buffer.begin()
        );

        // calculate how many indices were written to the buffer
        const size_t sharedCount = std::distance(shared_buffer.begin(), result.out);

        return sharedCount == 2;
    }
}
