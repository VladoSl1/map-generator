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
    * We are working with polygons of sizes roughly 3-8.
    *
    * TODO: consider using vector instead of array
    * TODO: consider preloading the edges into smaller array to avoid cache misses
    */
    PolygonI getIndicesPolygon(const std::vector<EdgeI>& edges,
                               const PolygonI& edgePolygon)
    {
        assert(edgePolygon.size() >= 3 && "edgePolygon must have at least 3 edges");

        PolygonI indicesPolygon;
        const size_t vertexCount = edgePolygon.size();
        indicesPolygon.indices.reserve(vertexCount);

        std::vector<bool> usedEdges(vertexCount, false);

        usedEdges[0] = true;
        const EdgeI& firstEdge = edges[edgePolygon[0]];
        indicesPolygon.indices.push_back(firstEdge[0]);
        size_t currentVertex = firstEdge[1];

        // starting from one since we already processed the first edge
        for (size_t i = 1; i < vertexCount; ++i)
        {
            indicesPolygon.indices.push_back(currentVertex);

            for (size_t j = 1; j < vertexCount; ++j)
            {
                if (usedEdges[j])
                {
                    continue;
                }

                const EdgeI& edge = edges[edgePolygon[j]];
                if (edge[0] == currentVertex)
                {
                    currentVertex = edge[1];
                    usedEdges[j] = true;
                    break;
                }
                if (edge[1] == currentVertex)
                {
                    currentVertex = edge[0];
                    usedEdges[j] = true;
                    break;
                }
            }
        }

        return std::move(indicesPolygon);
    }
}
