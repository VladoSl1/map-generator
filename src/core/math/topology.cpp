#include "topology.hpp"


namespace math
{
    bool doesShareEdge(const TriangleI& triangleA, const TriangleI& triangleB)
    {
        std::array<size_t, 3> shared_buffer;

        auto result = std::ranges::set_intersection(
            triangleA.indices,
            triangleB.indices,
            shared_buffer.begin()
        );

        // calculate how many indices were written to the buffer
        size_t sharedCount = std::distance(shared_buffer.begin(), result.out);

        return sharedCount == 2;
    }
}
