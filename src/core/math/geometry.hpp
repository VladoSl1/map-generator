#pragma once

#include "point2d.hpp"
#include "topology.hpp"

#include <optional>
#include <vector>

namespace math
{
    /* alg: https://en.wikipedia.org/wiki/Circumcircle#Circumcenter_coordinates */
    std::optional<Point2Dd> calculateCircumcenter(const Point2Dd& A, const Point2Dd& B, const Point2Dd& C);

    /* alg: https://en.wikipedia.org/wiki/Shoelace_formula */
    double calculateSignedPolygonArea(const std::vector<Point2Dd>& points, const PolygonI& polygon);

    /* alg: https://en.wikipedia.org/wiki/Centroid#By_geometric_decomposition
     * If the polygon is degenerate and has near zero area, the return value is the average
     * of point coordinates instead */
    Point2Dd calculatePolygonCentroid(const std::vector<Point2Dd>& points, const PolygonI& polygon);
}
