#include "geometry.hpp"

#include "constants.hpp"
#include "point2d.hpp"

#include <cmath>
#include <optional>

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
}

