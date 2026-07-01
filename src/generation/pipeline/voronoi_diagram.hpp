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

    VoronoiDiagram generateVoronoi(const std::vector<math::Point2Di>& trianglePoints,
                                   const std::vector<math::TriangleI>& triangleIndices);

    std::vector<math::Point2Di> findVoronoiVerticies(const std::vector<math::Point2Di>& trianglePoints,
                                                     const std::vector<math::TriangleI>& triangleIndices);
}
