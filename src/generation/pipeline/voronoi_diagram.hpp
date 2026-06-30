#pragma once

#include <vector>

#include "core/math/math.hpp"


namespace generation::pipeline
{
    struct VoronoiDiagram
    {
        std::vector<math::Point2Di> vertices;
        std::vector<math::EdgeI> edges;

    };

    std::vector<math::Point2Di> findVoronoiVerticies(std::vector<math::Point2Di> trianglePoints,
                                                     std::vector<math::TriangleI> triangleIndices);

    VoronoiDiagram generateVoronoi(std::vector<math::Point2Di> trianglePoints,
                                   std::vector<math::TriangleI> triangleIndices);


}
