#pragma once

#include <vector>

#include "core/math/point2d.hpp"
#include "core/math/topology.hpp"


namespace generation::pipeline
{
    struct VoronoiDiagram
    {
        std::vector<math::Point2Di> seeds;
        std::vector<math::Point2Di> vertices;
        std::vector<math::EdgeI> edges;
        std::vector<math::PolygonI> polygons;

        void assignEdgeToPolygon(size_t edgeIndex, size_t polygonIndex);

        void generate(const std::vector<math::Point2Di>& trianglePoints,
                      const std::vector<math::TriangleI>& triangleIndices);

        void clear();
    };


    std::vector<math::Point2Di> findVoronoiVertices(const std::vector<math::Point2Di>& trianglePoints,
                                                     const std::vector<math::TriangleI>& triangleIndices);

    void relaxVoronoiDiagram(VoronoiDiagram& voronoiDiagram);
}
