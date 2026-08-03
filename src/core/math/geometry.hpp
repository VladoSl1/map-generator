#pragma once

#include "point2d.hpp"

#include <optional>

namespace math
{
    /* algorithm: https://en.wikipedia.org/wiki/Circumcircle#Circumcenter_coordinates */
    std::optional<Point2Dd> calculateCircumcenter(const Point2Dd& A, const Point2Dd& B, const Point2Dd& C);
}
