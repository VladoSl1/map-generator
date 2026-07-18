#pragma once

#include <vector>

#include "core/math/point2d.hpp"
#include "core/math/topology.hpp"


namespace generation::pipeline
{
    struct VoronoiDiagram
    {
        std::vector<math::Point2Dd> seeds;
        std::vector<math::Point2Dd> vertices;
        std::vector<math::EdgeI> edges;
        /* PolygonI elements are indices of edges in the edges vector */
        std::vector<math::PolygonI> polygons;

        void assignEdgeToPolygon(size_t edgeIndex, size_t polygonIndex);

        void generate(std::vector<math::Point2Dd>& trianglePoints,
                      const std::vector<math::TriangleI>& triangleIndices);

        void clear();

        bool isPolygonClosed(size_t polygonIndex) const;
    };


    std::vector<math::Point2Dd> findVoronoiVertices(const std::vector<math::Point2Dd>& trianglePoints,
                                                     const std::vector<math::TriangleI>& triangleIndices);

    std::vector<math::Point2Dd> relaxVoronoiDiagram(const VoronoiDiagram& voronoiDiagram);
}
