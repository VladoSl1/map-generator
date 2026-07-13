#pragma once

#include "point2d.hpp"

namespace math
{

    constexpr double EPSILON = 1e-5;

    /* algorithm: https://en.wikipedia.org/wiki/Circumcircle#Circumcenter_coordinates */
    Point2Dd calculateCircumcenter(const Point2Dd& A, const Point2Dd& B, const Point2Dd& C);
}
