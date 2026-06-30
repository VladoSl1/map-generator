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
}
