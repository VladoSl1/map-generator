#include "topology.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>

#include "core/utils/debug.hpp"


namespace math
{
    // assumes that the points are sorted
    bool doesShareEdge(const TriangleI& triangleA, const TriangleI& triangleB)
    {
        assert(std::ranges::is_sorted(triangleA.indices) && "TriangleA is not sorted");
        assert(std::ranges::is_sorted(triangleB.indices) && "TriangleB is not sorted");

        std::array<size_t, 3> shared_buffer{};

        auto result = std::ranges::set_intersection(
            triangleA.indices,
            triangleB.indices,
            shared_buffer.begin()
        );

        // calculate how many indices were written to the buffer
        const size_t sharedCount = std::distance(shared_buffer.begin(), result.out);

        return sharedCount == 2;
    }

    /*
     * Algorithm works as follows:
     *  1. Add first two vertices of the first edge to the polygon
     *  2. For each edge in the polygon, find the next edge that shares a vertex with the current edge
     *     by iterating through all edges in the polygon
     *  3. Add the other vertex of the found edge to the polygon
    * This O(n^2) algorithm should be faster then linear algorithm which uses hash table
    * Preloading edges into stack array speeds up the algorithm by avoiding cache misses when accessing edges vector
    * (this happend in every iteration of the inner loop).
    *
    */
    PolygonI getIndicesPolygon(const std::vector<EdgeI>& edges, const PolygonI& edgePolygon)
    {
        constexpr size_t MAX_LOCAL = 32;

        const size_t vertexCount = edgePolygon.size();
        assert(vertexCount >= 3 && "edgePolygon must have at least 3 edges");
        assert(vertexCount <= MAX_LOCAL && "polygon degree exceeds bitmask capacity - increase MAX_LOCAL");

        // preload edges into a local array to avoid cache misses
        std::array<EdgeI, MAX_LOCAL> localEdges;
        for (size_t i = 0; i < vertexCount; ++i)
        {
            localEdges[i] = edges[edgePolygon[i]];
        }

        PolygonI indicesPolygon;
        indicesPolygon.indices.resize(vertexCount);

        indicesPolygon.indices[0] = localEdges[0][0];
        size_t currentVertex = localEdges[0][1];

        uint64_t usedMask = 1ull;  // already processed the first edge (index 0)

        // starting from one since we already processed the first edge
        for (size_t i = 1; i < vertexCount; ++i)
        {
            indicesPolygon.indices[i] = currentVertex;

            for (size_t j = 1; j < vertexCount; ++j)
            {
                const size_t comparedEdgeMask = 1ull << j;
                if (usedMask & comparedEdgeMask)
                {
                    continue;
                }

                const EdgeI& edge = localEdges[j];
                if (edge[0] == currentVertex)
                {
                    currentVertex = edge[1];
                    usedMask |= comparedEdgeMask;
                    break;
                }
                if (edge[1] == currentVertex)
                {
                    currentVertex = edge[0];
                    usedMask |= comparedEdgeMask;
                    break;
                }
            }
        }

        return indicesPolygon;
    }
}
