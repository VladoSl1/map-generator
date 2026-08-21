#include "geometry.hpp"

#include "constants.hpp"
#include "point2d.hpp"

#include <cassert>
#include <cmath>
#include <optional>
#include <vector>

namespace math
{
    std::optional<Point2Dd> calculateCircumcenter(const Point2Dd& A, const Point2Dd& B, const Point2Dd& C)
    {
        const double D = 2 * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));

        if (std::abs(D) < EPSILON)
        {
            return std::nullopt; // Points are collinear; circumcenter is undefined.
        }

        const double Ux = ((A.x * A.x + A.y * A.y) * (B.y - C.y) +
                     (B.x * B.x + B.y * B.y) * (C.y - A.y) +
                     (C.x * C.x + C.y * C.y) * (A.y - B.y)) / D;

        const double Uy = ((A.x * A.x + A.y * A.y) * (C.x - B.x) +
                     (B.x * B.x + B.y * B.y) * (A.x - C.x) +
                     (C.x * C.x + C.y * C.y) * (B.x - A.x)) / D;

        return std::optional<Point2Dd>({Ux, Uy});
    }

    double calculateSignedPolygonArea(const std::vector<Point2Dd>& points, const PolygonI& polygon)
    {
        const auto& indices = polygon.indices;
        assert(indices.size() >= 3 && "Polygon must have at least 3 vertices to determine orientation.");

        double signedArea = 0.0;
        for (size_t j = 0; j < indices.size(); ++j)
        {
            const Point2Dd& v1 = points[indices[j]];
            const Point2Dd& v2 = points[indices[(j + 1) % indices.size()]];

            signedArea += (v1.x * v2.y - v2.x * v1.y);
        }

        return signedArea / 2.0;
    }

    Point2Dd calculatePolygonCentroid(const std::vector<Point2Dd>& points, const PolygonI& polygon)
    {
        const auto& indices = polygon.indices;
        assert(indices.size() >= 3 && "Polygon must have at least 3 vertices to calculate centroid.");

        double signedArea = 0.0;
        Point2Dd centroid{0.0, 0.0};

        for (size_t i = 0; i < indices.size(); ++i)
        {
            const Point2Dd& v1 = points[indices[i]];
            const Point2Dd& v2 = points[indices[(i + 1) % indices.size()]];

            double crossProduct = (v1.x * v2.y - v2.x * v1.y);
            signedArea += crossProduct;
            centroid += (v1 + v2) * crossProduct;
        }

        signedArea /= 2.0;

        if (std::abs(signedArea) < EPSILON)
        {
            // degenerate polygon, return the average of the vertices as a fallback
            centroid = {0.0, 0.0};
            for (const auto& index : indices)
            {
                centroid += points[index];
            }
            centroid /= static_cast<double>(indices.size());
        }
        else
        {
            centroid /= (6.0 * signedArea);
        }

        return centroid;
    }

}

