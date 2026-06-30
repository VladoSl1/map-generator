#include "math.hpp"


namespace math
{
    Point2Dd calculateCircumcenter(const Point2Dd& A, const Point2Dd& B, const Point2Dd& C)
    {
        double D = 2 * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
        if (D == 0)
        {
            throw std::runtime_error("Points are collinear; circumcenter is undefined.");
         }

        double Ux = ((A.x * A.x + A.y * A.y) * (B.y - C.y) +
                     (B.x * B.x + B.y * B.y) * (C.y - A.y) +
                     (C.x * C.x + C.y * C.y) * (A.y - B.y)) / D;

        double Uy = ((A.x * A.x + A.y * A.y) * (C.x - B.x) +
                     (B.x * B.x + B.y * B.y) * (A.x - C.x) +
                     (C.x * C.x + C.y * C.y) * (B.x - A.x)) / D;

        return {Ux, Uy};

    }

    bool shareEdge(const TriangleI& triangleA, const TriangleI& triangleB)
    {
        size_t sharedCount = 0;

        // Count how many vertex indices are common to both triangles
        for (size_t idxA : triangleA.indices)
        {
            for (size_t idxB : triangleB.indices)
            {
                if (idxA == idxB)
                {
                    sharedCount++;
                    break; // Found a match for idxA, move to the next one
                }
            }
        }

        return sharedCount == 2;
    }

    template<size_t N>
    void bucketSortPrimitives(std::vector<math::IndexPrimitive<N>>& triangles, size_t vertexIndex)
    {
        const int n = triangles.size();
        if (n <= 1) return;

        std::vector<size_t> counts(n, 0);

        // count how many times each vertex value appears
        for (const auto& triangle : triangles)
        {
            counts[triangle[vertexIndex]]++;
        }

        // convert counts to prefix sums to determine the final index positions
        for (size_t i = 1; i < n; ++i)
        {
            counts[i] += counts[i - 1];
        }

        // build the sorted output array
        std::vector<math::IndexPrimitive<N>> output(n);
        for (int i = static_cast<int>(n) - 1; i >= 0; --i)
        {
            size_t val = triangles[i][vertexIndex];
            output[counts[val] - 1] = triangles[i];
            counts[val]--;
        }

        triangles = std::move(output);
    }

    template<size_t N>
    void sortPrimitives(std::vector<math::IndexPrimitive<N>>& primitive)
    {
        for (int vertexIndex = 2; vertexIndex >= 0; --vertexIndex)
        {
            bucketSortPrimitives(primitive, vertexIndex);
        }
    }

}
