#pragma once

#include "point2d.hpp"
#include "topology.hpp"

#include <optional>
#include <vector>

namespace math
{
    /* algorithm: https://en.wikipedia.org/wiki/Circumcircle#Circumcenter_coordinates */
    std::optional<Point2Dd> calculateCircumcenter(const Point2Dd& A, const Point2Dd& B, const Point2Dd& C);

    /* https://en.wikipedia.org/wiki/Shoelace_formula */
    void orderPolygonClockwise(std::vector<Point2Dd> points, PolygonI& polygon);
}
